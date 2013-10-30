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


#include "dpen/DPUtils.h"
#include "dpen/DPErrors.h"
#include "dpen/DPChannel.h"


class DPTraceFormats
{
public:
    
    DPTraceFormats(): _id("")
    {
    }

    DPTraceFormats(const std::string& id): _id(id)
    {
    }
    
    virtual ~DPTraceFormats()
    {
    }

    std::string getId() const
    {
        return _id;
    }
    
    void setId(const std::string& id)
    {
        _id = id;
    }
    
    std::vector<DPFloatChannel>& getChannelsRef()
    {
        return _channels;
    }
    
    void addChannel(DPFloatChannel channel)
    {
        if(!hasChannelNamed(channel.getName()))
        {
            _channels.push_back(channel);
        }
        else
        {
            DPLogError("Adding channel called " + channel.getName() + ": Already have one.");
        }
    }
    
    void setChannels(const std::vector<DPFloatChannel>& channels)
    {
        _channels = channels;
    }
    
    bool hasChannelNamed(const std::string& name) const
    {
        for(std::size_t i = 0; i < _channels.size(); ++i)
        {
            if(DPIsMatch(_channels[i].getName().c_str(), name.c_str()))
            {
                return true;
            }
        }
        return false;
    }

    DPFloatChannel* getChannelNamed(const std::string& name)
    {
        for(std::size_t i = 0; i < _channels.size(); ++i)
        {
            if(DPIsMatch(_channels[i].getName().c_str(), name.c_str()))
            {
                return &_channels[i];
            }
        }
        return 0;
    }

    
    std::string toString() const
    {
        std::stringstream ss;
        ss << "[TraceFormats id=" << _id << std::endl;
        for(std::size_t i = 0; i < _channels.size(); ++i)
        {
            ss << "\t\t\t" << _channels[i].toString() << std::endl;;
        }
        ss << "]" << std::endl;
        return ss.str();
    }
    
protected:
    std::string _id;
    std::vector<DPFloatChannel> _channels;
    
};
