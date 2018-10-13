//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "dpen/DPDeserializer.h"
#include "dpen/DPUtils.h"


namespace dpen {


DPDeserializer::DPDeserializer():
    hasInterference(false),
    interferenceValueMajor(0),
    interferenceValueMinor(0),
    thisBlockCode(DPBlockDefinition::DP_BLOCK_UNKNOWN),
    lastBlockCode(DPBlockDefinition::DP_BLOCK_UNKNOWN),
    samplesSinceTimestamp(0),
    lastTimestampMillis(0),
    i(0)
{
}


DPDeserializer::~DPDeserializer()
{
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

    // here we assume a sampling frequencey of 150 Hz (inkling)
    header.setSamplingFrequency(150);

    sketch.addPositionChannel();
    sketch.addTiltChannel();
    sketch.addPressureChannel();


    //    bool loadFileHB(string file) {
    //        cout << file << endl;
    //        vector<uint8_t> b;
    //        if(!readBinaryFile(file, b) && b.size() > 0) {
    //            return false;
    //        }
    //
    //                 short is =  (b[2044] << 8 ) | (b[2043]);
    //        unsigned short iu =  (b[2044] << 8 ) | (b[2043]);
    //
    //        cout << ofToString((std::size_t) b[2043],0,4,' ') << "," << ofToString((unsigned int) b[2044],0,4,' ') << "  " << is << " / " << iu << endl;
    //
    //
    //    }

    return DP_SUCCESS;
}

bool DPDeserializer::processClockCounter()
{
//    std::cout << "CLOCK_COUNTER" << std::endl;
    i+=4;

    lastTimestampMillis = readUShort(buf, i) * 1000; // get milliseconds

//    std::cout << "processClockCounter " << numSeconds << std::endl;

    std::cout << "lastTimeStampMillis: " << lastTimestampMillis << " samplesSinceTimestamp: " << samplesSinceTimestamp << std::endl;

    samplesSinceTimestamp = 0;

    return true;
}

bool DPDeserializer::processClockInit()
{
//    std::cout << "DP_BLOCK_CLOCK_INIT" << std::endl;

    if(DPBlockDefinition::blockEventMap[DPBlockDefinition::DP_BLOCK_CLOCK_INIT].getCount() > 1)
    {
        DPLogError(DPToString(DPBlockDefinition::blockEventMap[DPBlockDefinition::DP_BLOCK_CLOCK_INIT].getCount(), 0, 3, ' ') + " Clock Inits Found");
    }

    i+=4; //

    unsigned short v = readUShort(buf, i);

    if(v != 1)
    {
        DPLogDebug("Clock Init : " + DPToString(v, 0, 4, ' '));
    }

    return true;
}

bool DPDeserializer::processClockUnknown()
{
//    std::cout << "DP_BLOCK_CLOCK_UNKNOWN" << std::endl;
    if(DPBlockDefinition::blockEventMap[DPBlockDefinition::DP_BLOCK_CLOCK_UNKNOWN].getCount() > 1)
    {
        DPLogError(DPToString(DPBlockDefinition::blockEventMap[DPBlockDefinition::DP_BLOCK_CLOCK_UNKNOWN].getCount(), 0, 3, ' ') + " Clock Unknown Found");
    }

    i+=4;

    unsigned short v = readUShort(buf, i);

    if(v != 1)
    {
        DPLogDebug("Clock Unknown : " + DPToString(v, 0, 4, ' '));
    }

    return true;
}

bool DPDeserializer::processPosition()
{
    // do we have a point already defined?
    if(currentPoint.isPositionSet())
    {
        currentTrace.push_back(currentPoint);
        currentPoint.clear();
    }

    i += 2; // skip markers
    currentPoint.setX(readShort(buf, i) / 10.0f); // this scaling factor is what inkling uses for WAC
    currentPoint.setY(readShort(buf, i) /  5.0f); // this scaling factor is what inkling uses for WAC

    samplesSinceTimestamp++;

    return true;
}

bool DPDeserializer::processTilt()
{
    i+=2; // skip markers
    currentPoint.setTiltX(readChar(buf, i));
    currentPoint.setTiltY(readChar(buf, i));
    i+=2; // skip empty bytes after data
    return true;
}

bool DPDeserializer::processPressure()
{
    i+=2; // skip markers
    i+=2; // skip empty bytes before data
    currentPoint.setPressure(readUShort(buf, i));
    return true;
}

bool DPDeserializer::processLayerStart()
{
    std::cout << "DP_BLOCK_LAYER_START" << std::endl;

    i += DPBlockDefinition::blockEventMap[DPBlockDefinition::DP_BLOCK_LAYER_START].getBlockLength();

    if(!currentLayer.isEmpty())
    {
        sketch.push_back(currentLayer);
        currentLayer.clear();
    }

    return true;
}

bool DPDeserializer::processTraceStart()
{
//    std::cout << "DP_BLOCK_TRACE_START" << std::endl;

    i += DPBlockDefinition::blockEventMap[DPBlockDefinition::DP_BLOCK_TRACE_START].getBlockLength();

    // do we have a point already defined?
    if(currentPoint.isPositionSet())
    {
        currentTrace.push_back(currentPoint);
        currentPoint.clear();
    }

    if(!currentTrace.isEmpty())
    {
        currentLayer.push_back(currentTrace);
        currentTrace.clear();
    }

    return true;
}

bool DPDeserializer::processTraceEnd()
{
    i += DPBlockDefinition::blockEventMap[DPBlockDefinition::DP_BLOCK_TRACE_END].getBlockLength();
    return true;
}

bool DPDeserializer::processC5()
{
//    std::cout << "C5_UNKNNOWN" << std::endl;
    // we don't yet know what this is
    uint8_t lengthOfPacket = buf[i+1];
    i += lengthOfPacket; // consume the second section
    return true;
}

bool DPDeserializer::processC7()
{
//    std::cout << "C7_UNKNNOWN" << std::endl;
    // we don't yet know what this is
    uint8_t lengthOfPacket = buf[i+1];
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

    DPBlockDefinition::resetDebugCounts(); // reset debug counts

    // start moving through the data
    std::size_t n = buf.size();

    hasInterference = false;
    interferenceValueMajor = 0;
    interferenceValueMinor = 0;

    thisBlockCode = DPBlockDefinition::DP_BLOCK_UNKNOWN;
    lastBlockCode = DPBlockDefinition::DP_BLOCK_UNKNOWN;

    for(i = DP_HEADER_END; i < n;) // start!
    {
        if(DPBlockDefinition::matchEventBlock(buf, i, thisBlockCode))
        {
            if(DPBlockDefinition::hasBlockCode(thisBlockCode))
            {
                if(DPBlockDefinition::blockEventMap[thisBlockCode].getDebug())
                {
                    DPLogBlock(DPBlockDefinition::blockEventMap[thisBlockCode].getName(), buf,i);
                }

                DPBlockDefinition::blockEventMap[thisBlockCode].incrementCount();

                lastBlockCode = thisBlockCode;

                switch (thisBlockCode)
                {
                    case DPBlockDefinition::DP_BLOCK_LAYER_START:
                        if(processLayerStart()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_TRACE_START:
                        if(processTraceStart()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_TRACE_END:
                        if(processTraceEnd()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_PEN_POSITION:
                        if(processPosition()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_PEN_TILT:
                        if(processTilt()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_PEN_PRESSURE:
                        if(processPressure()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_CLOCK_COUNTER:
                        if(processClockCounter()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_CLOCK_UNKNOWN:
                        if(processClockUnknown()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_CLOCK_INIT:
                        if(processClockInit()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_C5:
                        if(processC5()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_C7_INTERFERENCE:
                        if(processInterference()) continue;
                        break;
                    case DPBlockDefinition::DP_BLOCK_C7_1E:
                    case DPBlockDefinition::DP_BLOCK_C7_1A:
                    case DPBlockDefinition::DP_BLOCK_C7_16:
                    case DPBlockDefinition::DP_BLOCK_C7_22:
                        if(processInterference()) continue;
                        break;
                    default:
                        DPLogError("Unknown BLOCK CODE: " + DPToHex(thisBlockCode));
                        break;
                }
            }
            else
            {
                if (lastBlockCode != DPBlockDefinition::DP_BLOCK_UNKNOWN)
                {
                    DPLogDebug("Unknown BLOCK CODE: " + DPToHex(thisBlockCode));
                    lastBlockCode = DPBlockDefinition::DP_BLOCK_UNKNOWN;
                }
            }
        }
        else
        {
            DPLogError("Error matching event block. Failing.");
        }

        // std::cout << DPToString( (std::size_t) buf[i], 0, 4, ' ');

        ++i; // working our way through ...
    }

    if (currentPoint.isPositionSet())
    {
        currentTrace.push_back(currentPoint);
    }

    if (!currentTrace.isEmpty())
    {
        currentLayer.push_back(currentTrace);
    }

    if (!currentLayer.isEmpty())
    {
        sketch.push_back(currentLayer);
    }

    DPBlockDefinition::dumpDebugCounts();

    std::cout << "--------------DEBUG VALUES--------------" << std::endl;
    std::cout << "lastTimeStampMillis    : " << lastTimestampMillis << std::endl;

    return DP_SUCCESS;
}


} // namespace dpen
