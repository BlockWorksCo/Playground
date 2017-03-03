


#include <stdio.h>
#include <stdint.h>


typedef uint8_t     string8[8];
typedef struct
{
    uint8_t     value[8];
} struct8;


void Blaa(string8& s)
{
    printf("%ld\n", sizeof(s) );
}

int main()
{
    uint8_t     a0[8];
    string8     a1;
    struct8     a2;

    printf("%ld\n",sizeof(a0));
    printf("%ld\n",sizeof(a1));
    printf("%ld\n",sizeof(a2));
    Blaa(a1);
}

