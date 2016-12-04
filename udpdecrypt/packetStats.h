#include "public.h"
#pragma once

int readPacketsAndPrint(std::string inputFileName, std::string outputFileName = "");

int printFlagStats(std::string inputFileName, std::string outputFileName = "");

int printSerialStats(std::string inputFileName, std::string outputFileName = "");

int printOriginStats(std::string inputFileName, std::string outputFileName = "");

int printControlStats(std::string inputFileName, std::string outputFileName = "");