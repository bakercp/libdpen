//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <sstream>
#include <string>


namespace dpen {


template <typename T> 
class DPChannel
{
public:
    DPChannel();
    virtual ~DPChannel();

    std::string getName() const;
    void setName(const std::string& name);

    std::string getUnits() const;
    void setUnits(const std::string& units);
    
    T getMin() const;
    void setMin(const T& min);

    T getMax() const;
    void setMax(const T& max);

    std::string toString() const;
    
protected:
    std::string _name;
    std::string _units;

    T _min;
    T _max;

};


template <class T>
DPChannel<T>::DPChannel()
{
}


template <class T>
DPChannel<T>::~DPChannel()
{
}


template <class T>
std::string DPChannel<T>::getName() const
{
    return _name;
}


template <class T>
void DPChannel<T>::setName(const std::string& name)
{
    _name = name;
}


template <class T> std::string DPChannel<T>::getUnits() const
{
    return _units;
}


template <class T>
void DPChannel<T>::setUnits(const std::string& units)
{
    _units = units;
}


template <class T>
T DPChannel<T>::getMin() const
{
    return _min;
}


template <class T>
void DPChannel<T>::setMin(const T& min)
{
    _min = min;
}


template <class T>
T DPChannel<T>::getMax() const
{
    return _max;
}


template <class T>
void DPChannel<T>::setMax(const T& max)
{
    _max = max;
}


template <class T>
std::string DPChannel<T>::toString() const
{
    std::stringstream ss;
    ss << "[Channel name=" << _name;
    ss << " min=" << _min;
    ss << " max=" << _max;
    ss << " units=" << _units;
    ss << "]";
    return ss.str();
}


// some defs
typedef DPChannel<char>             DPSCharChannel;
typedef DPChannel<unsigned char>    DPUCharChannel;
typedef DPChannel<short>            DPSShortChannel;
typedef DPChannel<unsigned short>   DPUShortChannel;
typedef DPChannel<int>              DPSIntChannel;
typedef DPChannel<unsigned int>     DPUIntChannel;
typedef DPChannel<float>            DPFloatChannel;
typedef DPChannel<double>           DPDoubleChannel;


} // namespace dpen
