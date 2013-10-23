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


#include "dpen/DPDeserializer.h"
#include "dpen/DPUtils.h"


std::map<DPELIBlockCode,DPELIBlockDef> create_map()
{
    std::map<DPELIBlockCode,DPELIBlockDef> m;
    m[DP_BLOCK_LAYER_START]  = DPELIBlockDef(0xF1, 0x03, 0x80, true, DP_BLOCK_LAYER_START,    "Layer Start      :", false);
    m[DP_BLOCK_STROKE_START] = DPELIBlockDef(0xF1, 0x03, 0x01, true, DP_BLOCK_STROKE_START,   "Stroke Start     :", false);
    m[DP_BLOCK_STROKE_END]   = DPELIBlockDef(0xF1, 0x03, 0x00, true, DP_BLOCK_STROKE_END,     "Stroke End       :", false);
    
    m[DP_BLOCK_PEN_POSITION] = DPELIBlockDef(0x61, 0x06, 0x00, false, DP_BLOCK_PEN_POSITION,  "Position         :", false);
    m[DP_BLOCK_PEN_TILT]     = DPELIBlockDef(0x65, 0x06, 0x00, false, DP_BLOCK_PEN_TILT,      "Tilt             :", false);
    m[DP_BLOCK_PEN_PRESSURE] = DPELIBlockDef(0x64, 0x06, 0x00, false, DP_BLOCK_PEN_PRESSURE,  "Pressure         :", false);
    
    m[DP_BLOCK_CLOCK_COUNTER] = DPELIBlockDef(0xC2, 0x06, 0x11, true, DP_BLOCK_CLOCK_COUNTER, "Clock Counter    :", false);
    m[DP_BLOCK_CLOCK_UNKNOWN] = DPELIBlockDef(0xC2, 0x06, 0x12, true, DP_BLOCK_CLOCK_UNKNOWN, "Clock (?)        :", false);
    m[DP_BLOCK_CLOCK_INIT]    = DPELIBlockDef(0xC2, 0x06, 0x00, true, DP_BLOCK_CLOCK_INIT,    "Clock Init (?)   :", false);
    
    m[DP_BLOCK_C5]    = DPELIBlockDef(0xC5, 0x13, 0x00, false, DP_BLOCK_C5,     "0xC5 (?)         :", false);
    
    m[DP_BLOCK_C7_INTERFERENCE] = DPELIBlockDef(0xC7, 0x0E, 0x00, false, DP_BLOCK_C7_INTERFERENCE,
                                                                                              "Interference     :", false);
    m[DP_BLOCK_C7_1E] = DPELIBlockDef(0xC7, 0x1E, 0x00, false, DP_BLOCK_C7_1E,  "0xC7-0x1E (?)    :", true);
    m[DP_BLOCK_C7_1A] = DPELIBlockDef(0xC7, 0x1A, 0x00, false, DP_BLOCK_C7_1A,  "0xC7-0x1A (?)    :", false);
    m[DP_BLOCK_C7_16] = DPELIBlockDef(0xC7, 0x16, 0x00, false, DP_BLOCK_C7_16,  "0xC7-0x16 (?)    :", false);
    m[DP_BLOCK_C7_22] = DPELIBlockDef(0xC7, 0x22, 0x00, false, DP_BLOCK_C7_22,  "0xC7-0x22 (?)    :", false);
    return m;
}


std::map<DPELIBlockCode,DPELIBlockDef> blockEventMap = create_map();


bool dumpDebugCounts()
{
    std::map<DPELIBlockCode,DPELIBlockDef>::iterator iter = blockEventMap.begin();

    std::cout << "--------------DEBUG COUNTS--------------" << std::endl;
    while(iter != blockEventMap.end())
    {
        std::cout << (*iter).second.getName() << " " << (*iter).second.getCount() << std::endl;
        ++iter;
    }
}

bool resetDebugCounts()
{
    std::map<DPELIBlockCode,DPELIBlockDef>::iterator iter = blockEventMap.begin();

    while(iter != blockEventMap.end())
    {
        (*iter).second.resetCount();
        ++iter;
    }
}

