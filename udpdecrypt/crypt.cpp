#include "stdafx.h"
#include "public.h"

// Generate the table by which we will decrypt
// Use 65068
void createXorTable(uint16_t seed, uint8_t* xorTable)
{
	long edx = 0;
	long eax;
	long ecx;
	int count = 0;

	while (edx < 0x100) {
		//printf("\nEDX: %#08X", edx);
		eax = 0;
		ecx = 8;
		eax = eax << 0x18;
		while (ecx > 0) {
			//printf("\n\tECX: %#08X", ecx);

			if (eax > 0)
			{
				eax += eax;
			}
			else
			{
				eax += eax;
				eax ^= 0x4C11Db7;
			}
			count++;
			ecx--;
		}
		xorTable[edx] = (eax >> 24) & 0xFF;
		xorTable[edx+1] = (eax >> 16) & 0xFF;
		xorTable[edx+2] = (eax >> 8) & 0xFF;
		xorTable[edx+3] = eax & 0xFF;
		edx++;
	}
}

// Two prototypes for the two different functions we see in executable
uint16_t getCryptTableOffset2(uint32_t seed)
{
	// esp + seed = seed
	// ecx = value

	//  mov     ecx, [esp + seed] ; Given by function def
	uint32_t ecx = seed;
	//	xor     ecx, 3D0000h
	ecx ^= 0x3D0000;
	//	sar     ecx, 10h
	ecx = ecx >> 16;
	ecx = ~ecx; // we do bitwise NOT since it's signed shift (sar)
				//	xor     ecx, [esp + seed]
	ecx ^= seed;
	//	lea     ecx, [ecx + ecx * 8] 
	// Clever ASM trick. See:
	// https://en.wikibooks.org/wiki/X86_Disassembly/Code_Obfuscation#Non-Intuitive_Instructions#lea
	// Common instruction substitutions
	ecx *= 9;

	//	mov     eax, ecx		
	uint32_t eax = ecx;
	//	sar     eax, 4
	eax = eax >> 4;
	eax = ~eax;
	//	xor     eax, ecx
	eax ^= ecx;
	//	imul    ecx, eax, 27D4EB2Dh
	ecx = (eax * 0x27D4EB2D);
	//	mov     eax, ecx
	eax = ecx;
	//	sar     eax, 0Fh
	eax = eax >> 15;
	eax = ~eax;
	//	xor     eax, ecx
	eax ^= ecx;
	//	and     eax, 7FFFh
	eax &= 0x7FFF;

	return (uint16_t)eax;
}

uint16_t getCryptTableOffset1(uint32_t seed)
{
	// I think this, the more complicated one is for receiving?
	return 0x0000;
}

// Takes in an xorTable, and the bytes we will decrypt
// Output is the decrypted bytes
uint8_t* decryptBytes(uint8_t* bytes, uint8_t* xorTable)
{
	return 0x00000000;
}

// Takes in an xorTable, and the bytes we will decrypt
// Output is the decrypted bytes
uint8_t* decryptBytes2(uint8_t* bytes, uint8_t* xorTable)
{
	return 0x00000000;
}