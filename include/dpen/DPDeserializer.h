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


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "dpen/DPELIHeader.h"
#include "dpen/DPSketch.h"
#include "dpen/DPXMLDeserializer.h"
#include "dpen/DPBlockDefinition.h"


class DPDeserializer
{
public:
    DPDeserializer();
    virtual ~DPDeserializer();

    DPError deserialize(const std::string& filename, DPSketch& _sketch);
    DPError deserializeELICompatible(const std::string& filename);
 
    DPError processHeader();
    
    bool processClockCounter();
    bool processClockInit();
    bool processClockUnknown();
    
    bool processPosition();
    bool processTilt();
    bool processPressure();
    bool processLayerStart();
    bool processTraceStart();
    bool processTraceEnd();
    
    bool processC5();
    
    bool processC7();
    bool processInterference();

protected:

    enum
    {
        DP_HEADER_START = 0,
        DP_HEADER_END = 2059
    };

    DPSketch sketch;
    DPELIHeader header;

    DPTraceGroup currentLayer;
    DPTrace currentTrace;
    DPTracePoint currentPoint;

    bool hasInterference;
    int interferenceValueMajor;
    int interferenceValueMinor;

    DPBlockDefinition::BlockCode thisBlockCode;
    DPBlockDefinition::BlockCode lastBlockCode;

    std::size_t samplesSinceTimestamp;
    unsigned long long lastTimestampMillis;

    //std::vector<DPTracePoint*> tracePointsSinceTimestamp;

    std::vector<unsigned char> buf;
    unsigned int i; // current position

};
