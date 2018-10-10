//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <sstream>
#include "dpen/DPTraceFormats.h"
#include "dpen/DPContext.h"


namespace dpen {


class DPDefinitions
{
public:
    DPDefinitions()
    {
    }

    virtual ~DPDefinitions()
    {
    }

    DPTraceFormats& getTraceFormatsRef()
    {
        return _traceFormats;
    }

    const DPTraceFormats getTraceFormatsRef() const
    {
        return _traceFormats;
    }

    void setTraceFormat(const DPTraceFormats& traceFormats)
    {
        _traceFormats = traceFormats;
    }

    DPContext getContextReference() const
    {
        return _contextReference;
    }

    void setContextReference(const DPContext& contextReference)
    {
        _contextReference = contextReference;
    }

    std::string toString() const
    {
        std::stringstream ss;
        ss << "[Definitions " << std::endl;
        ss << "\t" << _traceFormats.toString() << std::endl;
        ss << "\t" << _contextReference.toString() << std::endl;
        ss << "]" << std::endl;
        return ss.str();
    }

protected:
    DPTraceFormats _traceFormats;
    DPContext _contextReference;

};


} // namespace dpen
