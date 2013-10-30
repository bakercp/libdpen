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


#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "dpen/DPErrors.h"
#include "dpen/DPUtils.h"


class DPBlockDefinition
{
public:
    enum BlockCode
    {
        DP_BLOCK_LAYER_START = 0,
        DP_BLOCK_TRACE_START,
        DP_BLOCK_TRACE_END,
        DP_BLOCK_PEN_POSITION,
        DP_BLOCK_PEN_TILT,
        DP_BLOCK_PEN_PRESSURE,
        DP_BLOCK_CLOCK_COUNTER,
        DP_BLOCK_CLOCK_UNKNOWN,
        DP_BLOCK_CLOCK_INIT,
        DP_BLOCK_C5, // unknown
        DP_BLOCK_C7_INTERFERENCE, // partially known
        DP_BLOCK_C7_1E, // unknown
        DP_BLOCK_C7_1A, // unknown
        DP_BLOCK_C7_16, // unknown
        DP_BLOCK_C7_22, // unknown
        DP_BLOCK_UNKNOWN, // unknown
    };

    DPBlockDefinition()
    {
    }

    DPBlockDefinition(unsigned char blockStart,
                      unsigned char blockLength,
                      unsigned char blockId,
                      bool blockIdMustMatch,
                      BlockCode blockCode,
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

    BlockCode getBlockCode() const
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


    static bool matchEventBlock(std::vector<unsigned char>& buf,
                         std::size_t i,
                         DPBlockDefinition::BlockCode& blockCode);

    static bool getELIBlockDef(const DPBlockDefinition::BlockCode& code, DPBlockDefinition& def);

    static bool hasBlockCode(const DPBlockDefinition::BlockCode& code);

    static bool resetDebugCounts();
    static bool dumpDebugCounts();

    // TODO: make this const and get rid of embedded debug counts
    static std::map<DPBlockDefinition::BlockCode,DPBlockDefinition> blockEventMap;


protected:
    unsigned char _blockStart;
    unsigned char _blockLength;
    unsigned char _blockId; // not always used
    bool _blockIdMustMatch;
    BlockCode _blockCode;
    std::string _name;
    bool _debug;
    unsigned int _count;

};
