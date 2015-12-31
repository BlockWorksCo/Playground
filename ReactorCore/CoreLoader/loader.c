//
// Originally from https://github.com/embedded2014/elf-loader.git
//


#include "loader.h"
#include "elf.h"
#include "loader_config.h"

#define IS_FLAGS_SET(v, m) ((v&m) == m)
#define SECTION_OFFSET(e, n) (e->sectionTable + n * sizeof(Elf32_Shdr))


//
//
//
typedef struct
{
    void*       data;
    uint32_t    physicalAddress;
    int         secIdx;
    off_t       relSecIdx;

} ELFSection_t;


//
//
//
typedef struct
{
    int fd;

    size_t sections;
    off_t sectionTable;
    off_t sectionTableStrings;

    size_t symbolCount;
    off_t symbolTable;
    off_t symbolTableStrings;
    off_t entry;

    ELFSection_t text;
    ELFSection_t rodata;
    ELFSection_t data;
    ELFSection_t bss;

    void* stack;

    const ELFEnv_t* env;
} ELFExec_t;



//
//
//
typedef enum
{
    FoundERROR = 0,
    FoundSymTab = (1 << 0),
    FoundStrTab = (1 << 2),
    FoundText = (1 << 3),
    FoundRodata = (1 << 4),
    FoundData = (1 << 5),
    FoundBss = (1 << 6),
    FoundRelText = (1 << 7),
    FoundRelRodata = (1 << 8),
    FoundRelData = (1 << 9),
    FoundRelBss = (1 << 10),
    FoundValid = FoundSymTab | FoundStrTab,
    FoundExec = FoundValid | FoundText,
    FoundAll = FoundSymTab | FoundStrTab | FoundText | FoundRodata | FoundData
               | FoundBss | FoundRelText | FoundRelRodata | FoundRelData | FoundRelBss
} FindFlags_t;




//
//
//
static int readSectionName(ELFExec_t* e, off_t off, char* buf, size_t max)
{
    int ret = -1;
    off_t offset = e->sectionTableStrings + off;
    off_t old = LOADER_TELL(e->fd);

    if (lseek(e->fd, offset, SEEK_SET) != -1)
    {
        if (read(e->fd, buf, max) == 0)
        {
            ret = 0;
        }
    }

    (void) lseek(e->fd, old, SEEK_SET);
    return ret;
}


//
//
//
static int readSymbolName(ELFExec_t* e, off_t off, char* buf, size_t max)
{
    int ret = -1;
    off_t offset = e->symbolTableStrings + off;
    off_t old = LOADER_TELL(e->fd);

    if (lseek(e->fd, offset, SEEK_SET) != -1)
    {
        if (read(e->fd, buf, max) == 0)
        {
            ret = 0;
        }
    }

    (void) lseek(e->fd, old, SEEK_SET);

    return ret;
}


//
//
//
static uint32_t swabo(uint32_t hl)
{
    return ((((hl) >> 24)) | /* */
            (((hl) >> 8) & 0x0000ff00) | /* */
            (((hl) << 8) & 0x00ff0000) | /* */
            (((hl) << 24))); /* */
}




//
//
//
static void loadSecData(ELFExec_t* e, ELFSection_t* s, Elf32_Shdr* h)
{
    if (h->sh_size == 0)
    {
        //
        // NO data for section.
        //
        MSG(" No data for section");
    }
    else
    {
        //
        // This section has data, so attempt to load it.
        //
        uint32_t    physicalAddress;
        s->data = LOADER_ALIGN_ALLOC(h->sh_size, h->sh_addralign, h->sh_flags, &s->physicalAddress);

        if (s->data == NULL)
        {
            ERR("memory allocation failure.");
        }

        if (lseek(e->fd, h->sh_offset, SEEK_SET) == -1)
        {
            ERR("    seek fail");
        }

        if (read(e->fd, s->data, h->sh_size) != h->sh_size)
        {
            ERR("     read data fail");
        }        
    }
}


