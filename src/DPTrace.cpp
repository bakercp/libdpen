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
