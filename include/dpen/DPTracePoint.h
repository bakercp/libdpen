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
#include <iostream>


namespace dpen {


class DPTracePoint
{
public:
    DPTracePoint():
        _x(0),
        _y(0),
        _tiltX(0),
        _tiltY(0),
        _pressure(0),
        _timestamp(0),
        _xSet(false),
        _ySet(false),
        _tiltXSet(false),
        _tiltYSet(false),
        _pressureSet(false),
        _timestampSet(false)
    {
    }

    DPTracePoint(float x,
                 float y,
                 float tiltX,
                 float tiltY,
                 float pressure,
                 float timestamp):
        _x(x),
        _y(y),
        _tiltX(tiltX),
        _tiltY(tiltY),
        _pressure(pressure),
        _timestamp(timestamp),
        _xSet(true),
        _ySet(true),
        _tiltXSet(true),
        _tiltYSet(true),
        _pressureSet(true),
        _timestampSet(true)
    {
    }

    virtual ~DPTracePoint()
    {
    }
    
    float getX() const
    {
        return _x;
    }

    void setX(float x)
    {
        _x = x;
        _xSet = true;
    }
    
    float getY() const
    {
        return _y;
    }

    void setY(float y)
    {
        _y = y;
        _ySet = true;
    }

    float getTiltX() const
    {
        return _tiltX;
    }

    void setTiltX(float tiltX)
    {
        _tiltX = tiltX;
        _tiltXSet = true;
    }

    float getTiltY() const
    {
        return _tiltY;
    }

    void setTiltY(float tiltY)
    {
        _tiltY = tiltY; _tiltYSet = true;
    }

    float getPressure() const
    {
        return _pressure;
    }

    void setPressure(float pressure)
    {
        _pressure = pressure;
        _tiltXSet = true;
    }

    float getTimestamp() const
    {
        return _timestamp;
    }

    void setTimestamp(float timestamp)
    {
        _timestamp = timestamp;
        _timestampSet = true;
    }
 
    std::string toString() const
    {
        std::stringstream ss;
        ss << "\t[DPTracePoint :";
        ss << " pos=(" << _x << "/" << _y << ")\t";
        ss << " tilt=(" << _tiltX << "/" << _tiltY << ")\t";
        ss << " pres=(" << _pressure << ")\t";
        ss << " ts=(" << _timestamp << ")";
        ss << "]";
        return ss.str();
    }

    bool isPositionSet() const
    {
        return _xSet && _ySet;
    }

    bool isTiltSet() const
    {
        return _tiltXSet && _tiltYSet;
    }

    bool isPressureSet() const
    {
        return _pressureSet;
    }

    bool isTimestampSet() const
    {
        return _timestampSet;
    }

    void clear()
    {
        _x = 0;
        _y = 0;
        _tiltX = 0;
        _tiltY = 0;
        _pressure = 0;
        _timestamp = 0;
        
        // sets
        _xSet         = false;
        _ySet         = false;
        _tiltXSet     = false;
        _tiltYSet     = false;
        _pressureSet  = false;
        _timestampSet = false;
    }
    
protected:
    bool _xSet;
    bool _ySet;
    bool _tiltXSet;
    bool _tiltYSet;
    bool _pressureSet;
    bool _timestampSet;
    
    float _x;
    float _y;
    
    float _tiltX;
    float _tiltY;

    float _pressure;

    float _timestamp;
    
};


} // namespace dpen
