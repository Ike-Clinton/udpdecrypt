#include "stdafx.h"
#include "public.h"
#include "packet.h"



using namespace std;

int readPacketsAndPrint(string inputFileName, string outputFileName)
{
	// Open the input file in binary read mode
	ifstream inputFile;
	inputFile.open(inputFileName, ios::in | ios::binary);
	// Stop eating new lines in binary mode
	inputFile.unsetf(std::ios::skipws);
	
	FILE* outputFile = NULL;

	bool printToFile = FALSE;

	// Get a file pointer to output in binary mode
	if (outputFileName != "")
	{
		fopen_s(&outputFile, outputFileName.c_str(), "wb");
		printToFile = TRUE;
	}

	// Count number of packets processed
	unsigned int packetCount = 0;
	// Count number of BE packets skipped
	// unsigned int bePacketCount = 0;


	// Struct for holding file information
	struct stat inputFileStat;

	// Check for valid file
	if (stat(inputFileName.c_str(), &inputFileStat) == 0)
		printf("Opening input.dat: %d bytes for reading:\n", inputFileStat.st_size);
	else
	{
		cout << "Error could not stat file\n";
		return 1;
	}


	// If the file exists for reading
	if (inputFile.is_open())
	{
		// Peek the next byte and make sure it isn't end of file
		while (inputFile.peek() != EOF)
		{
			// Increment our counter
			packetCount++;
			
			if (printToFile)
			{
				fprintf(outputFile, "Packet #%hu\n", packetCount);
			}
			else 
			{
				printf("Packet #%hu\n", packetCount);
			}
			// Now we just filter out the packets in wireshark...
			//// Battle Eye Client or server packet received(0X4542 = EB or BE in ASCII)
			//// Skip next 16+20=36 bytes
			//// Client -> server skip 20 bytes
			//// Server -> client skip 16 bytes
			//if (packet.length == 0x4542)
			//{
			//	bePacketCount += 2;
			//	packetCount++;
			//	if (printToFile)
			//	{
			//		fprintf(outputFile, "Found BattlEye Packet. . . Skipping next 36 bytes.\n");
			//	}
			//	else
			//	{
			//		printf("Found BattlEye Packet. . . Skipping next 36 bytes.\n");
			//	}
			//	for (int i = 0; i < 34; i++) {
			//		inputFile.get();
			//	}
			//	// Go back to reading the next packet length
			//	continue;
			//}

			struct packet packet;
			// Read the packet headers into the struct
			inputFile.read((char*)&packet.length, sizeof(packet.length));
			// Read packet data into the struct
			inputFile.read((char*)&packet.flags, sizeof(packet.flags));
			inputFile.read((char*)&packet.crc32, sizeof(packet.crc32));
			inputFile.read((char*)&packet.serial, sizeof(packet.serial));
			inputFile.read((char*)&packet.origin, sizeof(packet.origin));
			inputFile.read((char*)&packet.control1, sizeof(packet.control1));
			inputFile.read((char*)&packet.control2, sizeof(packet.control2));

			if (printToFile) 
			{
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
			}
			else
			{
				// Print headers
				printf("Packet length \n\t\t\thex: %#010X \n\t\t\tdecimal: %hu bytes\n", packet.length, packet.length);
				printf("Packet flags \t\thex: %#010X\n", packet.flags);
				printf("Packet crc32 \t\thex: %#010X\n", packet.crc32);
				printf("Packet serial \t\thex: %#010X\n", packet.serial);
				printf("Packet origin \t\thex: %#010X\n", packet.origin);
				printf("Packet control1 \thex: %#010X\n", packet.control1);
				printf("Packet control2 \thex: %#010X\n", packet.control2);
			}


			// Begin Print Data to stdout
			unsigned short dataLength = packet.length - HEADER_SIZE;

			// Print our data to stdout and write to output.dat if there is data in the packet
			if (!(dataLength <= 0))
			{
				if (printToFile)
				{
					fprintf(outputFile, "Packet data length \tdecimal: %hu bytes\n", dataLength);
				}
				else
				{
					printf("Packet data length \tdecimal: %hu bytes\n", dataLength);
				}
					


				// Allocate memory for the packet data
				packet.data = new unsigned char[dataLength];
				// Read the packet data into the struct
				for (unsigned int i = 0; i < dataLength; i++)
				{
					packet.data[i] = inputFile.get();
				}


				
				if (printToFile)
				{
					fprintf(outputFile, "Data:\n%#04X", (unsigned char)packet.data[0]);
				}
				else
				{
					printf("Data:\n%#04X", (unsigned char)packet.data[0]);
				}
					

				for (int i = 1; i < dataLength; i++)
				{
					
					if (printToFile)
					{
						fprintf(outputFile, ", %#04X", (unsigned char)packet.data[i]);
					}
					else
					{
						printf(", %#04X", (unsigned char)packet.data[i]);
					}
				}
			}
			else
			{
				if (printToFile)
				{
					fprintf(outputFile, "Packet #%hu contained no data.", packetCount);
				}
				else
				{
					printf("Packet #%hu contained no data.", packetCount);
				}
				
			}

			
			if (printToFile)
			{
				fprintf(outputFile, "\n\n");
			}
			else 
			{
				printf("\n\n");
			}
				
		}

		// Finished processing packets
		// Make sure we didn't screw something up with BE packets
		//if (!(bePacketCount % 2 == 0))
		//	printf("\n\tERROR: Something went wrong with BattlEye packet count.\n\tIt should be divisible by 2\n");

		
		if (printToFile)
		{
			fprintf(outputFile, "Read %d packets.\n", packetCount);
		}
		else
		{
			printf("Read %d packets.\n", packetCount);
		}
			

		// Close the files for r/w
		inputFile.close();
		if(printToFile)
			fclose(outputFile);
		

		//uint8_t* xorTable = new uint8_t[2048];
		//uint16_t seed = 65068;
		//printf("Creating XOR Table\n");
		//CreateXorTable(seed, xorTable);
		//for (int i = 0; i < 2048; i++)
		//{
		//	printf(" %#04X", (unsigned char)xorTable[i]);
		//}
		//// Free allocated memory
		//delete[] xorTable;

		return 0;
	}
	else
	{
		printf("\n\tERROR: Could not open files!\n");
		return 1;
	}

}

