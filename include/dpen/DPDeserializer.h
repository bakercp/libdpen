//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


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


namespace dpen {


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
    uint64_t lastTimestampMillis;

    //std::vector<DPTracePoint*> tracePointsSinceTimestamp;

    std::vector<uint8_t> buf;
    std::size_t i; // current position

};


} // namespace dpen
