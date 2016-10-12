

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


//
//
//
typedef struct
{
    char*       start;
    uint32_t    length;

} Piece;


//
//
//
void OpenFile(char* fileName)
{
    struct stat     sb;
    off_t           len;
    char*           p;
    int             fd;

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