//
//
//
static void readSecHeader(ELFExec_t* e, int n, Elf32_Shdr* h)
{
    off_t offset = SECTION_OFFSET(e, n);

    if (lseek(e->fd, offset, SEEK_SET) == -1)
    {
        ERR("Can't seek to section start.");
    }

    if (read(e->fd, h, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr))
    {
        ERR("Can't read section header.");
    }
}

static int readSection(ELFExec_t* e, int n, Elf32_Shdr* h, char* name,
                       size_t nlen)
{
    readSecHeader(e, n, h);

    if (h->sh_name)
    {
        return readSectionName(e, h->sh_name, name, nlen);
    }

    return 0;
}

static int readSymbol(ELFExec_t* e, int n, Elf32_Sym* sym, char* name,
                      size_t nlen)
{
    int ret = -1;
    off_t old = LOADER_TELL(e->fd);
    off_t pos = e->symbolTable + n * sizeof(Elf32_Sym);

    if (lseek(e->fd, pos, SEEK_SET) != -1)
    {
        if (read(e->fd, sym, sizeof(Elf32_Sym)) == sizeof(Elf32_Sym))
        {
            if (sym->st_name)
            {
                ret = readSymbolName(e, sym->st_name, name, nlen);
            }

            else
            {
                Elf32_Shdr shdr;
                ret = readSection(e, sym->st_shndx, &shdr, name, nlen);
            }
        }
    }

    (void) lseek(e->fd, old, SEEK_SET);
    return ret;
}

static const char* typeStr(int symt)
{
#define STRCASE(name) case name: return #name;

    switch (symt)
    {
        STRCASE(R_ARM_NONE)
        STRCASE(R_ARM_ABS32)
        STRCASE(R_ARM_THM_PC22)
        STRCASE(R_ARM_THM_JUMP24)
        STRCASE(R_ARM_THM_MOVW_ABS_NC)
        STRCASE(R_ARM_THM_MOVT_ABS)

    default:
        return "R_<unknow>";
    }

#undef STRCASE
}



static void relJmpCall(Elf32_Addr relAddr, int type, Elf32_Addr symAddr)
{
    uint16_t upper_insn = ((uint16_t*) relAddr)[0];
    uint16_t lower_insn = ((uint16_t*) relAddr)[1];
    uint32_t S = (upper_insn >> 10) & 1;
    uint32_t J1 = (lower_insn >> 13) & 1;
    uint32_t J2 = (lower_insn >> 11) & 1;
    int32_t offset = (S << 24) | /* S     -> offset[24] */
                     ((~(J1 ^ S) & 1) << 23) | /* J1    -> offset[23] */
                     ((~(J2 ^ S) & 1) << 22) | /* J2    -> offset[22] */
                     ((upper_insn & 0x03ff) << 12) | /* imm10 -> offset[12:21] */
                     ((lower_insn & 0x07ff) << 1); /* imm11 -> offset[1:11] */

    if (offset & 0x01000000)
    {
        offset -= 0x02000000;
    }

    offset += symAddr - relAddr;
    S = (offset >> 24) & 1;
    J1 = S ^ (~(offset >> 23) & 1);
    J2 = S ^ (~(offset >> 22) & 1);
    upper_insn = ((upper_insn & 0xf800) | (S << 10) | ((offset >> 12) & 0x03ff));
    ((uint16_t*) relAddr)[0] = upper_insn;
    lower_insn = ((lower_insn & 0xd000) | (J1 << 13) | (J2 << 11)
                  | ((offset >> 1) & 0x07ff));
    ((uint16_t*) relAddr)[1] = lower_insn;
}

#define swab16(x) ( (((x) << 8) & 0xFF00) | (((x) >> 8) & 0x00FF) )