bool matchEventBlock(std::vector<unsigned char>& buf,
                     std::size_t i,
                     DPELIBlockCode& blockCode)
{
    std::size_t bufRemaining = buf.size() - i;
    blockCode = DP_BLOCK_UNKNOWN;

    if(bufRemaining <= 0)
    {
        DPLogError("Trying to read, but buffer empty.");
        return false;
    }
    
    std::map<DPELIBlockCode,DPELIBlockDef>::iterator iter = blockEventMap.begin();
    
    for (;iter != blockEventMap.end();++iter)
    {
        if (buf[i] == (*iter).second.getBlockStart())
        {
            if (bufRemaining > 1 && buf[i+1] == (*iter).second.getBlockLength())
            {
                if (bufRemaining >= buf[i+1])
                {
                    if ((*iter).second.getBlockIdMustMatch())
                    {
                        if ((*iter).second.getBlockId() == buf[i+2])
                        {
                            // matched the blockid
                            blockCode = (*iter).second.getBlockCode();
                            return true;
                        }
                        else
                        {
                            continue; // no match
                        }
                    }
                    else
                    {
                        // didn't need to match the block id
                        blockCode = (*iter).second.getBlockCode();
                        return true;
                    }
                }
                else
                {
                    DPLogError("Trying to read, but there are not enough bytes left in the buffer to read this entire block.");
                    return false;
                }
            }
        }
    }
    return true;
}

bool hasBlockCode(const DPELIBlockCode& code)
{
    return blockEventMap.find(code) != blockEventMap.end();
}

bool getELIBlockDef(const DPELIBlockCode& code, DPELIBlockDef& def)
{
    std::map<DPELIBlockCode,DPELIBlockDef>::iterator iter = blockEventMap.find(code);
    
    if(iter != blockEventMap.end())
    {
        def = (*iter).second;
        return true;
    } else {
        return false;
    }
}

DPError DPDeserializer::deserialize(const std::string& filename, DPSketch& _sketch)
{
    if(DPStringEndsWith(filename, ".wac"))
    {
        return DPXMLDeserializer::deserializeWACCompatible(filename, _sketch);
    }
    else if(DPStringEndsWith(filename, ".eli") || DPStringEndsWith(filename, ".wpi"))
    {
        DPError err = deserializeELICompatible(filename);
        _sketch = sketch;
        return err;
    }
    else
    {
        return DP_ERROR_UNKNOWN_FILE_ENDING;
    }
}

DPError DPDeserializer::processHeader()
{
    // TODO.  get something out of here.
    
    //    bool loadFileHB(string file) {
    //        cout << file << endl;
    //        vector<unsigned char> b;
    //        if(!readBinaryFile(file, b) && b.size() > 0) {
    //            return false;
    //        }
    //
    //                 short is =  (b[2044] << 8 ) | (b[2043]);
    //        unsigned short iu =  (b[2044] << 8 ) | (b[2043]);
    //
    //        cout << ofToString((unsigned int) b[2043],0,4,' ') << "," << ofToString((unsigned int) b[2044],0,4,' ') << "  " << is << " / " << iu << endl;
    //
    //
    //    }
    
    return DP_SUCCESS;
}

bool DPDeserializer::processClockCounter()
{
    i+=4;
    numSeconds = readUShort(buf,i);
    //samplesSinceTimestamp = 0;
    return true;
}

bool DPDeserializer::processClockInit()
{
//    if(blockEventMap[DP_BLOCK_CLOCK_INIT].cnt > 1) {
//        DPLogError(DPToString(blockEventMap[DP_BLOCK_CLOCK_INIT].cnt, 0, 3, ' ') + " Clock Inits Found");
//    }
    i+=4; //
    unsigned short v = readUShort(buf, i);
//    if(v != 1) DPLogDebug("Clock Init : " + DPToString(v, 0, 4, ' '));
    return true;
}

bool DPDeserializer::processClockUnknown()
{
//    if(blockEventMap[DP_BLOCK_CLOCK_UNKNOWN].cnt > 1) {
//        DPLogError(DPToString(blockEventMap[DP_BLOCK_CLOCK_UNKNOWN].cnt, 0, 3, ' ') + " Clock Unknown Found");
//    }
    i+=4;
    unsigned short v = readUShort(buf, i);
    if(v != 1) DPLogDebug("Clock Unknown : " + DPToString(v, 0, 4, ' '));
    return true;
}

