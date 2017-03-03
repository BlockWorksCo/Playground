


#include <arm_neon.h>
#include <stdio.h>
#include <stdint.h>




int main()
{
	//uint8_t 	inputData[] 	= {0x34, 0x24,0x67,0x0ab,0x90,0x12,0x00,0x12,0xa2,0xc2,0x03,0x01};
	uint8_t 	inputData[] 	= {0x01, 0x03,0x80,0x03,0x01,0x80,0x01,0x03,0x01,0x03,0x01,0x03};

	uint8x8_t	bitMask		= {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	uint8x8_t	ones		= {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
	uint8x8_t	countThreshold	= {0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05};

	uint8x8_t 	rollingBits	= {0};

	for(uint32_t i=0; i<sizeof(inputData); i++)
	{
		uint8x8_t 	inputVector	= vld1_dup_u8(&inputData[i]);
		uint8x8_t 	selectedBits	= vand_u8(inputVector, bitMask);
		uint8x8_t	bits 		= vceq_u8(selectedBits, bitMask);
		uint8x8_t	oneFlags	= vmin_u8(ones, bits);
		rollingBits	= vshl_n_u8(rollingBits, 1);
		rollingBits 	= vorr_u8(rollingBits, oneFlags);
		uint8x8_t	bitCount	= vcnt_u8(rollingBits);
		uint8x8_t 	pastThreshold	= vcge_u8(bitCount, countThreshold);

		printf("%02x) ", inputData[i]);

		for(uint32_t i=0; i<8; i++)
		{
			printf("%02x ", pastThreshold[7-i]);
			//printf("%02x ", bitCount[7-i]);
			//printf("%02x ", rollingBits[7-i]);
		}
		printf("\n");

	}
}

