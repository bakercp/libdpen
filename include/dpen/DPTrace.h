//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


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
    std::string getContext() const;

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
