//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "dpen/DPUtils.h"
#include <algorithm>
#include <cstring>


namespace dpen {


short readShort(std::vector<unsigned char>& buf, unsigned int& i)
{
    short out = 0;
    out  = buf[i++] << 8;
    out |= buf[i++];
    return out;
}


unsigned short readUShort(std::vector<unsigned char>& buf, unsigned int& i)
{
    unsigned short out = 0;
    out  = buf[i++] << 8;
    out |= buf[i++];
    return out;
}


unsigned char readUChar(std::vector<unsigned char>& buf, unsigned int& i)
{
    return buf[i++];
}


char readChar(std::vector<unsigned char>& buf, unsigned int& i)
{
    return buf[i++];
}


bool DPReadBinaryFile(const std::string& path, std::vector<unsigned char>& buffer)
{
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        std::ifstream::pos_type size = file.tellg(); // how long is the file?
        buffer.resize(size); // resize buffer to fit
        file.seekg(0, std::ios::beg); // back to beginning b/c the file was opened w/ ios::ate
        file.read((char*)&buffer[0], size); // read int size # bytes
        file.close(); // close the file
        return true;
    }
    else
    {
        return false;
    }
}

// number conversion

std::string DPStringToString(const char* str)
{
    std::string x = 0;
    std::istringstream cur(str);
    cur >> x;
    return x;
}

//template <class T>
//string DPToString(const T& value, int precision, int width, char fill ){
//	ostringstream out;
//	out << fixed << setfill(fill) << setw(width) << setprecision(precision) << value;
//	return out.str();
//}

float DPStringToFloat(const std::string& floatString)
{
    float x = 0;
    std::istringstream cur(floatString);
    cur >> x;
    return x;
}

short int DPStringToShortInt(const std::string& shortString)
{
    short int x = 0;
    std::istringstream cur(shortString);
    cur >> x;
    return x;
}

//template <class T>
//string DPToHex(const T& value) {
//	ostringstream out;
//	// pretend that the value is a bunch of bytes
//	unsigned char* valuePtr = (unsigned char*) &value;
//	// the number of bytes is determined by the datatype
//	int numBytes = sizeof(T);
//	// the bytes are stored backwards (least significant first)
//	for(int i = numBytes - 1; i >= 0; i--) {
//		// print each byte out as a 2-character wide hex value
//		out << setfill('0') << setw(2) << hex << (int) valuePtr[i];
//	}
//	return out.str();
//}
//


template <>
inline std::string DPToHex(const std::string& value)
{
	std::ostringstream out;
	// how many bytes are in the string
    std::size_t numBytes = value.size();
	for (std::size_t i = 0; i < numBytes; i++)
    {
		// print each byte as a 2-character wide hex value
		out << std::setfill('0') << std::setw(2) << std::hex << (unsigned int) value[i];
	}
	return out.str();
}

std::string DPToHex(const char* value)
{
	// this function is necessary if you want to print a string
	// using a syntax like ofToHex("test")
	return DPToHex((std::string) value);
}


bool DPIsMatch(const char* name0, const char* name1)
{
    return strcmp(name0, name1) == 0;
}


std::string& DPLTrim(std::string& s)
{
    s.erase(s.begin(),
            std::find_if(s.begin(),
                         s.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}


std::string& DPRTrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(),
                         s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
            s.end());
    return s;
}


std::string& DPTrim(std::string &s)
{
    return DPLTrim(DPRTrim(s));
}


std::vector<std::string>& DPSplit(const std::string& s,
                                  char delim,
                                  std::vector<std::string>& elems,
                                  bool trimIt)
{
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim))
    {
        elems.push_back(trimIt ? DPTrim(item) : item);
    }
    return elems;
}


std::vector<std::string> DPSplit(const std::string &s,
                                 char delim,
                                 bool trim)
{
    std::vector<std::string> elems;
    return DPSplit(s, delim, elems);
}


bool DPStringEndsWith(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        // ignore case
        std::string full = fullString;
        std::string end = ending;
        std::transform(full.begin(), full.end(), full.begin(), ::tolower);
        std::transform(end.begin(), end.end(), end.begin(), ::tolower);

        return (0 == full.compare(full.length() - end.length(),
                                  end.length(),
                                  end));
    }
    else
    {
        return false;
    }
}


} // namespace dpen
