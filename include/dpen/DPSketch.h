//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include <iostream>
#include <string>
#include "dpen/DPDefinitions.h"
#include "dpen/DPTraceGroup.h"


namespace dpen {


class DPSketch
{
public:
    DPSketch();
    virtual ~DPSketch();

    DPDefinitions& getDefinitionsRef();
    DPDefinitions& getDefinitions();
    const DPDefinitions& getDefinitions() const;
    void setDefinitions(const DPDefinitions& definitions);
    std::vector<DPTraceGroup>& getLayersRef();
    std::vector<DPTraceGroup>& getLayers();
    const std::vector<DPTraceGroup>& getLayers() const;
    void push_back(const DPTraceGroup& traceGroup);
    void setLayers(const std::vector<DPTraceGroup>& layers);
    std::size_t getNumLayers() const;
    std::size_t getNumTraces() const;
    std::size_t getNumPoints() const;
    std::string getSketchStats() const;
    bool hasPosition() const;
    bool hasX() const;
    bool hasY() const;
    bool hasTilt() const;
    bool hasTiltX() const;
    bool hasTiltY() const;
    bool hasPressure() const;
    bool hasTimestamp() const;
    void addPositionChannel();
    void addTiltChannel();
    bool addPressureChannel();
    void addTimestampChannel();

    std::string toString() const;
    bool isEmpty() const;

protected:
    DPDefinitions _definitions;
    std::vector<DPTraceGroup> _layers;

};


} // namespace dpen
