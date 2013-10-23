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
#include "dpen/DPUtils.h"
#include "dpen/DPDefinitions.h"
#include "dpen/DPContext.h"
#include "dpen/DPSketch.h"
#include "dpen/DPXMLDeserializer.h"


enum DPELIBlockCode
{
    DP_BLOCK_LAYER_START = 0,
    DP_BLOCK_STROKE_START,
    DP_BLOCK_STROKE_END,
    DP_BLOCK_PEN_POSITION,
    DP_BLOCK_PEN_TILT,
    DP_BLOCK_PEN_PRESSURE,
    DP_BLOCK_CLOCK_COUNTER,
    DP_BLOCK_CLOCK_UNKNOWN,
    DP_BLOCK_CLOCK_INIT,
    DP_BLOCK_C5,
    DP_BLOCK_C7_INTERFERENCE,
    DP_BLOCK_C7_1E,
    DP_BLOCK_C7_1A,
    DP_BLOCK_C7_16,
    DP_BLOCK_C7_22,
    DP_BLOCK_UNKNOWN,
};


class DPELIBlockDef
{
public:
    DPELIBlockDef()
    {
    }

    DPELIBlockDef(unsigned char blockStart,
                  unsigned char blockLength,
                  unsigned char blockId,
                  bool blockIdMustMatch,
                  DPELIBlockCode blockCode,
                  std::string name,
                  bool debug):
        _blockStart(blockStart),
        _blockLength(blockLength),
        _blockId(blockId),
        _blockIdMustMatch(blockIdMustMatch),
        _blockCode(blockCode),
        _name(name),
        _debug(debug)
    {
    }

    unsigned char getBlockStart() const
    {
        return _blockStart;
    }

    unsigned char getBlockLength() const
    {
        return _blockLength;
    }

    unsigned char getBlockId() const
    {
        return _blockId;
    }

    bool getBlockIdMustMatch() const
    {
        return _blockIdMustMatch;
    }

    DPELIBlockCode getBlockCode() const
    {
        return _blockCode;
    }

    std::string getName() const
    {
        return _name;
    }

    bool getDebug() const
    {
        return _debug;
    }

    unsigned int getCount() const
    {
        return _count;
    }

    void incrementCount()
    {
        _count++;
    }

    void resetCount()
    {
        _count = 0;
    }

protected:
    unsigned char _blockStart;
    unsigned char _blockLength;
    unsigned char _blockId; // not always used
    bool _blockIdMustMatch;
    DPELIBlockCode _blockCode;
    std::string _name;
    bool _debug;
    unsigned int _count;

};


bool matchEventBlock(std::vector<unsigned char>& buf,
                     std::size_t i,
                     DPELIBlockCode& blockCode);

bool getELIBlockDef(const DPELIBlockCode& code, DPELIBlockDef& def);

bool resetDebugCounts();


class DPELIHeader
{
public:
    DPELIHeader()
    {
    }

    virtual ~DPELIHeader()
    {
    }

    std::string data;
};


class DPDeserializer
{
public:
    DPDeserializer()
    {
    }

    virtual ~DPDeserializer()
    {
    }

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
    bool processStrokeStart();
    bool processStrokeEnd();
    
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

    DPELIBlockCode thisBlockCode;
    DPELIBlockCode lastBlockCode;

    //    float lastTimestamp; // this is used to estimate realtime point time data.
    int numSeconds;
    std::vector<DPTracePoint*> tracePointsSinceTimestamp;

    //DPELIBlockCode lastEvent;

    std::vector<unsigned char> buf;
    unsigned int i; // current position

};
