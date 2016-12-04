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





int main()
{
	std::string inputFileName = "input.dat";
	std::string outputFileName = "output.txt";

	// Just read packets and print to console
	// readPacketsAndPrint(inputFileName);

	// Read packets and write to file
	if (readPacketsAndPrint(inputFileName, outputFileName) == 0)
	{
		printf("Packets read and processed successfully.\n");
		printf("\nThe output file is visible in %s\n\n", outputFileName.c_str());
	}
	else
	{
		printf("Error while processing packets");
	}
		
	
    return 0;
}










