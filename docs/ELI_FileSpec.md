
_The .ELI/.WPI binary format is a serialized binary format representing pen data, including position, tilt, pressure, timing, strokes, layers, and more._

_This specification is unofficial, incomplete, in progress and the result of trial and error, a hex editor, and a handful of .WPI files carefully generated with the Wacom Inkling._

_Christopher Baker <https://christopherbaker.net>_

# Equipment
## Wacom Inkling
The Wacom Inkling's official specifications can be found [here](https://web.archive.org/web/20121013141410/http://www.wacom.com/en/Products/Inkling/Inkling-Technical-Specifics.aspx).  It appears that the sampling rate for the Inkling is 200 samples / second.  It provides 1024 levels of pressure.

## Others
…

# Header
The standard header length is **2059** bytes.  There are some variations between headers, but no examples have exceeded **2059** bytes.  The contents are still a work in progress.

|Byte_Range|Function|_Notes_|
|--:|:--|:--|
|0-269|…|…|
|270-273|…|differ across devices ?|
|270-273|…|differ across devices ?|
|334-337|…|differ across devices ?|
|434-436|…|differ across devices ?|
|446-1136|…|differ across devices ?|
|1146-1837|…|differ across devices ?|
|1858-1860|…|differ across devices ?|
|1870-1871|…|differ across devices ?|
|1882-1885|…|differ across devices ?|
|1894-1897|…|differ across devices ?|
|2043-2044|…|differ across devices ?|
|2045-2059|…|Are again the same.|

# Layer and Stroke Blocks

Layer and stroke blocks are 3 bytes long and are indicated by a `0xF1` marker, and a block identification byte differentiating the specific event types.

## Layer Start Block

A new layer is indicated by block identification of `0x80`.

|BYTE_0|BYTE_1|BYTE_2|
|:----:|:----:|:----:|
|_Marker_|_Length_|_ID_|
|`0xF1`|`0x03`|`0x80`|

### Notes
> The new layer marker will be present even when there is no stroke data on the layer. In the case of the Wacom Inkling, this can happen if one presses the new layer button multiple times without putting pen to paper.  Wacom's Sketch Manager software skips such "empty" layers during display and export.

## Stroke Start Block

The beginning of a stroke (a pen down) is indicated by 3 byte sequence.

|BYTE_0|BYTE_1|BYTE_2|
|:----:|:----:|:----:|
|_Marker_|_Length_|_ID_|
|`0xF1`|`0x03`|`0x01`|

## Stroke End Block

The end of the stroke (a pen lift) is indicated by a 3 byte sequence.

|BYTE_0|BYTE_1|BYTE_2|
|:----:|:----:|:----:|
|_Marker_|_Length_|_ID_|
|`0xF1`|`0x03`|`0x00`|

# Pen Data Blocks

