/*
 *Included with this repo is a .pcapng captured from wireshark which 
 * will remain the same even throughout future updates of the .exe
 * This is to establish a base line from which to proceed. If the 
 * encryption changes in future updates, we don't want to have a 
 * moving goal post.
 */

#include "stdafx.h"
#include <stdint.h>

// My header includes
#include "packet.h"
#include "public.h"
#include "prototypes.h"




using namespace std;

int main()
{
	// Open the input file in binary read mode
	ifstream inputFile;
	inputFile.open("input.dat", ios::in | ios::binary);
	// Stop eating new lines in binary mode
	inputFile.unsetf(std::ios::skipws);
	// Get a file pointer to output in binary mode
	FILE* outputFile;
	fopen_s(&outputFile, "output.txt", "wb");

	// Count number of lines written
	unsigned int packetCount = 0;
	// Count number of BE packets skipped
	unsigned int bePacketCount = 0;
	// Struct for holding file information
	struct stat inputFileStat;

	// Check for valid file
	if (stat("input.dat", &inputFileStat) == 0)
		printf("Opening input.dat: %d bytes for reading:\n", inputFileStat.st_size);
	else
	{
		cout << "Error could not stat file\n";
		return 0;
	}
		

	// If the file exists for reading
	if (inputFile.is_open())
	{
		// Peek the next byte and make sure it isn't end of file
		while (inputFile.peek() != EOF)
		{
			// Increment our counter
			packetCount++;
			printf("Packet #%hu\n", packetCount);
			fprintf(outputFile, "Packet #%hu\n", packetCount);

			struct packet packet;
			// Read the packet headers into the struct
			inputFile.read((char*)&packet.length, sizeof(packet.length));

			// Battle Eye Client or server packet received(17730 = EB or BE in ASCII)
			// Skip next 16+20=36 bytes
			// Client -> server skip 16 bytes
			// Server -> client skip 20 bytes
			if (packet.length == (uint16_t)17730)
			{
				bePacketCount += 2;
				packetCount++;
				fprintf(outputFile, "Found BattlEye Packet. . . Skipping next 36 bytes.\n");
				for (int i = 0; i < 34; i++) {
					inputFile.get();
				}
				continue;
			}

			inputFile.read((char*)&packet.flags, sizeof(packet.flags));
			inputFile.read((char*)&packet.crc32, sizeof(packet.crc32));
			inputFile.read((char*)&packet.serial, sizeof(packet.serial));
			inputFile.read((char*)&packet.origin, sizeof(packet.origin));
			inputFile.read((char*)&packet.control1, sizeof(packet.control1));
			inputFile.read((char*)&packet.control2, sizeof(packet.control2));

			// Print headers
			printf("Packet length \n\t\t\thex: %#010X \n\t\t\tdecimal: %hu bytes\n", packet.length, packet.length);
			printf("Packet flags \t\thex: %#010X\n", packet.flags);
			printf("Packet crc32 \t\thex: %#010X\n", packet.crc32);
			printf("Packet serial \t\thex: %#010X\n", packet.serial);
			printf("Packet origin \t\thex: %#010X\n", packet.origin);
			printf("Packet control1 \thex: %#010X\n", packet.control1);
			printf("Packet control2 \thex: %#010X\n", packet.control2);

			// Write headers to file
			fprintf(outputFile, "Packet length \t\thex: %#010X \n\t\t\t\tdecimal: %hu bytes\n",
				packet.length, packet.length);
			//fprintf(outputFile, "Packet length: %d\n", packet.length);
			fprintf(outputFile, "Packet flags \t\thex: %#010X\n", packet.flags);
			fprintf(outputFile, "Packet crc32 \t\thex: %#010X\n", packet.crc32);
			fprintf(outputFile, "Packet serial \t\thex: %#010X\n", packet.serial);
			fprintf(outputFile, "Packet origin \t\thex: %#010X\n", packet.origin);
			fprintf(outputFile, "Packet control1 \thex: %#010X\n", packet.control1);
			fprintf(outputFile, "Packet control2 \thex: %#010X\n", packet.control2);

			// Begin Print Data to stdout
			unsigned short dataLength = packet.length - HEADER_SIZE;


			// Print our data to stdout and write to output.dat if there is data in the packet
			if (!(dataLength <= 0))
			{
				printf("Packet data length \tdecimal: %hu bytes\n", dataLength);

				// Begin Write data to file
				fprintf(outputFile, "Packet data length \tdecimal: %hu bytes\n", dataLength);

				// Allocate memory for the packet data
				packet.data = new unsigned char[dataLength];

				// Read the packet data into the struct
				for (unsigned int i = 0; i < dataLength; i++)
				{
					packet.data[i] = inputFile.get();
				}
				cout << "Data:\n";
				printf("%#04X", (unsigned char)packet.data[0]);
				fprintf(outputFile, "%#04X", (unsigned char)packet.data[0]);
				for (int i = 1; i < dataLength; i++)
				{
					printf(", %#04X", (unsigned char)packet.data[i]);
					fprintf(outputFile, ", %#04X", (unsigned char)packet.data[i]);
				}
			}
			else
			{
				printf("Packet #%hu contained no data.", packetCount);
			}
			
			cout << "\n\n";
			fprintf(outputFile, "\n\n");
		}

		// No more input lines to read
		cout << "Done reading/writing\n";
		printf("Read %d packets: %d from game packets and %d BE packets.\n", packetCount, packetCount - bePacketCount, bePacketCount);
		inputFile.close();
		fclose(outputFile);
		printf("Allocating memory on the heap\n");
		uint8_t* xorTable = new uint8_t[2048];
		uint16_t seed = 65068;
		printf("Memory allocated\nEntering CreateXorTable\n");
		CreateXorTable(seed, xorTable);
		printf("Left XOR Table, begin print\n");
			
		for (int i = 0; i < 2048; i++)
		{
			cout << (char*)xorTable;
		}
		// Free allocated memory
		delete[] xorTable;
	}
	else
	{
		cout << "Could not open files";
	}
    return 0;
}



