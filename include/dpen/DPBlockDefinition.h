//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "dpen/DPErrors.h"
#include "dpen/DPUtils.h"


namespace dpen {


class DPBlockDefinition
{
public:
    enum class BlockCode
    {
        DP_BLOCK_LAYER_START,
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
        DP_BLOCK_UNKNOWN // unknown
    };

    static std::string toString(const BlockCode& code)
    {
        switch (code)
        {
            case BlockCode::DP_BLOCK_LAYER_START:
                return "DP_BLOCK_LAYER_START";
            case BlockCode::DP_BLOCK_TRACE_START:
                return "DP_BLOCK_TRACE_START";
            case BlockCode::DP_BLOCK_TRACE_END:
                return "DP_BLOCK_TRACE_END";
            case BlockCode::DP_BLOCK_PEN_POSITION:
                return "DP_BLOCK_PEN_POSITION";
            case BlockCode::DP_BLOCK_PEN_TILT:
                return "DP_BLOCK_PEN_TILT";
            case BlockCode::DP_BLOCK_PEN_PRESSURE:
                return "DP_BLOCK_PEN_PRESSURE";
            case BlockCode::DP_BLOCK_CLOCK_COUNTER:
                return "DP_BLOCK_CLOCK_COUNTER";
            case BlockCode::DP_BLOCK_CLOCK_UNKNOWN:
                return "DP_BLOCK_CLOCK_UNKNOWN";
            case BlockCode::DP_BLOCK_CLOCK_INIT:
                return "DP_BLOCK_CLOCK_INIT";
            case BlockCode::DP_BLOCK_C5: // unknown
                return "DP_BLOCK_C5";
            case BlockCode::DP_BLOCK_C7_INTERFERENCE: // partially known
                return "DP_BLOCK_C7_INTERFERENCE";
            case BlockCode::DP_BLOCK_C7_1E: // unknown
                return "DP_BLOCK_C7_1E";
            case BlockCode::DP_BLOCK_C7_1A: // unknown
                return "DP_BLOCK_C7_1A";
            case BlockCode::DP_BLOCK_C7_16: // unknown
                return "DP_BLOCK_C7_16";
            case BlockCode::DP_BLOCK_C7_22: // unknown
                return "DP_BLOCK_C7_22";
            case BlockCode::DP_BLOCK_UNKNOWN: // unknown
                return "DP_BLOCK_UNKNOWN";
        }

        return "ERROR: UNKNOWN BLOCK";
    }


    DPBlockDefinition()
    {
    }

    DPBlockDefinition(uint8_t blockStart,
                      uint8_t blockLength,
                      uint8_t blockId,
                      bool blockIdMustMatch,
                      BlockCode blockCode,
                      const std::string& name,
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

    uint8_t getBlockStart() const
    {
        return _blockStart;
    }

    uint8_t getBlockLength() const
    {
        return _blockLength;
    }

    uint8_t getBlockId() const
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

    std::size_t getCount() const
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

    void resetCountSinceTimestamp()
    {
        _countSinceTimestamp = 0;
    }

    void incrementCountSinceTimestamp()
    {
        _countSinceTimestamp++;
    }

    std::size_t getCountSinceTimestamp() const
    {
        return _countSinceTimestamp;
    }


    static bool matchEventBlock(std::vector<uint8_t>& buf,
                                std::size_t i,
                                DPBlockDefinition::BlockCode& blockCode);

    static bool getELIBlockDef(const DPBlockDefinition::BlockCode& code, DPBlockDefinition& def);

    static bool hasBlockCode(const DPBlockDefinition::BlockCode& code);

    static bool resetDebugCounts();
    static bool dumpDebugCounts();

    // TODO: make this const and get rid of embedded debug counts
    static std::map<DPBlockDefinition::BlockCode, DPBlockDefinition> blockEventMap;


protected:
    uint8_t _blockStart = 0;
    uint8_t _blockLength = 0;
    uint8_t _blockId = 0; // not always used
    bool _blockIdMustMatch = false;
    BlockCode _blockCode = DPBlockDefinition::BlockCode::DP_BLOCK_UNKNOWN;
    std::string _name;
    bool _debug = false;
    std::size_t _count = 0;

    std::size_t _countSinceTimestamp = 0;

};


} // namespace dpen
