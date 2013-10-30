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


#include "dpen/DPBlockDefinition.h"



std::map<DPBlockDefinition::BlockCode,DPBlockDefinition> create_map()
{
    std::map<DPBlockDefinition::BlockCode,DPBlockDefinition> m;
    m[DPBlockDefinition::DP_BLOCK_LAYER_START]  = DPBlockDefinition(0xF1, 0x03, 0x80, true, DPBlockDefinition::DP_BLOCK_LAYER_START,    "Layer Start      :", false);
    m[DPBlockDefinition::DP_BLOCK_TRACE_START] = DPBlockDefinition(0xF1, 0x03, 0x01, true, DPBlockDefinition::DP_BLOCK_TRACE_START,   "Trace Start     :", false);
    m[DPBlockDefinition::DP_BLOCK_TRACE_END]   = DPBlockDefinition(0xF1, 0x03, 0x00, true, DPBlockDefinition::DP_BLOCK_TRACE_END,     "Stroke End       :", false);

    m[DPBlockDefinition::DP_BLOCK_PEN_POSITION] = DPBlockDefinition(0x61, 0x06, 0x00, false, DPBlockDefinition::DP_BLOCK_PEN_POSITION,  "Position         :", false);
    m[DPBlockDefinition::DP_BLOCK_PEN_TILT]     = DPBlockDefinition(0x65, 0x06, 0x00, false, DPBlockDefinition::DP_BLOCK_PEN_TILT,      "Tilt             :", false);
    m[DPBlockDefinition::DP_BLOCK_PEN_PRESSURE] = DPBlockDefinition(0x64, 0x06, 0x00, false, DPBlockDefinition::DP_BLOCK_PEN_PRESSURE,  "Pressure         :", false);

    m[DPBlockDefinition::DP_BLOCK_CLOCK_COUNTER] = DPBlockDefinition(0xC2, 0x06, 0x11, true, DPBlockDefinition::DP_BLOCK_CLOCK_COUNTER, "Clock Counter    :", false);
    m[DPBlockDefinition::DP_BLOCK_CLOCK_UNKNOWN] = DPBlockDefinition(0xC2, 0x06, 0x12, true, DPBlockDefinition::DP_BLOCK_CLOCK_UNKNOWN, "Clock (?)        :", false);
    m[DPBlockDefinition::DP_BLOCK_CLOCK_INIT]    = DPBlockDefinition(0xC2, 0x06, 0x00, true, DPBlockDefinition::DP_BLOCK_CLOCK_INIT,    "Clock Init (?)   :", false);

    m[DPBlockDefinition::DP_BLOCK_C5]    = DPBlockDefinition(0xC5, 0x13, 0x00, false, DPBlockDefinition::DP_BLOCK_C5,     "0xC5 (?)         :", false);

    m[DPBlockDefinition::DP_BLOCK_C7_INTERFERENCE] = DPBlockDefinition(0xC7, 0x0E, 0x00, false, DPBlockDefinition::DP_BLOCK_C7_INTERFERENCE,
                                                                       "Interference     :", false);
    m[DPBlockDefinition::DP_BLOCK_C7_1E] = DPBlockDefinition(0xC7, 0x1E, 0x00, false, DPBlockDefinition::DP_BLOCK_C7_1E,  "0xC7-0x1E (?)    :", true);
    m[DPBlockDefinition::DP_BLOCK_C7_1A] = DPBlockDefinition(0xC7, 0x1A, 0x00, false, DPBlockDefinition::DP_BLOCK_C7_1A,  "0xC7-0x1A (?)    :", false);
    m[DPBlockDefinition::DP_BLOCK_C7_16] = DPBlockDefinition(0xC7, 0x16, 0x00, false, DPBlockDefinition::DP_BLOCK_C7_16,  "0xC7-0x16 (?)    :", false);
    m[DPBlockDefinition::DP_BLOCK_C7_22] = DPBlockDefinition(0xC7, 0x22, 0x00, false, DPBlockDefinition::DP_BLOCK_C7_22,  "0xC7-0x22 (?)    :", false);
    return m;
}


std::map<DPBlockDefinition::BlockCode,DPBlockDefinition> DPBlockDefinition::blockEventMap = create_map();


bool DPBlockDefinition::dumpDebugCounts()
{
    std::map<DPBlockDefinition::BlockCode,DPBlockDefinition>::const_iterator iter = DPBlockDefinition::blockEventMap.begin();

    std::cout << "--------------DEBUG COUNTS--------------" << std::endl;
    while(iter != blockEventMap.end())
    {
        std::cout << (*iter).second.getName() << " " << (*iter).second.getCount() << std::endl;
        ++iter;
    }
}

bool DPBlockDefinition::resetDebugCounts()
{
    std::map<DPBlockDefinition::BlockCode,DPBlockDefinition>::iterator iter = DPBlockDefinition::blockEventMap.begin();

    while(iter != blockEventMap.end())
    {
        (*iter).second.resetCount();
        ++iter;
    }
}

bool DPBlockDefinition::matchEventBlock(std::vector<unsigned char>& buf,
                                        std::size_t i,
                                        DPBlockDefinition::BlockCode& blockCode)
{
    std::size_t bufRemaining = buf.size() - i;
    blockCode = DPBlockDefinition::DP_BLOCK_UNKNOWN;

    if(bufRemaining <= 0)
    {
        DPLogError("Trying to read, but buffer empty.");
        return false;
    }

    std::map<DPBlockDefinition::BlockCode,DPBlockDefinition>::const_iterator iter = blockEventMap.begin();

    for (;iter != blockEventMap.end();++iter)
    {
        if (buf[i] == (*iter).second.getBlockStart())
        {
            if (bufRemaining > 1 && buf[i+1] == (*iter).second.getBlockLength())
            {
                if (bufRemaining >= buf[i+1])
                {
                    if ((*iter).second.getBlockIdMustMatch())
                    {
                        if ((*iter).second.getBlockId() == buf[i+2])
                        {
                            // matched the blockid
                            blockCode = (*iter).second.getBlockCode();
                            return true;
                        }
                        else
                        {
                            continue; // no match
                        }
                    }
                    else
                    {
                        // didn't need to match the block id
                        blockCode = (*iter).second.getBlockCode();
                        return true;
                    }
                }
                else
                {
                    DPLogError("Trying to read, but there are not enough bytes left in the buffer to read this entire block.");
                    return false;
                }
            }
        }
    }
    return true;
}

bool DPBlockDefinition::hasBlockCode(const DPBlockDefinition::BlockCode& code)
{
    return blockEventMap.find(code) != DPBlockDefinition::blockEventMap.end();
}

bool DPBlockDefinition::getELIBlockDef(const DPBlockDefinition::BlockCode& code, DPBlockDefinition& def)
{
    std::map<DPBlockDefinition::BlockCode,DPBlockDefinition>::iterator iter = blockEventMap.find(code);

    if(iter != blockEventMap.end())
    {
        def = (*iter).second;
        return true;
    } else {
        return false;
    }
}


