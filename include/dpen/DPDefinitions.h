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


#include <sstream>
#include "dpen/DPTraceFormats.h"
#include "dpen/DPContext.h"


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
