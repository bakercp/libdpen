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
#include "dpen/DPTracePoint.h"


namespace dpen {


class DPTrace
{
public:
    DPTrace(unsigned long long startTimeMillis = 0);
    virtual ~DPTrace();

    unsigned long long getStartTimeMillis() const;
    void setStartTimeMillis(unsigned long long startTimeMillis);

    void setContext(const std::string& context);
    std::string getContext();

    bool isEmpty() const;
    void clear();

    void push_back(const DPTracePoint& pnt);
    
    std::vector<DPTracePoint>& getPointsRef();

    std::size_t getNumPoints() const;

    void setPoints(const std::vector<DPTracePoint>& points);

    std::string toString() const;


protected:
    unsigned long long _startTimeMillis;

    std::string _context;
    std::vector<DPTracePoint> _points;

};


} // namespace dpen
