//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "dpen/DPTraceGroup.h"


namespace dpen {


DPTraceGroup::DPTraceGroup()
{
}


DPTraceGroup::~DPTraceGroup()
{
}


void DPTraceGroup::setTraces(std::vector<DPTrace>& traces)
{
    _traces = traces;
}


std::vector<DPTrace>& DPTraceGroup::getTracesRef()
{
    return _traces;
}


std::vector<DPTrace>& DPTraceGroup::getTraces()
{
    return _traces;
}


const std::vector<DPTrace>& DPTraceGroup::getTraces() const
{
    return _traces;
}


void DPTraceGroup::push_back(const DPTrace& trace)
{
    _traces.push_back(trace);
}


bool DPTraceGroup::isEmpty() const
{
    return _traces.empty();
}


void DPTraceGroup::clear()
{
    _traces.clear();
}


std::size_t DPTraceGroup::getNumTraces() const
{
    return _traces.size();
}


std::size_t DPTraceGroup::getNumPoints() const
{
    std::size_t numPoints = 0;
    for (const auto& trace: _traces)
        numPoints += trace.getNumPoints();

    return numPoints;
}


std::string DPTraceGroup::toString() const
{
    std::stringstream ss;

    for (const auto& trace: _traces)
        ss << trace.toString() << std::endl;

    return ss.str();
}


} // namespace dpen
