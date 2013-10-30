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


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "tinyxml.h"
#include "dpen/DPErrors.h"
#include "dpen/DPUtils.h"
#include "dpen/DPDefinitions.h"
#include "dpen/DPContext.h"
#include "dpen/DPSketch.h"


class DPXMLDeserializer
{
public:
    static DPError deserializeWACCompatible(const std::string& filename,
                                            DPSketch& sketch)
    {
        TiXmlDocument doc(filename);

        if (doc.LoadFile())
        {
            TiXmlElement* root = doc.FirstChildElement(); 
            if(!isValidXMLNodeWithName(root, "ink"))
            {
                return DP_ERROR_INVALID_XML;
            }

            for(TiXmlElement* elem = root->FirstChildElement(); elem != 0; elem = elem->NextSiblingElement())
            {
                if(!isValidNode(elem))
                {
                    DPLogWarning("invalid node found");
                    continue;
                }
                else if(DPIsMatch(elem->Value(),"definitions"))
                {
                    DPDefinitions defs;
                    DPError err = parseXMLDefinitions(elem, defs);
                    if(err)
                    {
                        DPLogError("Error in deserializeWACCompatible() while parseXMLDefinitions()" + DPErrorToString(err));
                        return err;
                    }
                    sketch.setDefinitions(defs);
                }
                else if(DPIsMatch(elem->Value(),"tracegroup"))
                {
                    DPTraceGroup tg;
                    DPError err = parseXMLTraceGroup(elem, tg);
                    if(err)
                    {
                        DPLogError("Error in deserializeWACCompatible() while parseXMLDefinitions()" + DPErrorToString(err));
                        return err;
                    }
                    sketch.getLayersRef().push_back(tg);
                }
                else
                {
                    DPLogWarning("found an unknown element " + DPStringToString(elem->Value()));
                }
            }
            return DP_SUCCESS;
        }
        else
        {
            return DP_ERROR_UNABLE_TO_LOAD_FROM_DISK;
        }
    }
    
    static DPError parseXMLTraceFormats(TiXmlElement* node, DPTraceFormats& tf)
    {
        tf.setId(getStringAttr(node, "id")); // set the id

        for(TiXmlElement* elem = node->FirstChildElement();
            0 != elem; elem = elem->NextSiblingElement())
        {
            if(!isValidNode(elem))
            {
                DPLogWarning("invalid node found");
                continue;
            }
            else if(DPIsMatch(elem->Value(),"channel"))
            {
                DPFloatChannel channel;
                channel.setName(getStringAttr(elem, "name"));
                channel.setMin(getFloatAttr(elem, "min"));
                channel.setMax(getFloatAttr(elem, "max"));
                channel.setUnits(getStringAttr(elem, "units"));
                tf.getChannelsRef().push_back(channel);
            }
            else
            {
                DPLogWarning("found an unknown element " + DPStringToString(elem->Value()));
            }
        }
        return DP_SUCCESS;
    }

