//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "dpen/DPTrace.h"


namespace dpen {


DPTrace::DPTrace(unsigned long long startTimeMillis): _startTimeMillis(startTimeMillis)
{
}


DPTrace::~DPTrace()
{
}


unsigned long long DPTrace::getStartTimeMillis() const
{
    return _startTimeMillis;
}


void DPTrace::setStartTimeMillis(unsigned long long startTimeMillis)
{
    _startTimeMillis = startTimeMillis;
}


void DPTrace::setContext(const std::string& context)
{
    _context = context;
}


std::string DPTrace::getContext()
{
    return _context;
}


bool DPTrace::isEmpty() const
{
    return _points.empty();
}


void DPTrace::clear()
{
    _points.clear();
}


void DPTrace::push_back(const DPTracePoint& pnt)
{
    _points.push_back(pnt);
}


std::vector<DPTracePoint>& DPTrace::getPointsRef()
{
    return _points;
}


std::size_t DPTrace::getNumPoints() const
{
    return _points.size();
}


void DPTrace::setPoints(const std::vector<DPTracePoint>& points)
{
    _points = points;
}


std::string DPTrace::toString() const
{
    std::stringstream ss;

    ss << "Trace conext=" << _context << " " << std::endl;

    for (std::size_t i = 0 ; i < _points.size(); ++i)
    {
        ss << _points[i].toString() << std::endl;
    }

    return ss.str();
}


} // namespace dpen
