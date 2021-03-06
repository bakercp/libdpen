//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "dpen/DPSketch.h"


namespace dpen {


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


DPDefinitions& DPSketch::getDefinitions()
{
    return _definitions;
}


const DPDefinitions& DPSketch::getDefinitions() const
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


std::vector<DPTraceGroup>& DPSketch::getLayers()
{
    return _layers;
}


const std::vector<DPTraceGroup>& DPSketch::getLayers() const
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
    for (const auto& l: _layers)
        numTraces += l.getNumTraces();
    return numTraces;
}


std::size_t DPSketch::getNumPoints() const
{
    std::size_t numPoints = 0;
    for (const auto& l: _layers)
        numPoints += l.getNumPoints();
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
    if (hasPosition())
        return;

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
    if (hasTilt())
        return;

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
    if (hasPressure())
    {
        return true;
    }

    DPFloatChannel pressureChan;
    pressureChan.setName("pressure");
    pressureChan.setUnits("none");
    pressureChan.setMin(0);
    pressureChan.setMax(1024);
    _definitions.getTraceFormatsRef().addChannel(pressureChan);

    return true;
}


void DPSketch::addTimestampChannel()
{
    if (hasTimestamp())
    {
        return;
    }

    DPFloatChannel timestampChan;
    timestampChan.setName("timestamp");
    timestampChan.setUnits("0.0067sec");
    timestampChan.setMin(0);
    timestampChan.setMax(128);
    _definitions.getTraceFormatsRef().addChannel(timestampChan);
}


std::string DPSketch::toString() const
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


} // namespace dpen