bool DPDeserializer::processPosition()
{
    if(sketch.hasPosition())
    {
        if(currentPoint.isPositionSet())
        {
            // position was already set, so we must be on a new set
            currentTrace.push_back(currentPoint);
            currentPoint.clear();
            //samplesSinceTimestamp++;
        }
    }
    else
    {
        sketch.addPositionChannel();
    }
    
    // read it;
    i+=2; // skip markers
    currentPoint.setX(readShort(buf, i) / 10.0f); // this scaling factor is what inkling uses for WAC
    currentPoint.setY(readShort(buf, i) /  5.0f); // this scaling factor is what inkling uses for WAC
    return true;
}

bool DPDeserializer::processTilt()
{
    if(sketch.hasTilt())
    {
        if(currentPoint.isTiltSet())
        {
            // position was already set, so we must be on a new set
            currentTrace.push_back(currentPoint);
            currentPoint.clear();
            //samplesSinceTimestamp++;
        }
    }
    else
    {
        sketch.addTiltChannel();
    }
    
    i+=2; // skip markers
    currentPoint.setTiltX(readChar(buf, i));
    currentPoint.setTiltY(readChar(buf, i));
    i+=2; // skip empty bytes after data

    return true;
}

bool DPDeserializer::processPressure()
{
    if(sketch.hasPressure())
    {
        if(currentPoint.isPressureSet()) {
            // position was already set, so we must be on a new set
            currentTrace.push_back(currentPoint);
            currentPoint.clear();
            //samplesSinceTimestamp++;
        }
    }
    else
    {
        sketch.addPressureChannel();
    }
    
    i+=2; // skip markers
    i+=2; // skip empty bytes before data
    currentPoint.setPressure(readUShort(buf, i));

    return true;
}

bool DPDeserializer::processLayerStart()
{
    i += blockEventMap[DP_BLOCK_LAYER_START].getBlockLength();
    if(!currentLayer.isEmpty()) sketch.push_back(currentLayer);
    currentLayer.clear();
    return true;
}

bool DPDeserializer::processStrokeStart()
{
    i += blockEventMap[DP_BLOCK_STROKE_START].getBlockLength();
    if(!currentTrace.isEmpty()) currentLayer.push_back(currentTrace);
    currentTrace.clear();
    return true;
}

bool DPDeserializer::processStrokeEnd()
{
    i += blockEventMap[DP_BLOCK_STROKE_END].getBlockLength();
    if(!currentTrace.isEmpty()) currentLayer.push_back(currentTrace);
    currentTrace.clear();
    return true;
}

bool DPDeserializer::processC5()
{
    // we don't yet know what this is
    unsigned char lengthOfPacket = buf[i+1];
    i += lengthOfPacket; // consume the second section
    return true;
}

bool DPDeserializer::processC7()
{
    // we don't yet know what this is
    unsigned char lengthOfPacket = buf[i+1];
    i += lengthOfPacket; // consume the second section
    return true;
}

bool DPDeserializer::processInterference()
{
    i+=2; // markers
    if(readChar(buf, i) != 4) DPLogWarning("Var on interference 0");
    if(readChar(buf, i) != 4) DPLogWarning("Var on interference 1");
    if(readChar(buf, i) != 0) DPLogWarning("Var on interference 2");
    if(readChar(buf, i) != 0) DPLogWarning("Var on interference 3");
    if(readChar(buf, i) != 3) DPLogWarning("Var on interference 4");
    if(readChar(buf, i) != 0) DPLogWarning("Var on interference 5");
    if(readChar(buf, i) != 0) DPLogWarning("Var on interference 6");
    if(readChar(buf, i) != 0) DPLogWarning("Var on interference 7");
    
    int _interferenceValueMinor = readUChar(buf, i);
    int _interferenceValueMajor = readUChar(buf, i);
    
    if(_interferenceValueMinor != (interferenceValueMinor + 2))
    {
    //    DPLogWarning("XXX - Interference Val Minor Dev");
    }
    if(_interferenceValueMajor > (interferenceValueMajor + 1))
    {
    //    DPLogWarning("XXX - Interference Val Major Dev");
    }
    
    interferenceValueMinor = _interferenceValueMinor;
    interferenceValueMajor = _interferenceValueMajor;
    
    if(0 != readChar(buf, i))
    {
        DPLogWarning("Var on interference 8");
    }

    if(0 != readChar(buf, i))
    {
        DPLogWarning("Var on interference 9");
    }

    return true;
}


