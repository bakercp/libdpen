//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


namespace dpen {


short readShort(std::vector<unsigned char>& buf, unsigned int& i);
unsigned short readUShort(std::vector<unsigned char>& buf, unsigned int& i);
unsigned char readUChar(std::vector<unsigned char>& buf, unsigned int& i);
char readChar(std::vector<unsigned char>& buf, unsigned int& i);

bool DPReadBinaryFile(const std::string& path, std::vector<unsigned char>& buffer);
// number conversion

std::string DPStringToString(const char* str);
template <class T>
std::string DPToString(const T& value, int precision, int width, char fill )
{
	std::ostringstream out;
	out << std::fixed << std::setfill(fill) << std::setw(width) << std::setprecision(precision) << value;
	return out.str();
}

float DPStringToFloat(const std::string& floatString);
short int DPStringToShortInt(const std::string& shortString);

template <class T>
std::string DPToHex(const T& value)
{
	std::ostringstream out;

    // pretend that the value is a bunch of bytes

    unsigned char* valuePtr = (unsigned char*) &value;
	// the number of bytes is determined by the datatype
	int numBytes = sizeof(T);

	// the bytes are stored backwards (least significant first)
	for(int i = numBytes - 1; i >= 0; i--)
    {
		// print each byte out as a 2-character wide hex value
		out << std::setfill('0') << std::setw(2) << std::hex << (int) valuePtr[i];
	}

	return out.str();
}

std::string DPToHex(const char* value);

// sring operations

bool DPIsMatch(const char* name0, const char* name1);

// trim from start
std::string& DPLTrim(std::string &s);

// trim from end
std::string& DPRTrim(std::string &s);

// trim from both ends
std::string& DPTrim(std::string &s);

std::vector<std::string>& DPSplit(const std::string& s,
                                  char delim,
                                  std::vector<std::string>& elems,
                                  bool trimIt = true);

std::vector<std::string> DPSplit(const std::string &s,
                                 char delim,
                                 bool trim = true);

bool DPStringEndsWith(std::string const &fullString,
                      std::string const &ending);


} // namespace dpen
