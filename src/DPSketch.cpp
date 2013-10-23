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


#include "dpen/DPSketch.h"


DPSketch::DPSketch()
{
}

DPSketch::~DPSketch()
{
}

DPDefinitions& DPSketch::getDefinitionsRef()
{
    return _definitions;
}

void DPSketch::setDefinitions(const DPDefinitions& definitions)
{
    _definitions = definitions;
}

std::vector<DPTraceGroup>& DPSketch::getLayersRef()
{
    return _layers;
}

void DPSketch::push_back(const DPTraceGroup& traceGroup)
{
    _layers.push_back(traceGroup);
}

void DPSketch::setLayers(const std::vector<DPTraceGroup>& layers)
{
    _layers = layers;
}

std::string DPSketch::toString()
{
    std::stringstream ss;
    ss << "[DPSKETCH" << std::endl;
    ss << _definitions.toString() << std::endl;
    for(std::size_t i = 0; i < _layers.size(); ++i)
    {
        ss << _layers[i].toString() << std::endl;
    }
    ss << "]";

    return ss.str();
}

bool DPSketch::isEmpty() const
{
    return _layers.empty();
}

std::size_t DPSketch::getNumLayers() const
{
    return _layers.size();
}

std::size_t DPSketch::getNumTraces() const
{
    std::size_t numTraces = 0;
    for(std::size_t i = 0; i < _layers.size(); ++i)
    {
        numTraces += _layers[i].getNumTraces();
    }
    return numTraces;
}

std::size_t DPSketch::getNumPoints() const
{
    std::size_t numPoints = 0;
    for(std::size_t i = 0; i < getNumLayers(); ++i)
    {
        numPoints += _layers[i].getNumPoints();
    }
    return numPoints;
}

std::string DPSketch::getSketchStats() const
{
    std::stringstream ss;
    ss << "[DPSketch:";
    ss << " #layers: " << getNumLayers();
    ss << " #traces: " << getNumTraces();
    ss << " #points: " << getNumPoints();
    ss << "]";
    ss << std::endl;
    return ss.str();
}

DPTracePoint* DPSketch::getFirstPoint()
{
    if(_layers.empty())
    {
        return 0;
    }
    else
    {
        return _layers[0].getFirstPoint();
    }
}

DPTracePoint* DPSketch::getLastPoint()
{
    if(_layers.empty())
    {
        return 0;
    }
    else
    {
        return _layers[_layers.size()-1].getLastPoint();
    }
}

bool DPSketch::hasPosition() const
{
    return hasX() && hasY();
}

bool DPSketch::hasX() const
{
    return _definitions.getTraceFormatsRef().hasChannelNamed("x");
}

bool DPSketch::hasY() const
{
    return _definitions.getTraceFormatsRef().hasChannelNamed("y");
}

bool DPSketch::hasTilt() const
{
    return hasTiltX() && hasTiltY();
}

bool DPSketch::hasTiltX() const
{
    return _definitions.getTraceFormatsRef().hasChannelNamed("xtilt");
}

bool DPSketch::hasTiltY() const
{
    return _definitions.getTraceFormatsRef().hasChannelNamed("xtilt");
}

bool DPSketch::hasPressure() const
{
    return _definitions.getTraceFormatsRef().hasChannelNamed("pressure");
}

bool DPSketch::hasTimestamp() const
{
    return _definitions.getTraceFormatsRef().hasChannelNamed("timestamp");
}

void DPSketch::addPositionChannel()
{
    if(hasPosition())
    {
        return;
    }

    DPFloatChannel xChan;
    xChan.setName("x");
    xChan.setUnits("0.1mm");
    xChan.setMin(-3000);
    xChan.setMax(3000);
    _definitions.getTraceFormatsRef().addChannel(xChan);

    DPFloatChannel yChan;
    yChan.setName("y");
    yChan.setUnits("0.1mm");
    yChan.setMin(0);
    yChan.setMax(6000);
    _definitions.getTraceFormatsRef().addChannel(yChan);

}

void DPSketch::addTiltChannel()
{
    if(hasTilt())
    {
        return;
    }

    DPFloatChannel xChan;
    xChan.setName("xtilt");
    xChan.setUnits("none");
    xChan.setMin(-127);
    xChan.setMax(127);
    _definitions.getTraceFormatsRef().addChannel(xChan);

    DPFloatChannel yChan;
    yChan.setName("ytilt");
    yChan.setUnits("none");
    yChan.setMin(-127);
    yChan.setMax(128);
    _definitions.getTraceFormatsRef().addChannel(yChan);
}

bool DPSketch::addPressureChannel()
{
    if(hasPressure())
    {
        return;
    }

    DPFloatChannel pressureChan;
    pressureChan.setName("pressure");
    pressureChan.setUnits("none");
    pressureChan.setMin(0);
    pressureChan.setMax(1024);
    _definitions.getTraceFormatsRef().addChannel(pressureChan);
}

void DPSketch::addTimestampChannel()
{
    if(hasTimestamp())
    {
        return;
    }
    DPFloatChannel timestampChan;
    timestampChan.setName("timestamp");
    timestampChan.setUnits("0.1sec");
    timestampChan.setMin(0);
    timestampChan.setMax(128);
    _definitions.getTraceFormatsRef().addChannel(timestampChan);
}