Wacom Inkling `.WPI` files generally present pen data in clusters of three ordered data points.  These clusters are always surrounded by [Stroke Start](#strokestart) and [Stroke End](#strokeend) events.

### Example

> *   [Stroke Start](#strokestart)
> *   ...
> *   [X/Y Position](#xyposition)
> *   [Pressure](#pressure)
> *   [X/Y Tilt](#xytilt)
> *   ...
> *   [Stroke End](#strokeend)


## X/Y POSITION Block

X and Y values are encoded as a two byte `uint16_t` values.

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|
|:----:|:----:|:----:|:----:|:----:|:----:|
|_Marker_|_Length_|_X HIGH_|_X LOW_|_Y HIGH_|_Y LOW_|
|`0xF1`|`0x06`|`VARIES`|`VARIES`|`VARIES`|`VARIES`|

### Example

```c++
uint8_t  buffer[] = { 0x61, 0x06, 0x03, 0x4F, 0x13, 0x61 };
uint32_t i = 0;
uint16_t xPos = (buffer[i+2] << 8) | (buffer[i+3]); // get x value
uint16_t yPos = (buffer[i+4] << 8) | (buffer[i+5]); // get y value
```

### Notes
> The Inkling Sketch Manager software scales the resulting raw `x` and `y` values by `x / 10.0f` and `y / 5.0f` when creating the `.WAC` InkML output files. e.g.

```c++
float x = xPos / 10.0f; // Scale like Inkling Sketch Manager
float y = xPos /  5.0f; // Scale like Inkling Sketch Manager
```

This scaling factor was empirically determined by comparing the raw `.WPI` file output to Wacom's Sketch Manager `.WAC` InkML files.

## X/Y TILT Block

Tilt X and Tilt Y values are encoded as a one byte `uint8_t` values.

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|
|:----:|:----:|:----:|:----:|:----:|:----:|
|_Marker_|_Length_|_X TILT_|_Y TILT_|_?_|_?_|
|`0x65`|`0x06`|`VARIES`|`VARIES`|`0x00`[‡](#dc)|`0x00`[‡](#dc)|

### Example

```c++
uint8_t  buffer[] = { 0x65, 0x06, 0x3D, 0x28 0x00, 0x00 };
uint32_t i = 0;
int8_t   xTilt = buffer[i+2];  // get tilt x value
int8_t   yTilt = buffer[i+3];  // get tilt y value
```

### Notes
> ...


## PRESSURE

Pressure is encoded as a two byte `uint16_t` value.  Pressure values range between 0-1024 based on the specifications.

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|
|:----:|:----:|:----:|:----:|:----:|:----:|
|_Marker_|_Length_|_?_|_?_|_PRES. HIGH_|_PRES. LOW_|
|`0x64`|`0x06`|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`VARIES`|`VARIES`|

### Example

```c++
uint8_t  buffer[] = { 0x64, 0x06, 0x00, 0x00, 0x00, 0xAD };
uint32_t i = 0;
uint16_t pressure = (buffer[i+4] << 8) | (buffer[i+5]); // get pressure
```

### Notes
> ...

# Time

Timing blocks begin with a `0xC2` are `0x06` bytes long and are identified by a block id, depending on the type of counter information.  The most important timer is the [Clock](#clock).

## Clock

Throughout each file, a time marker is recorded every second beginning with device power on. They are identified by a block id of `0x11`.  The timing sequence is composed of 6 bytes. The elapsed time is encoded in the last two bytes as an `uint16_t`. BYTE_3 may also be used to encode time, but the last two bytes can represent 2<sup>8</sup> seconds (over 18 hours). No test data of this duration has been generated.

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|
|:----:|:----:|:----:|:----:|:----:|:----:|
|_Marker_|_Length_|_ID_|_?_|_COUNTER HIGH_|_COUNTER LOW_|
|`0xC2`|`0x06`|`0x11`|`0x00`[‡](#dc)|`VARIES`|`VARIES`|

### Example
```c++
uint8_t  buffer[] = { 0xC2, 0x06, 0x11, 0x00, 0x00, 0x01 };
uint32_t i = 0;
uint16_t counter = (buffer[i+4] << 8) | (buffer[i+5]); // get counter value
```

### Notes
> ...

## Clock "Init" [INCOMPLETE](#Incomplete)

The clock "init" block happens early in all files examined -- usually before the first Clock block.  They are identified by a block id of `0x00`.  Usually this block ends with a `0x01`, but when multiples are present in longer files, the last byte can be a `0x02`.  Usually this byte alternates between `0x01` and `0x02` when multiples are present. It does not seem to affect or respond other timer values.

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|
|:----:|:----:|:----:|:----:|:----:|:----:|
|_Marker_|_Length_|_ID_|_?_|_?_|_Alternates b/t 0x01/0x02_|
|`0xC2`|`0x06`|`0x00`|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`0x01, 0x02`[‡](#dc)|

## Clock "Unknown" [INCOMPLETE](#Incomplete)

The clock "unknown" block usually only happens once per file.  It is usually located near the beginning of the file.  They are identified by a block id of `0x12`.  The last two bytes often share values between files, but not consistently.  The range of values taken by the last two bytes are fairly limited, matching occasionally, but not in a clear pattern.  It is possible that the last two bytes should be read as a `int16_t` or `uint16_t`.

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|
|:----:|:----:|:----:|:----:|:----:|:----:|
|_Marker_|_Length_|_ID_|_?_|_?_|_?_|
|`0xC2`|`0x06`|`0x12`|`0x00`[‡](#dc)|`VARIES`|`VARIES`|


# Quality / Control Blocks

## Interference / Obstructions

The interference sequence occurs when an object is placed in front of the sensor.  While the obstruction is present, bytes 10 and 11 are incremented, perhaps representing a counter.  Note that unlike position values (which are Big Endian), the counter value appears to be Little Endian.

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|BYTE_6|BYTE_7|
|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
|_Marker_|_Length_|_?_|_?_|_?_|_?_|_?_|_?_|
|`0xC7`|`0x0E`|`0x04`[‡](#dc)|`0x04`[‡](#dc)|`0x03`[‡](#dc)|`VARIES`|`VARIES`|`VARIES`|

|BYTE_8|BYTE_9|BYTE_10|BYTE_11|BYTE_12|BYTE_13|
|:----:|:----:|:-----:|:-----:|:-----:|:-----:|
|_?_|_?_|_COUNTER LOW_|_COUNTER HIGH_|_?_|_?_|
|`VARIES`|`VARIES`|`VARIES`|`VARIES`|`0x00`[‡](#dc)|`0x00`[‡](#dc)|

## 0xC7, 0x1E [INCOMPLETE](#Incomplete)

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|BYTE_6|BYTE_7|
|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
|_Marker_|_Length (30)_|_?_|_?_|_?_|_?_|_?_|_?_|
|`0xC7`|`0x1E`|`0x05 or 0x03`[‡](#dc)|`0x04 or 0x02`[‡](#dc)|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`0x07`[‡](#dc)|`0x00`[‡](#dc)|

|BYTE_8|BYTE_9|BYTE_10|BYTE_11|BYTE_12|BYTE_13|BYTE_14|BYTE_15|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|_?_|_?_|_?_|_COUNTER LOW_|_COUNTER HIGH_|_?_|_?_|_?_|
|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`VARIES`|`VARIES`|`VARIES`|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`0x00`[‡](#dc)|

|BYTE_16|BYTE_17|BYTE_18|BYTE_19|BYTE_20|BYTE_21|BYTE_22|BYTE_23|
|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
|_?_|_?_|_?_|_?_|_?_|_?_|_?_|_?_|
|`0x00`|`0x00`|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`0x00 OR 0x01`[‡](#dc)|`0x00`[‡](#dc)|`0x00`[‡](#dc)|

|BYTE_24|BYTE_25|BYTE_26|BYTE_27|BYTE_28|BYTE_29|
|:-:|:-:|:-:|:-:|:-:|:-:|
|_?_|_?_|_?_|_?_|_?_|_?_|
|`0x00`[‡](#dc)|`0x00 OR 0x01`[‡](#dc)|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`0x00`[‡](#dc)|`0x00`[‡](#dc)|

### Notes
> ...

## 0xC7, 0x1A [INCOMPLETE](#Incomplete)
### Notes
> ...

## 0xC7, 0x16 [INCOMPLETE](#Incomplete)
### Notes
> ...

## 0xC7, 0x22 [INCOMPLETE](#Incomplete)
### Notes
> ...




e.g.
c7,0e,04,04,00,00,03,00,00,00,4a,2a,00,00w

0xC7, 0x0E, 0x00


/////////////////
The "0xC513" sequence happens through the file.  The file always ends with a "0xC513" sequence and the very last one last byte in the last "0xC513" sequence is always a 0x02.

e.g.
0xC5 0x13 0x00 0x4A 0xCD 0x59 0x03 0x03 0x01 0x94 0x04 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00

e.g. last "0xC513" seq.
0xC5 0x13 0x00 0x6E 0xCD 0x59 0x03 0x06 0x01 0xB6 0x04 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x02

The "0xC71E" sequence usually happens early in the file, but can happen elsewhere.  It is usually (but not always) closely followed by a "0xC71A" sequence (below).

e.g.
0xC7 0x1E 0x03 0x02 0x00 0x00 0x07 0x00 0x00 0x00 0xE2 0x0C 0x00 0x00 0x00 0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x01 0x00 0x00 0x00 0x01 0x00 0x00 0x00

The "0xC71A" sequence always follows a "0xC71E" sequence.

e.g.
0xC7 0x1A 0x04 0x02 0x00 0x00 0x06 0x00 0x00 0x00 0x06 0x0D 0x00 0x00 0x00 0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x02 0x00 0x00 0x00


# Footnotes
*   test <a id="dcc"></a>dcc
*   <a id="*">\*</a> :
*   <a id="**">\**</a> : -
*   <a id="†">†</a> :
*   <a id="dc">‡</a> : Typical Value - could take other values, but no other values have been observed.
*   <a id="§">§</a> : -
*   <a id="Incomplete">Incomplete</a> : Incomplete - more data needed.
