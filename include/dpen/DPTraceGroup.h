//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <vector>
#include <iostream>
#include "dpen/DPTrace.h"


namespace dpen {


class DPTraceGroup
{
public:
    DPTraceGroup();
    virtual ~DPTraceGroup();

    void setTraces(std::vector<DPTrace>& traces);
    std::vector<DPTrace>& getTracesRef();
    void push_back(const DPTrace& trace);
    bool isEmpty() const;
    void clear();

    std::size_t getNumTraces() const;
    std::size_t getNumPoints() const;

    std::string toString() const;

protected:
    std::vector<DPTrace> _traces;

};


} // namespace dpen
