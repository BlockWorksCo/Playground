

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>


#define NUMBER_OF_ELEMENTS(a)       (sizeof(a)/sizeof(a[0]))

#define MAX_PIECES      (1024)


//
//
//
typedef struct
{
    const char* text;
    uint32_t    position;
    uint32_t    length;

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
uint32_t FindPieceContainingPosition( const uint32_t position )
{
    for(uint32_t i=0; i<NUMBER_OF_ELEMENTS(pieces); i++)
    {
        uint32_t    start   = pieces[i].position;
        uint32_t    end     = start + pieces[i].length;

        if( (position >= start) && (position < end) )
        {
            return i;
        }
    }

    return -1;
}


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
void Remove( const uint32_t position, const uint32_t length )
{
    uint32_t    pieceToSplit    = FindPieceContainingPosition( position );

    //
    // Work out the modifications needed.
    //
    uint32_t    lengthBefore        = position - pieces[pieceToSplit].position;
    uint32_t    lengthAfter         = (pieces[pieceToSplit].position + pieces[pieceToSplit].length) - (position + length);

    //
    // Nothing before it, so we need to move this one up.
    //
    if(lengthBefore == 0)
    {
        pieces[pieceToSplit].text      = &pieces[pieceToSplit].text[length];
        pieces[pieceToSplit].length    = lengthAfter;
        pieces[pieceToSplit].position  += length;
    }
    
    //
    // Nothing after it, so just truncate this one.
    //
    if(lengthAfter == 0)
    {
        pieces[pieceToSplit].length    = lengthBefore;
    }

    //
    // We have text before and after the new piece
    //
    if( (lengthAfter > 0) && (lengthBefore > 0) )
    {
        //
        // Truncate the 'before' piece.
        //
        pieces[pieceToSplit].length    = lengthBefore;

        //
        // Add a new piece with the 'after' data.
        //
        uint32_t    newPiece  = FindUnusedPiece();
        pieces[newPiece].text    = &pieces[pieceToSplit].text[position - pieces[pieceToSplit].position + length];
        pieces[newPiece].length  = lengthAfter;
        pieces[newPiece].position= position+length;
    }
}



//
//
//
void Insert( const char* text, const uint32_t position, const uint32_t length )
{
    uint32_t    pieceToSplit    = FindPieceContainingPosition( position );
    uint32_t    pieceToOverlay  = FindUnusedPiece();

    //
    // Always need the new piece to overlay.
    //
    pieces[pieceToOverlay].text    = text;
    pieces[pieceToOverlay].length  = length;
    pieces[pieceToOverlay].position= position;

    //
    // Work out the modifications needed.
    //
    uint32_t    lengthBefore        = position - pieces[pieceToSplit].position;
    uint32_t    lengthAfter         = (pieces[pieceToSplit].position + pieces[pieceToSplit].length) - position;

    //
    // Nothing before it, so we need to move this one up.
    //
    if(lengthBefore == 0)
    {
        pieces[pieceToSplit].text      = &pieces[pieceToSplit].text[length];
        pieces[pieceToSplit].length    = lengthAfter;
        pieces[pieceToSplit].position  += length;
    }
    
    //
    // Nothing after it, so just truncate this one.
    //
    if(lengthAfter == 0)
    {
        pieces[pieceToSplit].length    = lengthBefore;
    }

    //
    // We have text before and after the new piece
    //
    if( (lengthAfter > 0) && (lengthBefore > 0) )
    {
        //
        // Truncate the 'before' piece.
        //
        pieces[pieceToSplit].length    = lengthBefore;

        //
        // Add a new piece with the 'after' data.
        //
        uint32_t    newPiece  = FindUnusedPiece();
        pieces[newPiece].text    = &pieces[pieceToSplit].text[position - pieces[pieceToSplit].position];
        pieces[newPiece].length  = lengthAfter;
        pieces[newPiece].position= position+length;
    }

}




//
//
//
void Replace( const char* text,  const uint32_t position, const uint32_t length )
{
    Remove( position, length );
    Insert( text, position, length );
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
    pieces[0].position  = 0;



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
void Show()
{
    uint32_t    position    = 0;
    uint32_t    piece       = (uint32_t)-1;

    printf("\n");
    do 
    {
        piece = FindPieceContainingPosition( position );
        if( piece != (uint32_t)-1 )
        {
            for(uint32_t i=0; i<pieces[piece].length; i++)
            {
                printf( "%c",pieces[piece].text[i] );
            }

            position    += pieces[piece].length;
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
        //OpenFile( argv[1] );

        //
        //
        //
        pieces[0].text      = "One two three four five six seven eight nine ten.";
        pieces[0].length    = strlen(pieces[0].text);
        pieces[0].position  = 0;
        Show();

        //
        //
        //
        Insert( "[Hello World]", 10, 13 );
        Show();

        //
        //
        //
        //Replace( "[Hello World]", 10, 20 );
        //Show();

        printf("\n\n");
    }
}