/*
From: https://stackoverflow.com/questions/5056645/sorting-stdmap-using-value
*/
template<typename A, typename B>
std::pair<B, A> flip_pair(const std::pair<A, B> &p)
{
	return std::pair<B, A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B, A> flip_map(const std::map<A, B> &src)
{
	std::multimap<B, A> dst;
	std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
		flip_pair<A, B>);
	return dst;
}

int printFlagStats(string inputFileName, string statsFileName)
{
	std::map<string, int> flagMap;
	string flagStr = "flags";
	string readStr;
	string flagVal;
	ifstream inputFile;
	
	inputFile.open(inputFileName, ios::in);

	if (!inputFile.is_open())
	{
		cout << "\nError opening file for reading";
		return 1;
	}

	FILE* statsFile = NULL;

	bool printToFile = FALSE;
	if (statsFileName != "")
	{
		fopen_s(&statsFile, statsFileName.c_str(), "wb");
		printToFile = TRUE;
	}

	while (inputFile.peek() != EOF)
	{
		
		inputFile >> readStr;
		if (readStr == flagStr)
		{
			inputFile >> readStr;
			inputFile >> flagVal;
			++flagMap[flagVal];
		}
	}
	// Sort map by value	
	std::multimap<int, string> sortedMap = flip_map(flagMap);
	for (auto iterator = sortedMap.begin(); iterator != sortedMap.end(); iterator++)
	{
		if (printToFile)
		{
			
			fprintf(statsFile, "Flag: %s - %d\n", iterator->second.c_str(), iterator->first);
		}
		else
		{
			cout << "Flag: " << iterator->second << " - " << iterator->first << "\n";
		}
		
	}

	inputFile.close();
	if(printToFile)
		fclose(statsFile);

	return 0;
}

int printSerialStats(string inputFileName, string outputFileName)
{
	return 0;
}

int printOriginStats(string inputFileName, string outputFileName)
{
	return 0;
}

int printControlStats(string inputFileName, string outputFileName)
{
	return 0;
}



