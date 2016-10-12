

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
void AddPiece( char* start, uint32_t length )
{
    for(uint32_t i=0; i<numberOfPieces; i++)
    {
        if(pieces[i].length == 0)
        {
            pieces[i].start    = start;
            pieces[i].length   = length;
            return;
        }
    }

    pieces[numberOfPieces].start    = start;
    pieces[numberOfPieces].length   = length;
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
void Insert( char* start, uint32_t length )
{
    for(uint32_t i=0; i<numberOfPieces; i++)
    {
        uint32_t    startA  = (uint64_t)start;
        uint32_t    endA    = startA + length;
        uint32_t    startB  = (uint64_t)pieces[i].start;
        uint32_t    endB    = startB + length;

        //
        // A:  <------------->
        // B:         <--------------->
        //
        if( (startA < startB) && (endA >= startB) && (endA <= endB) )
        {

        }

        //
        // A:                 <------------->
        // B:        <--------------->
        //
        if( (startA >= startB) && (startA <= endB) && (endA >= endB) )
        {

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
#if 0
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

    //
    // Add the initial piece as the whole file.
    //
    AddPiece( p, sb.st_size );
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



