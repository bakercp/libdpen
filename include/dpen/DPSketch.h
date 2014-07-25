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
    void setDefinitions(const DPDefinitions& definitions);
    std::vector<DPTraceGroup>& getLayersRef();
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
