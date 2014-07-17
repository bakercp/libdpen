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

    for(std::size_t i = 0; i < getNumTraces(); ++i)
    {
        numPoints += _traces[i].getNumPoints();
    }
    return numPoints;
}

std::string DPTraceGroup::toString() const
{
    std::stringstream ss;

    for(std::size_t i = 0 ; i < _traces.size(); ++i)
    {
        ss << _traces[i].toString() << std::endl;
    }
    return ss.str();
}


} // namespace dpen
