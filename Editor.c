

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <stdlib.h>

#define NUMBER_OF_ELEMENTS(a)       (sizeof(a)/sizeof(a[0]))

#define MAX_PIECES      (1024)


//
//
//
typedef struct
{
    const char* text;
    uint32_t    length;

    uint32_t    next;
    uint32_t    last;

} Piece;


//
// Pieces is an array of Piece structures that together make up a continuous, seamless coverage of the total 
// buffer with no gaps:
//      0 <-----><--------------><-><-------><-----------><----------------> NumLines
// 
// The array is unsorted, each piece may be immutable.
// Possible insertion scenarios:
//
// A:
//           <----->
//        <------------------->
//               becomes:
//        <-><-----><--------->
//
// B:
//        <-------->
//            <--------------->
//               becomes:
//        <--------><--------->
//
// C:
//                   <-------->
//        <-------------->
//               becomes:
//        <---------><-------->
//
Piece       pieces[MAX_PIECES]  = {0};
uint32_t    numberOfPieces      = 0;




//
//
//
uint32_t FindUnusedPiece()
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(pieces); i++)
    {
        if( pieces[i].length == 0 )
        {
            return i;
        }
    }

    return -1;
}


//
//
//
void Remove( const uint32_t pieceToSplit, const uint32_t position, const uint32_t length )
{
    //
    // Work out the modifications needed.
    //
    uint32_t    lengthBefore        = position;
    uint32_t    lengthAfter         = (pieces[pieceToSplit].length - position) - length;

    //
    // We have text before and after the new piece
    //
    if( (lengthAfter > 0) && (lengthBefore > 0) )
    {
        //
        // Need the new piece to overlay.
        //
        uint32_t    newPiece  = FindUnusedPiece();
        pieces[newPiece].text    = &pieces[pieceToSplit].text[position+length];
        pieces[newPiece].length  = lengthAfter;

        //
        // Truncate the 'before' piece.
        //
        pieces[pieceToSplit].length    = lengthBefore;

        //
        // Links
        //
        uint32_t    afterPiece  = pieces[pieceToSplit].next;
        pieces[pieceToSplit].next   = newPiece;
        pieces[newPiece].next       = afterPiece;
        pieces[newPiece].last       = pieceToSplit;
        if(afterPiece != (uint32_t)-1)
        {
            pieces[afterPiece].last     = newPiece;
        }
    }
}



//
//
//
void Insert( const uint32_t pieceToSplit, const char* text, const uint32_t position, const uint32_t length )
{
    //
    // Work out the modifications needed.
    //
    uint32_t    lengthBefore        = position;
    uint32_t    lengthAfter         = pieces[pieceToSplit].length - position;

    //
    // Nothing before it, so we need to move this one up.
    //
    if(lengthBefore == 0)
    {
        //
        // Always need the new piece to overlay.
        //
        uint32_t    newPiece  = FindUnusedPiece();
        pieces[newPiece].text    = text;
        pieces[newPiece].length  = length;

        pieces[pieceToSplit].text      = &pieces[pieceToSplit].text[0];
        pieces[pieceToSplit].length    = lengthAfter;

        //
        // Links
        //
        uint32_t    previousPiece   = pieces[pieceToSplit].last;
        pieces[newPiece].next       = pieceToSplit;
        pieces[newPiece].last       = previousPiece;
        pieces[pieceToSplit].last   = newPiece;
        if(previousPiece != (uint32_t)-1 )
        {
            pieces[previousPiece].next  = newPiece;
        }
    }
    
    //
    // Nothing after it, nothing to do, its already added.
    //
    if(lengthAfter == 0)
    {
        //
        // Always need the new piece to overlay.
        //
        uint32_t    newPiece  = FindUnusedPiece();
        pieces[newPiece].text    = text;
        pieces[newPiece].length  = length;

        pieces[pieceToSplit].text      = &pieces[pieceToSplit].text[0];
        pieces[pieceToSplit].length    = lengthBefore;

        //
        // Links
        //
        uint32_t    nextPiece       = pieces[pieceToSplit].next;
        pieces[newPiece].next       = nextPiece;
        pieces[newPiece].last       = pieceToSplit;
        pieces[pieceToSplit].next   = newPiece;
        if(nextPiece != (uint32_t)-1 )
        {
            pieces[nextPiece].last  = newPiece;
        }
    }

    //
    // We have text before and after the new piece
    //
    if( (lengthAfter > 0) && (lengthBefore > 0) )
    {
        //
        // Need the new piece to overlay.
        //
        uint32_t    newPiece  = FindUnusedPiece();
        pieces[newPiece].text    = text;
        pieces[newPiece].length  = length;

        //
        // Truncate the 'before' piece.
        //
        pieces[pieceToSplit].length    = lengthBefore;

        //
        // Add a new piece with the 'after' data.
        //
        uint32_t    afterPiece  = FindUnusedPiece();
        pieces[afterPiece].text    = &pieces[pieceToSplit].text[position];
        pieces[afterPiece].length  = lengthAfter;

        //
        // Links
        //
        pieces[newPiece].next       = afterPiece;
        pieces[newPiece].last       = pieceToSplit;
        pieces[afterPiece].next     = pieces[pieceToSplit].next;
        pieces[pieceToSplit].next   = newPiece;
        pieces[afterPiece].last     = newPiece;

    }

}





