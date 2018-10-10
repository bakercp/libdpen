//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <string>
#include <iostream>
#include "dpen/DPUtils.h"



namespace dpen {


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


inline std::string DPErrorToString(const DPError& error)
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
    std::cerr << "Error: " << error << std::endl;
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


} // namespace dpen
