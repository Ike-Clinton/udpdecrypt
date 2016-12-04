#include "stdafx.h"
#include "public.h"

// Generate the table by which we will decrypt
void CreateXorTable(uint16_t seed, uint8_t* xorTable)
{
	//XX_CRC_32       proc near; CODE XREF : xx_call_crc + 12p
	//	; XX_decrypt + 1Bp
	//	xor     edx, edx
	long edx = 0;

	//	xx_crc_main_loop : ; CODE XREF : XX_CRC_32 + 30j
	//	mov     eax, edx
	long eax = 0;
	//	mov     ecx, 8
	long ecx = 8;
	//	shl     eax, 18h; Header decryption
	eax = eax << 0x18;
	//	lea     esp, [esp + 0]
	// Save stack state??
	while (edx < 0x100) { //	cmp     edx, 100h
		printf("EDX: %lx\n", edx);
		while (ecx >= 0) { 	//	jnz     short xx_crc_inner_loop
			printf("\n\tECX: %lx", ecx);
			//printf("%\n" PRIu32, ecx);
			//	xx_crc_inner_loop:; CODE XREF : XX_CRC_32 + 20j
			//	test    eax, eax
			//	jns     short xx_lbl_A
			if (eax >= 0) //xx_lbl_A:; CODE XREF : XX_CRC_32 + 12j
			{
				//	add     eax, eax
				eax += eax;
			}
			else
			{
				//	add     eax, eax
				eax += eax;
				//	xor     eax, 4C11DB7h
				eax ^= 0x4C11Db7;
				//	jmp     short xx_lbl_B
			}
			//	xx_lbl_B : ; CODE XREF : XX_CRC_32 + 1Bj
			//	dec     ecx
			ecx--;
		}
		//	mov     xx_packet_xor_table[edx * 4], eax
		xorTable[edx] = eax;
		//	inc     edx
		edx++;

	}	//	cmp     edx, 100h ; jl      short xx_crc_main_loop

		//	mov     xx_table_is_generated, 1
		// sentinel_value = 1;
		//	retn
}

// Two prototypes for the two different functions we see in executable
uint16_t GetCryptTableOffset2(uint32_t seed)
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

uint16_t GetCryptTableOffset1(uint32_t seed)
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