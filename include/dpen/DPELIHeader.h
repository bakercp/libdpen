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

    void setSamplingFrequency(uint64_t samplingFrequency)
    {
        _samplingFrequency = samplingFrequency;
    }

    uint64_t getSamplingFrequency(uint64_t samplingFrequency)
    {
        return _samplingFrequency;
    }

private:
    uint64_t _samplingFrequency;

};


} // namespace dpen