    static DPError parseXMLTraceGroup(TiXmlElement* node, DPTraceGroup& defs)
    {
        for(TiXmlElement* elem = node->FirstChildElement();
            0 != elem;
            elem = elem->NextSiblingElement())
        {
            if(!isValidNode(elem))
            {
                DPLogWarning("invalid node found");
                continue;
            }
            else if(DPIsMatch(elem->Value(),"trace"))
            {
                // TODO: estimate trace start time based on number of samples before
                DPTrace trace;
                trace.setContext(getStringAttr(elem, "contextRef"));
                
                // get the texst
                TiXmlNode* child = elem->FirstChild();
                if(child != NULL && child->Type() == TiXmlNode::TEXT)
                {
                    std::vector<std::string> tokens;
                    DPSplit(child->ToText()->Value(), ',', tokens);
                    for(std::size_t i = 0; i < tokens.size(); ++i)
                    {
                        std::vector<std::string> valueTokens;
                        DPSplit(tokens[i], ' ', valueTokens);
                        
                        // TODO: this should match up w/ channels
                        if(valueTokens.size() == 7)
                        {
                            float x         = DPStringToFloat(valueTokens[0]);
                            float y         = DPStringToFloat(valueTokens[1]);
                            float pressure  = DPStringToFloat(valueTokens[3]);
                            float tiltX     = DPStringToFloat(valueTokens[4]);
                            float tiltY     = DPStringToFloat(valueTokens[5]);
                            float timestamp = DPStringToFloat(valueTokens[6]);
                            
                            DPTracePoint point(x,y,tiltX,tiltY,pressure,timestamp);
                            
                            // add trace
                            trace.getPointsRef().push_back(point);
                            
                        }
                        else
                        {
                            DPLogWarning("skipping unknown # of channels.");
                        }
                    }
                }
                else
                {
                    DPLogWarning("found an empty trace " + DPStringToString(elem->Value()));
                }
                
                if(trace.getPointsRef().size() > 0)
                {
                    defs.getTracesRef().push_back(trace); // add the trace
                }
                else
                {
                    DPLogWarning("empty trace found, skipping.");
                }
            }
            else
            {
                DPLogWarning("found an unknown element " + DPStringToString(elem->Value()));
            }
        }

        return DP_SUCCESS;
    }
    
    static DPError parseXMLDefinitions(TiXmlElement* node, DPDefinitions& defs)
    {
        bool isTraceFormatSet = false;
        bool isContextSet = false;
        
        for(TiXmlElement* elem = node->FirstChildElement();
            0 != elem;
            elem = elem->NextSiblingElement())
        {
            if(!isValidNode(elem))
            {
                DPLogWarning("invalid node found");
                continue;
            }
            else if(DPIsMatch(elem->Value(),"traceFormat"))
            {
                DPTraceFormats tf;
                DPError err = parseXMLTraceFormats(elem,tf);
                if(err)
                {
                    DPLogError("Error in parseXMLDefinitions() while parseXMLTraceFormats()" +
                               DPErrorToString(err));
                    return err;
                }

                if(isTraceFormatSet)
                {
                    DPLogWarning("found multiple trace formats");
                }

                isTraceFormatSet = true;
                defs.setTraceFormat(tf);
            }
            else if(DPIsMatch(elem->Value(), "context"))
            {
                DPContext context;
                context.setId(getStringAttr(elem, "id"));
                context.setTraceFormatRef(getStringAttr(elem, "traceFormatRef"));
                if(isContextSet)
                {
                    DPLogWarning("found multiple contexts.");
                }
                isContextSet = true;
                defs.setContextReference(context);
            }
            else
            {
                DPLogWarning("found an unknown element " + DPStringToString(elem->Value()));
            }
        }
        return DP_SUCCESS;
    }
    
protected:
    static bool isValidXMLNodeWithName(TiXmlElement* element, const char* name)
    {
        return isValidNode(element) && DPIsMatch(element->Value(), name);
    }
    
    static bool isValidNode(TiXmlElement* element)
    {
        return 0 != element;
    }

    static std::string getContent(TiXmlElement* elem,
                                  std::string defaultValue = "")
    {
        TiXmlText* text = elem->ToText();
        return 0 == text? defaultValue : text->Value();
    }
    
    static std::string getStringAttr(TiXmlElement* elem,
                                     const std::string& name,
                                     std::string defaultValue = "")
    {
        const char* tag = elem->Attribute(name.c_str());
        return 0 != tag ? tag : defaultValue;
    }
    
    static float getFloatAttr(TiXmlElement* elem,
                              const std::string& name,
                              float defaultValue = 0)
    {
        const char* tag = elem->Attribute(name.c_str());
        return 0 != tag ? DPStringToFloat(tag) : defaultValue;
    }
    
    static short int getShortAttr(TiXmlElement* elem,
                                  const std::string& name,
                                  short int defaultValue = 0)
    {
        const char* tag = elem->Attribute(name.c_str());
        return 0 != tag ? DPStringToShortInt(tag) : defaultValue;
    }

};