//
//
//
static void relR_ARM_THM_MOVW_ABS_NC(Elf32_Addr relAddr, int type, Elf32_Addr symAddr)
{
    uint32_t    upper_insn = ( ((uint16_t*) relAddr)[0] );
    uint32_t    lower_insn = ( ((uint16_t*) relAddr)[1] );
    int32_t     offset;
    offset =    ((upper_insn & 0x000f) << 12) |
                ((upper_insn & 0x0400) << 1) |
                ((lower_insn & 0x7000) >> 4) | (lower_insn & 0x00ff);
    offset =    (offset ^ 0x8000) - 0x8000;
    offset +=   symAddr;
    upper_insn = (uint16_t)((upper_insn & 0xfbf0) |
                            ((offset & 0xf000) >> 12) |
                            ((offset & 0x0800) >> 1));
    lower_insn = (uint16_t)((lower_insn & 0x8f00) |
                            ((offset & 0x0700) << 4) |
                            (offset & 0x00ff));
    ((uint16_t*) relAddr)[0] = ( upper_insn );
    ((uint16_t*) relAddr)[1] = ( lower_insn );
}




//
//
//
static void relR_ARM_THM_MOVT_ABS(Elf32_Addr relAddr, int type, Elf32_Addr symAddr)
{
    uint32_t    upper_insn = ( ((uint16_t*) relAddr)[0] );
    uint32_t    lower_insn = ( ((uint16_t*) relAddr)[1] );
    int32_t     offset;
    offset =    ((upper_insn & 0x000f) << 12) |
                ((upper_insn & 0x0400) << 1) |
                ((lower_insn & 0x7000) >> 4) | (lower_insn & 0x00ff);
    offset =    (offset ^ 0x8000) - 0x8000;
    offset +=   symAddr;
    offset >>= 16;
    upper_insn = (uint16_t)((upper_insn & 0xfbf0) |
                            ((offset & 0xf000) >> 12) |
                            ((offset & 0x0800) >> 1));
    lower_insn = (uint16_t)((lower_insn & 0x8f00) |
                            ((offset & 0x0700) << 4) |
                            (offset & 0x00ff));
    ((uint16_t*) relAddr)[0] = ( upper_insn );
    ((uint16_t*) relAddr)[1] = ( lower_insn );
}




//
// Modify the value to be relocated according to the relocation-type.
//
static void relocateSymbol(Elf32_Addr relAddr, int type, Elf32_Addr symAddr)
{
    switch (type)
    {
        case R_ARM_ABS32:
            *((uint32_t*) relAddr) += symAddr;
            DBG("  R_ARM_ABS32 relocated is 0x%08X\n", (unsigned int) * ((uint32_t*)relAddr));
            break;

        case R_ARM_THM_PC22:
        case R_ARM_THM_JUMP24:
            relJmpCall(relAddr, type, symAddr);
            DBG("  R_ARM_THM_CALL/JMP relocated is 0x%08X\n", (unsigned int) * ((uint32_t*)relAddr));
            break;

        case R_ARM_THM_MOVW_ABS_NC:
            relR_ARM_THM_MOVW_ABS_NC(relAddr, type, symAddr);
            DBG("  R_ARM_THM_MOVW_ABS_NC relocated is 0x%08X\n", (unsigned int) * ((uint32_t*)relAddr));
            break;

        case R_ARM_THM_MOVT_ABS:
            relR_ARM_THM_MOVT_ABS(relAddr, type, symAddr);
            DBG("  R_ARM_THM_MOVT_ABS relocated is 0x%08X\n", (unsigned int) * ((uint32_t*)relAddr));
            break;

        default:
            ERR("  Undefined relocation type.");
            break;
    }
}


//
// Return the section with the matching index value.
//
static ELFSection_t* sectionOf(ELFExec_t* e, int index)
{
    ELFSection_t*   section     = NULL;

    if(e->text.secIdx == index)
    {
        section     = &e->text;
    }

    if(e->rodata.secIdx == index)
    {
        section     = &e->rodata;
    }

    if(e->data.secIdx == index)
    {
        section     = &e->data;
    }

    if(e->bss.secIdx == index)
    {
        section     = &e->bss;
    }

    if(section == NULL)
    {
        ERR("Unknown section.");
    }

    return section;
}


