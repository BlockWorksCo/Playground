

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


#define MAX_PIECES      (1024)


//
//
//
typedef struct
{
    char*       start;
    uint32_t    length;

} Piece;



Piece       pieces[MAX_PIECES]  = {0};
uint32_t    numberOfPieces      = 0;


//
//
//
void ModifyPiece( uint32_t i, char* start, uint32_t length )
{
    pieces[i].start    = start;
    pieces[i].length   = length;
}



//
//
//
void AddPiece( char* start, uint32_t length )
{
    for(uint32_t i=0; i<numberOfPieces; i++)
    {
        if(pieces[i].length == 0)
        {
            ModifyPiece( i, start,length );
            return;
        }
    }

    ModifyPiece( numberOfPieces, start,length );
    numberOfPieces++;
}



//
//
//
void RemovePiece( uint32_t i )
{
    pieces[i].length   = 0;
}


//
//
//
void InsertPiece( char* start, uint32_t length )
{
    for(uint32_t i=0; i<numberOfPieces; i++)
    {
        uint64_t    startA  = (uint64_t)start;
        uint64_t    endA    = startA + length;
        uint64_t    startB  = (uint64_t)pieces[i].start;
        uint64_t    endB    = startB + length;

        //
        // A:  <------------->
        // B:         <--------------->
        //
        if( (startA < startB) && (endA >= startB) && (endA <= endB) )
        {
            ModifyPiece( i, (char*)startA,endA );
            AddPiece( (char*)(endA+1),endB );
        }

        //
        // A:                 <------------->
        // B:        <--------------->
        //
        if( (startA >= startB) && (startA <= endB) && (endA >= endB) )
        {
            ModifyPiece( i, (char*)startA,endA );
            AddPiece( (char*)(endA+1),endB );
        }

        //
        // A:                 <----->
        // B:         <--------------->
        //
        if( (startA >= startB) && (endA <= endB) )
        {

        }

        //
        // A:     <------------------------>
        // B:         <--------------->
        //
        if( (startA <= startB) && (endA >= endB) )
        {

        }
    }
}


//
//
//
void OpenFile(char* fileName)
{
    struct stat     sb;
    off_t           len;
    char*           p;
    int             fd;


    //
    // Open and map the file.
    //
    fd = open ( fileName, O_RDONLY);
    if(fd == -1) 
    {
        perror ("open");
        return;
    }

    if(fstat(fd,&sb)==-1) 
    {
        perror ("fstat");
        return;
    }

    if(!S_ISREG (sb.st_mode)) 
    {
        fprintf (stderr, "%s is not a file\n", fileName);
        return;
    }

    p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED) 
    {
        perror ("mmap");
        return;
    }

    //
    // Add the initial piece as the whole file.
    //
    AddPiece( p, sb.st_size );



#if 0
    //
    // Unmap and coe the file.
    //
    if(close (fd) == -1) 
    {
        perror ("close");
        return;
    }

    for(len = 0; len < sb.st_size; len++)
    {
        putchar(p[len]);
    }

    if(munmap (p, sb.st_size) == -1) 
    {
        perror ("munmap");
        return;
    }
#endif

}



//
//
//
void DumpFile()
{

}



//
//
//
int main(int argc, char* argv[])
{
    if(argc < 2) 
    {
        fprintf (stderr, "usage: %s <file>\n", argv[0]);
    }
    else
    {
        OpenFile( argv[1] );
    }
}



