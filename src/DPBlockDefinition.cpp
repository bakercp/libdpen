//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "dpen/DPBlockDefinition.h"


namespace dpen {


std::map<DPBlockDefinition::BlockCode,DPBlockDefinition> create_map()
{
    std::map<DPBlockDefinition::BlockCode,DPBlockDefinition> m;
    m[DPBlockDefinition::BlockCode::DP_BLOCK_LAYER_START] = DPBlockDefinition(0xF1, 0x03, 0x80, true, DPBlockDefinition::BlockCode::DP_BLOCK_LAYER_START,    "Layer Start      :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_TRACE_START] = DPBlockDefinition(0xF1, 0x03, 0x01, true, DPBlockDefinition::BlockCode::DP_BLOCK_TRACE_START,     "Trace Start      :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_TRACE_END]   = DPBlockDefinition(0xF1, 0x03, 0x00, true, DPBlockDefinition::BlockCode::DP_BLOCK_TRACE_END,       "Stroke End       :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_PEN_POSITION] = DPBlockDefinition(0x61, 0x06, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_PEN_POSITION,  "Position         :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_PEN_TILT]     = DPBlockDefinition(0x65, 0x06, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_PEN_TILT,      "Tilt             :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_PEN_PRESSURE] = DPBlockDefinition(0x64, 0x06, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_PEN_PRESSURE,  "Pressure         :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_CLOCK_COUNTER] = DPBlockDefinition(0xC2, 0x06, 0x11, true, DPBlockDefinition::BlockCode::DP_BLOCK_CLOCK_COUNTER, "Clock Counter    :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_CLOCK_UNKNOWN] = DPBlockDefinition(0xC2, 0x06, 0x12, true, DPBlockDefinition::BlockCode::DP_BLOCK_CLOCK_UNKNOWN, "Clock (?)        :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_CLOCK_INIT]    = DPBlockDefinition(0xC2, 0x06, 0x00, true, DPBlockDefinition::BlockCode::DP_BLOCK_CLOCK_INIT,    "Clock Init (?)   :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_C5]            = DPBlockDefinition(0xC5, 0x13, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_C5,     "0xC5 (?)         :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_C7_INTERFERENCE] = DPBlockDefinition(0xC7, 0x0E, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_C7_INTERFERENCE, "Interference     :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_C7_1E] = DPBlockDefinition(0xC7, 0x1E, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_C7_1E,  "0xC7-0x1E (?)    :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_C7_1A] = DPBlockDefinition(0xC7, 0x1A, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_C7_1A,  "0xC7-0x1A (?)    :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_C7_16] = DPBlockDefinition(0xC7, 0x16, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_C7_16,  "0xC7-0x16 (?)    :", false);
    m[DPBlockDefinition::BlockCode::DP_BLOCK_C7_22] = DPBlockDefinition(0xC7, 0x22, 0x00, false, DPBlockDefinition::BlockCode::DP_BLOCK_C7_22,  "0xC7-0x22 (?)    :", false);
//    m[DPBlockDefinition::DP_BLOCK_C7_22] = DPBlockDefinition(0xC7, 0x22, 0x00, false, DPBlockDefinition::DP_BLOCK_0xF,    "0xF (?)          :", false);
    return m;
}


std::map<DPBlockDefinition::BlockCode, DPBlockDefinition> DPBlockDefinition::blockEventMap = create_map();


bool DPBlockDefinition::dumpDebugCounts()
{
    for (const auto& entry: blockEventMap)
    {
        std::stringstream ss;
        ss << entry.second.getName() << " " << entry.second.getCount();
        DPLogDebug(ss.str());
    }
}


bool DPBlockDefinition::resetDebugCounts()
{
    for (auto& entry: blockEventMap)
    {
        entry.second.resetCount();
    }
}

bool DPBlockDefinition::matchEventBlock(std::vector<uint8_t>& buf,
                                        std::size_t i,
                                        DPBlockDefinition::BlockCode& blockCode)
{
    std::size_t bufRemaining = buf.size() - i;
    blockCode = DPBlockDefinition::BlockCode::DP_BLOCK_UNKNOWN;

    if (bufRemaining <= 0)
    {
        DPLogError("Trying to read, but buffer empty.");
        return false;
    }

    auto iter = blockEventMap.begin();

    for (;iter != blockEventMap.end(); ++iter)
    {
        const auto& e = iter->second;

        if (buf[i] == e.getBlockStart())
        {
            if (bufRemaining > 1 && buf[i + 1] == e.getBlockLength())
            {
                if (bufRemaining >= buf[i + 1])
                {
                    if (e.getBlockIdMustMatch())
                    {
                        if (e.getBlockId() == buf[i + 2])
                        {
                            // matched the blockid
                            blockCode = e.getBlockCode();
                            return true;
                        }
                        else continue; // no match
                    }
                    else
                    {
                        // didn't need to match the block id
                        blockCode = e.getBlockCode();
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


bool DPBlockDefinition::getELIBlockDef(const DPBlockDefinition::BlockCode& code,
                                       DPBlockDefinition& def)
{
    auto iter = blockEventMap.find(code);
    if (iter != blockEventMap.end())
    {
        def = (*iter).second;
        return true;
    }

    return false;
}


} // namespace dpen