//
// Determine the address of the given symbol within the sections or the 
// enviromment if undefined.
//
static Elf32_Addr addressOf(ELFExec_t* e, Elf32_Sym* sym, const char* sName)
{
    uint32_t    address     = 0xffffffff;

    if (sym->st_shndx == SHN_UNDEF)
    {
        printf("<undefined symbol %s>\n", sName);

        //
        // Undefined symbol, look it up from the environment for linking.
        //
        for (uint32_t i = 0; i < e->env->exported_size; i++)
        {
            if (LOADER_STREQ(e->env->exported[i].name, sName))
            {
                address     = (Elf32_Addr)(e->env->exported[i].ptr);
            }            
        }
    }
    else
    {
        //
        // Defined symbol so get the section that contains it and work out the address.
        //
        ELFSection_t* symSec = sectionOf(e, sym->st_shndx);
        //address = ((Elf32_Addr) symSec->data) + sym->st_value;
        address     = ((Elf32_Addr) symSec->physicalAddress) + sym->st_value;
    }

    if( address == 0xffffffff)
    {
        ERR("  Can not find address for symbol.");
    }

    return address;
}


//
//
//
static void relocate(ELFExec_t* e, Elf32_Shdr* h, ELFSection_t* s, const char* name)
{
    if (s->data != NULL)
    {
        Elf32_Rel   rel;
        size_t      relEntries = h->sh_size / sizeof(rel);
        size_t      relCount;

        (void) lseek(e->fd, h->sh_offset, SEEK_SET);

        //
        // For every relocation entry...
        //
        DBG(" Offset   Info     Type             Name\n");

        for (relCount = 0; relCount < relEntries; relCount++)
        {
            if (read(e->fd, &rel, sizeof(rel)) == sizeof(rel))
            {
                Elf32_Sym   sym;
                Elf32_Addr  symAddr;
                char        name[33]    = "<unnamed>";
                int         symEntry    = ELF32_R_SYM(rel.r_info);
                int         relType     = ELF32_R_TYPE(rel.r_info);
                Elf32_Addr  relAddr     = ((Elf32_Addr) s->data) + rel.r_offset;

                readSymbol(e, symEntry, &sym, name, sizeof(name));
                DBG(" %08X %08X %-16s %s\n", (unsigned int) rel.r_offset, (unsigned int) rel.r_info, typeStr(relType), name);

                //
                // Get the address to relocate to.
                //
                symAddr = addressOf(e, &sym, name);

                //
                // Do the relocation.
                //
                DBG("  symAddr=%08X relAddr=%08X\n", (unsigned int) symAddr, (unsigned int) relAddr);
                relocateSymbol(relAddr, relType, symAddr);
            }
        }
    }
    else
    {
        ERR("Section not loaded");
    }
}


//
//
//
int placeInfo(ELFExec_t* e, Elf32_Shdr* sh, const char* name, int n)
{
    int type    = 0;

    if (strcmp(name, ".symtab") == 0)
    {
        e->symbolTable = sh->sh_offset;
        e->symbolCount = sh->sh_size / sizeof(Elf32_Sym);
        type = FoundSymTab;
    }
    else if (strcmp(name, ".strtab") == 0)
    {
        e->symbolTableStrings = sh->sh_offset;
        type = FoundStrTab;
    }
    else if (strcmp(name, ".text") == 0)
    {
        loadSecData(e, &e->text, sh);
        e->text.secIdx = n;
        type = FoundText;
    }
    else if (strcmp(name, ".rodata") == 0)
    {
        loadSecData(e, &e->rodata, sh);
        e->rodata.secIdx = n;
        type = FoundRodata;
    }
    else if (strcmp(name, ".data") == 0)
    {
        loadSecData(e, &e->data, sh);
        e->data.secIdx = n;
        type = FoundData;
    }
    else if (strcmp(name, ".bss") == 0)
    {
        loadSecData(e, &e->bss, sh);
        e->bss.secIdx = n;
        type = FoundBss;
    }
    else if (strcmp(name, ".rel.text") == 0)
    {
        e->text.relSecIdx = n;
        type = FoundRelText;
    }
    else if (strcmp(name, ".rel.rodata") == 0)
    {
        e->rodata.relSecIdx = n;
        type =  FoundRelText;
    }
    else if (strcmp(name, ".rel.data") == 0)
    {
        e->data.relSecIdx = n;
        type = FoundRelText;
    }

    return type;
}


