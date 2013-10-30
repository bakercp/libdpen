// =============================================================================
//
// Copyright (c) 2012-2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <string>
#include <iostream>
#include "dpen/DPUtils.h"


static bool LOG_DEBUG = true;
static bool HEX_DEBUG = true;


enum DPError
{
    DP_SUCCESS = 0,
    DP_ERROR_UNKNOWN_FILE_ENDING,
    DP_ERROR_UNABLE_TO_LOAD_FROM_DISK,
    DP_ERROR_INVALID_XML,
    DP_ERROR_INVALID_HEADER,
};


inline std::string DPErrorToString(DPError& error)
{
    switch (error)
    {
        case DP_SUCCESS:
            return "SUCCESS";
        case DP_ERROR_UNKNOWN_FILE_ENDING:
            return "UNKNOWN FILE ENDING";
        case DP_ERROR_UNABLE_TO_LOAD_FROM_DISK:
            return "UNABLE TO LOAD FILE FROM DISK";
        case DP_ERROR_INVALID_XML:
            return "INVALID XML";
        case DP_ERROR_INVALID_HEADER:
            return "INVALID HEADER";
        default:
            return "UNKNOWN ERROR CODE";
    }
}

inline void DPLogBlock(const std::string& prefix,
                       std::vector<unsigned char>& buf,
                       unsigned int i)
{
    if(LOG_DEBUG)
    {
        unsigned char length = buf[i+1];

        std::cout << prefix << " ";

        for (unsigned char j = 0; j < length; ++j)
        {
            if (HEX_DEBUG)
            {
                std::cout << "0x" <<  DPToHex(buf[i+j]) << " ";
            }
            else
            {
                std::cout << DPToString( (unsigned int) buf[i+j], 0, 4, ' ');
            }
        }

        std::cout << std::endl;
    }
}

// logging utilities
inline void DPLogError(const std::string& error)
{
    std::cout << "Error: " << error << std::endl;
}

inline void DPLogWarning(const std::string& warning)
{
    std::cout << "Warning: " << warning << std::endl;
}

inline void DPLogDebug(const std::string& verbose)
{
    if(LOG_DEBUG)
    {
        std::cout << "Debug: " << verbose << std::endl;
    }
}
