// udpdecrypt.cpp : Defines the entry point for the console application.
//

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
char decryptLine(char* buffer[]);
char readLine(char* buffer[]);

int main()
{
	// Open the input file in binary read mode
	ifstream inputFile;
	inputFile.open("input.dat", ios::in | ios::binary);
	// Stop eating new lines in binary mode!!!
	inputFile.unsetf(std::ios::skipws);
	// Get a file handle to output in binary mode
	ofstream outputFile;
	outputFile.open("output.dat", ios::out | ios::binary);

	// Count number of lines written
	int count = 0;
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
	if (inputFile.is_open() && outputFile.is_open())
	{
		// While there is more data to decrypt
		// Each line should be a string of UDP data
		while (!inputFile.eof())
		{
			cout << "Reading packet #" << count << "\n";
			count++;
			struct packet packet;
			// Read the packet headers into the struct
			inputFile.read((char*)&packet.length, sizeof(packet.length));
			inputFile.read((char*)&packet.flags, sizeof(packet.flags));
			inputFile.read((char*)&packet.crc32, sizeof(packet.crc32));
			inputFile.read((char*)&packet.serial, sizeof(packet.serial));
			inputFile.read((char*)&packet.origin, sizeof(packet.origin));
			inputFile.read((char*)&packet.control1, sizeof(packet.control1));
			inputFile.read((char*)&packet.control2, sizeof(packet.control2));

			// Print headers
			printf("Packet length \n\thex: %#010X \n\tdecimal: %d\n", packet.length, packet.length);
			printf("Packet flags \t\thex: %#010X\n", packet.flags);
			printf("Packet crc32 \t\thex: %#010X\n", packet.crc32);
			printf("Packet serial \t\thex: %#010X\n", packet.serial);
			printf("Packet origin \t\thex: %#010X\n", packet.origin);
			printf("Packet control1 \thex: %#010X\n", packet.control1);
			printf("Packet control2 \thex: %#010X\n", packet.control2);

			// Begin Print Data
			unsigned char dataLength = packet.length - HEADER_SIZE;
			printf("Packet data length \tdecimal: %d\n", dataLength, dataLength);

			char c;
			packet.data = new char[dataLength];

			// Read the packet data into the struct
			for (int i = 0; i < dataLength; i++)
			{
				packet.data[i] = inputFile.get();
			}

			cout << "Data:\n";
			for (int i = 0; i < dataLength; i++)
			{
				printf("%#04X, ", (unsigned char)packet.data[i]);
			}
			cout << endl;
			
		}

		// No more input lines to read
		cout << "Done reading/writing\n";
		printf("Read %d packets", count);
		inputFile.close();
		outputFile.close();
	}
	else
	{
		cout << "Could not open files";
	}
    return 0;
}

char decryptLine(char* buffer[])
{
	return 'a';
}

char readLine(char* buffer[])
{
	//if (buffer[0] == 'B' && buffer[1]* == 'E')
	return 'a';
}


