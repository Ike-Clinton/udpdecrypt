#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

// My header includes
#include "packet.h"

using namespace std;

// Function Prototypes
char* decryptPacket(char* buffer);
char* readPacket(char* buffer);

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
				fprintf(outputFile, "Packet data length \tdecimal: %hu bytes\n", dataLength, dataLength);

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
	}
	else
	{
		cout << "Could not open files";
	}
    return 0;
}



char* decryptPacket(char* buffer)
{
	return 0x00000000;
}

char* readPacket(char* buffer)
{
	//if (buffer[0] == 'B' && buffer[1]* == 'E')
	return 0x00000000;
}