//
//
//
static int loadSymbols(ELFExec_t* e)
{
    int n;
    int founded = 0;

    MSG("Scan ELF indexs...");

    //
    // For all sections...
    //
    for (n = 1; n < e->sections; n++)
    {
        Elf32_Shdr  sectHdr;
        char        name[33] = "<unamed>";

        //
        // Read the section header.
        //
        readSecHeader(e, n, &sectHdr);

        if (sectHdr.sh_name)
        {
            readSectionName(e, sectHdr.sh_name, name, sizeof(name));
        }

        DBG("Examining section %d %s\n", n, name);
        founded |= placeInfo(e, &sectHdr, name, n);

        if (IS_FLAGS_SET(founded, FoundAll))
        {
            return FoundAll;
        }
    }

    MSG("Done");
    return founded;
}


//
//
//
static void initElf(ELFExec_t* e, int f)
{
    Elf32_Ehdr h;
    Elf32_Shdr sH;

    if (!LOADER_FD_VALID(f))
    {
        ERR("Invalid file handle.");
    }

    LOADER_CLEAR(e, sizeof(ELFExec_t));

    if (read(f, &h, sizeof(h)) != sizeof(h))
    {
        ERR("Can't read the file header.");
    }

    e->fd = f;

    if (lseek(e->fd, h.e_shoff + h.e_shstrndx * sizeof(sH), SEEK_SET) == -1)
    {
        ERR("Cant seek to section start.");
    }

    if (read(e->fd, &sH, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr))
    {
        ERR("Can't read section header.");
    }

    e->entry                = h.e_entry;
    e->sections             = h.e_shnum;
    e->sectionTable         = h.e_shoff;
    e->sectionTableStrings  = sH.sh_offset;
}


//
//
//
static void relocateSection(ELFExec_t* e, ELFSection_t* s, const char* name)
{
    DBG("Relocating section %s\n", name);

    if (s->relSecIdx)
    {
        Elf32_Shdr sectHdr;

        readSecHeader(e, s->relSecIdx, &sectHdr);
        relocate(e, &sectHdr, s, name);
    }
    else
    {
        MSG("No relocation index");    /* Not an error */
    }
}


//
//
//
static void relocateSections(ELFExec_t* e)
{
    relocateSection(e, &e->text, ".text");
    relocateSection(e, &e->rodata, ".rodata");
    relocateSection(e, &e->data, ".data");
}

static int jumpTo(ELFExec_t* e)
{
    if (e->entry)
    {
        entry_t* entry = (entry_t*)(e->text.data + e->entry);
        LOADER_JUMP_TO(entry);
        return 0;
    }

    else
    {
        MSG("No entry defined.");
        return -1;
    }
}



//
// Top level call to load a .elf file.
//
void exec_elf(const char* path, const ELFEnv_t* env)
{
    ELFExec_t exec;

    //
    // Setup file handle and read file header and initial section header.
    //
    initElf(&exec, open(path, O_RDONLY) );

    //
    // Store the environment symbols.
    //
    exec.env = env;

    //
    // If we have all the required sections,then relocate the sections.
    //
    int     r   = loadSymbols(&exec);
    if (IS_FLAGS_SET(r, FoundValid))
    {
        //
        // Relocate all the sections.
        //
        relocateSections(&exec);
        printf("ELF loaded... executing offset %08x PC @ %08x\n", (uint32_t)(exec.entry), (uint32_t)(exec.text.data + exec.entry) );
        
        //
        // Call the newly loaded executable.
        //
        jumpTo(&exec);
    }
    else
    {
        ERR("Invalid EXEC");
    }
}
