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


class DPTrace
{
public:
    DPTrace()
    {
    }

    virtual ~DPTrace()
    {
    }

    void setContext(const std::string& context)
    {
        _context = context;
    }

    std::string getContext()
    {
        return _context;
    }

    bool isEmpty() const
    {
        return _points.empty();
    }
    
    void clear()
    {
        _points.clear();
    }

    void push_back(const DPTracePoint& pnt)
    {
        _points.push_back(pnt);
    }
    
    std::vector<DPTracePoint>& getPointsRef()
    {
        return _points;
    }

    std::size_t getNumPoints() const
    {
        return _points.size();
    }

    DPTracePoint* getFirstPoint()
    {
        if(_points.empty())
        {
            return 0;
        }
        else
        {
            return &_points[0];
        }
    }

    DPTracePoint* getLastPoint()
    {
        if(_points.empty())
        {
            return 0;
        }
        else
        {
            return &_points[_points.size()-1];
        }
    }
    
    void setPoints(const std::vector<DPTracePoint>& points)
    {
        _points = points;
    }

    std::string toString() const
    {
        std::stringstream ss;
        ss << "Trace conext=" << _context << " " << std::endl;
        for(std::size_t i = 0 ; i < _points.size(); ++i)
        {
            ss << _points[i].toString() << std::endl;
        }
        return ss.str();
    }
    
protected:
    std::string _context;
    std::vector<DPTracePoint> _points;

};