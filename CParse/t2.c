

#include <stdint.h>


int 	i = 0;
int     j = 1;


void BlaaOne()
{
	i 	= 23;
}

void BlaaTwo()
{
	i 	= 23;
	BlaaOne();
}

void main()
{
 	uint8_t i=0;

	while(true)
	{
		BlaaTwo();
	}
}