// Generate the table by which we will decrypt
uint8_t* CreateXorTable(uint16_t seed, uint8_t* xorTable)
{
	printf("Begin generate XOR table\n");
	uint8_t value = 0x00;
	//xor     edx, edx
	uint8_t edx = 0;
	// LABEL A
	//.text : 004E50A2 loc_4E50A2 : ; CODE XREF : XX_CRC_32 + 30j
	//mov     eax, edx
	if (edx < 0x100)
	{
		uint8_t eax = edx;
		//mov     ecx, 8
		uint8_t ecx = 8;
		//shl     eax, 18h; Header decryption
		ecx = ecx << 24;
		//lea     esp, [esp + 0]
		// ???????????????
		// LABEL B
		//.text : 004E50B0 loc_4E50B0 : ; CODE XREF : XX_CRC_32 + 20j
		//test    eax, eax
		//jns     short loc_4E50BD
		int count = 0;
		for (int i = 0; i >= 0; i++)
		{
			if (eax < 0)
			{
				//add     eax, eax
				eax *= 2;
				//xor     eax, 4C11DB7h
				eax ^= 0x4C11DB7;
				//jmp     LABEL_D

			}
			else
			{
				// LABEL C
				// add     eax, eax
				eax *= 2;

			}
			// LABEL D
			// dec     ecx
			ecx--;
			// jnz     LABEL_B
		}

		// mov     dword_1005FB0[edx * 4], eax
		xorTable[count] = eax;
		count++;
		// inc     edx
		edx += 1;
	} // cmp     edx, 100h ;	jl      short loc_4E50A2
	

	
	// Global, the table has been generated
	// mov     xx_table_is_generated, 1
	// retn



	return 0x00000000;
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

	return (uint16_t) eax;
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