//
//
//
void Replace( const char* text,  const uint32_t position, const uint32_t length )
{
    //Remove( position, length );
    //Insert( text, position, length );
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
    pieces[0].text      = p;
    pieces[0].length    = sb.st_size;
    pieces[0].next      = (uint32_t)-1;
    pieces[0].last      = (uint32_t)-1;



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



void InitialiseEditor()
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(pieces); i++)
    {
        pieces[i].next  = (uint32_t)-1;
        pieces[i].last  = (uint32_t)-1;
    }
}




//
//
//
uint32_t FindFirstPiece()
{
    uint32_t    firstPiece  = 0;

    while(pieces[firstPiece].last != (uint32_t)-1 )
    {
        firstPiece  = pieces[firstPiece].last;
    }

    return firstPiece;
}



//
//
//
void Show()
{
    uint32_t    piece       = FindFirstPiece();
    uint32_t    i           = 0;

    printf("\n");
    do 
    {
        if( piece != (uint32_t)-1 )
        {
            for(uint32_t i=0; i<pieces[piece].length; i++)
            {
                printf( "%c",pieces[piece].text[i] );
            }
        }

        piece   = pieces[piece].next;


        i++;
        if(i>=10)
        {
            exit(-1);
        }

    } while(piece != (uint32_t)-1);
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
        //
        //
        //
        InitialiseEditor();
    
        //
        //
        //
        //OpenFile( argv[1] );

        //
        //
        //
        memset( &pieces[0], 0, sizeof(pieces) );
        pieces[0].text      = "One two three four five six seven eight nine ten.";
        pieces[0].length    = strlen(pieces[0].text);
        pieces[0].next  = (uint32_t)-1;
        pieces[0].last  = (uint32_t)-1;
        Show();
        Insert( FindFirstPiece(), "[Hello World]", 10, 13 );
        Show();
        printf("\n");

        //
        //
        //
        memset( &pieces[0], 0, sizeof(pieces) );
        pieces[0].text      = "One two three four five six seven eight nine ten.";
        pieces[0].length    = strlen(pieces[0].text);
        pieces[0].next  = (uint32_t)-1;
        pieces[0].last  = (uint32_t)-1;
        Show();
        Insert( FindFirstPiece(), "[Hello World]", 0, 13 );
        Show();
        printf("\n");

        //
        //
        //
        memset( &pieces[0], 0, sizeof(pieces) );
        pieces[0].text      = "One two three four five six seven eight nine ten.";
        pieces[0].length    = strlen(pieces[0].text);
        pieces[0].next  = (uint32_t)-1;
        pieces[0].last  = (uint32_t)-1;
        Show();
        Insert( FindFirstPiece(), "[Hello World]", strlen(pieces[0].text), 13 );
        Show();
        printf("\n");

        //
        //
        //
        memset( &pieces[0], 0, sizeof(pieces) );
        pieces[0].text      = "One two three four five six seven eight nine ten.";
        pieces[0].length    = strlen(pieces[0].text);
        pieces[0].next  = (uint32_t)-1;
        pieces[0].last  = (uint32_t)-1;
        Show();
        Remove( FindFirstPiece(), 10, 13 );
        Show();
        printf("\n");

        printf("\n\n");
    }
}



