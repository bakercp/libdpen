//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <sstream>


namespace dpen {


class DPContext
{
public:
    DPContext(): _id(""), _traceFormatReference("")
    {
    }

    DPContext(const std::string& id,
              const std::string& traceFormatReference):
        _id(id),
        _traceFormatReference(traceFormatReference)
    {
    }

    virtual ~DPContext()
    {
    }

    std::string getTraceFormatReference() const
    {
        return _traceFormatReference;
    }

    void setTraceFormatRef(const std::string& traceFormatReference)
    {
        _traceFormatReference = traceFormatReference;
    }

    std::string getId() const
    {
        return _id;
    }

    void setId(const std::string& id)
    {
        _id = id;
    }

    std::string toString() const
    {
        std::stringstream ss;
        ss << "[Context id=" << _id << " traceFormatRef=" << _traceFormatReference << "]";
        return ss.str();
    }

protected:
    std::string _traceFormatReference;
    std::string _id;

};


} // namespace dpen