DPError DPDeserializer::deserializeELICompatible(const std::string& filename)
{
    if(!DPReadBinaryFile(filename, buf))
    {
        return DP_ERROR_UNABLE_TO_LOAD_FROM_DISK;
    }

    // parse header
    if(processHeader())
    {
        DPLogError("Error parsing header.");
        return DP_ERROR_INVALID_HEADER;
    }

    resetDebugCounts(); // reset debug counts
    
    // start moving through the data
    std::size_t n = buf.size();
    
    hasInterference = false;
    interferenceValueMajor = 0;
    interferenceValueMinor = 0;

    thisBlockCode = DP_BLOCK_UNKNOWN;
    lastBlockCode = DP_BLOCK_UNKNOWN;

    for(i = DP_HEADER_END; i < n;) // start!
    {
        if(matchEventBlock(buf, i, thisBlockCode))
        {
            if(hasBlockCode(thisBlockCode))
            {
                if(blockEventMap[thisBlockCode].getDebug())
                {
                    DPLogBlock(blockEventMap[thisBlockCode].getName(), buf,i);
                }

                blockEventMap[thisBlockCode].incrementCount();

                lastBlockCode = thisBlockCode;


                switch (thisBlockCode)
                {
                    case DP_BLOCK_LAYER_START:
                        if(processLayerStart()) continue;
                        break;
                    case DP_BLOCK_STROKE_START:
                        if(processStrokeStart()) continue;
                        break;
                    case DP_BLOCK_STROKE_END:
                        if(processStrokeEnd()) continue;
                        break;
                    case DP_BLOCK_PEN_POSITION:
                        if(processPosition()) continue;
                        break;
                    case DP_BLOCK_PEN_TILT:
                        if(processTilt()) continue;
                        break;
                    case DP_BLOCK_PEN_PRESSURE:
                        if(processPressure()) continue;
                        break;
                    case DP_BLOCK_CLOCK_COUNTER:
                        if(processClockCounter()) continue;
                        break;
                    case DP_BLOCK_CLOCK_UNKNOWN:
                        if(processClockUnknown()) continue;
                        break;
                    case DP_BLOCK_CLOCK_INIT:
                        if(processClockInit()) continue;
                        break;
                    case DP_BLOCK_C5:
                        if(processC5()) continue;
                        break;
                    case DP_BLOCK_C7_INTERFERENCE:
                        if(processInterference()) continue;
                        break;
                    case DP_BLOCK_C7_1E:
                    case DP_BLOCK_C7_1A:
                    case DP_BLOCK_C7_16:
                    case DP_BLOCK_C7_22:
                        if(processInterference()) continue;
                        break;
                    default:
                        DPLogError("Unknown BLOCK CODE: " + DPToHex(thisBlockCode));
                        break;
                }
            }
            else
            {
                if(lastBlockCode != DP_BLOCK_UNKNOWN)
                {
                    DPLogDebug("Unknown BLOCK CODE: " + DPToHex(thisBlockCode));
                    lastBlockCode = DP_BLOCK_UNKNOWN;
                }
            }
        }
        else
        {
            DPLogError("Error matching event block. Failing.");
        }

        std::cout << DPToString( (unsigned int) buf[i], 0, 4, ' ');

        i++;
    }
    
    
    if(!currentTrace.isEmpty())
    {
        currentLayer.push_back(currentTrace);
    }
    
    if(!currentLayer.isEmpty())
    {
        sketch.push_back(currentLayer);
    }
    
    dumpDebugCounts();
    
    std::cout << "--------------DEBUG VALUES--------------" << std::endl;
    std::cout << "Num Seconds    : " << numSeconds << std::endl;
    
    return DP_SUCCESS;
}
