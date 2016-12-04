/*
 *Included with this repo is a .pcapng captured from wireshark which 
 * will remain the same even throughout future updates of the .exe
 * This is to establish a base line from which to proceed. If the 
 * encryption changes in future updates, we don't want to have a 
 * moving goal post.
 */

#include "stdafx.h"


// My header includes
#include "public.h"
#include "packet.h"
#include "crypt.h"
#include "packetStats.h"



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

			// Battle Eye Client or server packet received(0X4542 = EB or BE in ASCII)
			// Skip next 16+20=36 bytes
			// Client -> server skip 40 bytes
			// Server -> client skip 32 bytes
			if (packet.length == 0x4542)
			{
				bePacketCount += 2;
				packetCount++;
				fprintf(outputFile, "Found BattlEye Packet. . . Skipping next 36 bytes.\n");
				for (int i = 0; i < 34; i++) {
					inputFile.get();
				}
				// Go back to reading the next packet length
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

		// Finished processing packets
		cout << "Done reading/writing\n";
		// Make sure we didn't screw something up with BE packets
		if (!(bePacketCount % 2 == 0))
			printf("\n\tERROR: Something went wrong with BattlEye packet count.\n\tIt should be divisible by 2\n");
		printf("Read %d packets: %d game packets and %d BE packets.\n", packetCount, packetCount - bePacketCount, bePacketCount);
		fprintf(outputFile, "Read %d packets: %d from game packets and %d BE packets.\n", packetCount, packetCount - bePacketCount, bePacketCount);

		// Close the files for r/w
		inputFile.close();
		fclose(outputFile);

		uint8_t* xorTable = new uint8_t[2048];
		uint16_t seed = 65068;
		printf("Creating XOR Table\n");
		CreateXorTable(seed, xorTable);
		for (int i = 0; i < 2048; i++)
		{
			printf(" %#04X", (unsigned char)xorTable[i]);
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










