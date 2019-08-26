//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include "dpen/DPTracePoint.h"


namespace dpen {


class DPTrace
{
public:
    DPTrace(uint64_t startTimeMillis = 0);
    virtual ~DPTrace();

    uint64_t getStartTimeMillis() const;
    void setStartTimeMillis(uint64_t startTimeMillis);

    void setContext(const std::string& context);
    std::string getContext() const;

    bool isEmpty() const;
    void clear();

    void push_back(const DPTracePoint& pnt);

    std::vector<DPTracePoint>& getPointsRef();
    std::vector<DPTracePoint>& getPoints();
    const std::vector<DPTracePoint>& getPoints() const;

    std::size_t getNumPoints() const;

    void setPoints(const std::vector<DPTracePoint>& points);

    std::string toString() const;


protected:
    uint64_t _startTimeMillis;

    std::string _context;
    std::vector<DPTracePoint> _points;

};


} // namespace dpen
