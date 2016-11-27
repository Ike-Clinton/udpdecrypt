// udpdecrypt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Function Prototypes
string decryptLine(string line);

int main()
{
	// Handles to the files for r/w
	ifstream inputFile("input.txt");
	ofstream outputFile("output.txt");

	// Temp variable to store each line for decryption
	string line = "";

	// Count number of lines written
	int count = 0;

	// If the file exists for reading
	if (inputFile.is_open() && outputFile.is_open())
	{
		// While there is more data to decrypt
		// Each line should be a string of UDP data
		while (getline(inputFile, line))
		{
			outputFile << decryptLine(line) << " " << count+1 << "\n";
			count++;
		}

		// No more input lines to read
		cout << "Done reading/writing\n";
		inputFile.close();
		outputFile.close();
	}
	else
	{
		cout << "Could not open files";
	}
    return 0;
}

string decryptLine(string line) 
{
	return "test";
}

