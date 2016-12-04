#include "public.h"
#pragma once

int readPacketsAndPrint(std::string inputFileName, std::string outputFileName = "");

void printFlagStats();
void fprintFlagStats(FILE* outputFile);

void printSerialStats();
void fprintSerialStats(FILE* outputFile);

void printOriginStats();
void fprintOriginStats(FILE* outputFile);

void printControlStats();
void fprintControlStats(FILE* outputFile);