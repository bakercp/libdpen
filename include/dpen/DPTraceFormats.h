//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "dpen/DPUtils.h"
#include "dpen/DPErrors.h"
#include "dpen/DPChannel.h"


namespace dpen {


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

    std::vector<DPFloatChannel>& getChannels()
    {
        return _channels;
    }

    const std::vector<DPFloatChannel>& getChannels() const
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
            if (DPIsMatch(_channels[i].getName().c_str(), name.c_str()))
            {
                return true;
            }
        }
        return false;
    }

    DPFloatChannel* getChannelNamed(const std::string& name)
    {
        for (std::size_t i = 0; i < _channels.size(); ++i)
        {
            if (DPIsMatch(_channels[i].getName().c_str(), name.c_str()))
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
        for (std::size_t i = 0; i < _channels.size(); ++i)
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


} // namespace dpen
