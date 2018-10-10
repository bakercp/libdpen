//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <string>


namespace dpen {


class DPELIHeader
{
public:
    DPELIHeader(): _samplingFrequency(150)
    {
    }

    virtual ~DPELIHeader()
    {
    }

    void setSamplingFrequency(unsigned long long samplingFrequency)
    {
        _samplingFrequency = samplingFrequency;
    }

    unsigned long long getSamplingFrequency(unsigned long long samplingFrequency)
    {
        return _samplingFrequency;
    }

private:
    unsigned long long _samplingFrequency;

};


} // namespace dpen
