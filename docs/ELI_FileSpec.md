The .ELI/.WPI binary format is a serialized binary format representing pen data, including position, tilt, pressure, timing, strokes, layers, and more.

This specification is unofficial, incomplete, in progress and the result of trial and error, a hex editor, and a handful of .WPI files carefully generated with the Wacom Inkling.

Christopher Baker <http://christopherbaker.net>

#Equipment

###Wacom Inkling
The Wacom Inkling's official specifications can be found [here](http://www.wacom.com/en/Products/Inkling/Inkling-Technical-Specifics.aspx).  It appears that the sampling rate for the Inkling is 200 samples / second.  It provides 1024 levels of pressure.
###Others
…

#Header
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

#Layers
##Layer Start

A new layer is indicated by the byte sequence:


BYTE_0|BYTE_1|BYTE_2
:-:|:-:|:-:
*Marker*|*Length*|*ID*
`0xF1`|`0x03`|`0x80`


The new layer marker will be present even when there is stroke data on the layer.  In the case of the Wacom Inkling, this can happen if one presses the new layer button multiple times without putting pen to paper.  Wacom's Sketch Manager software skips such "empty" layers during display and export.

#Strokes
##Stroke Start

The beginning of a stroke (a pen down) is indicated by the 3 byte sequence:

BYTE_0|BYTE_1|BYTE_2
:-:|:-:|:-:
*Marker*|*Length*|*ID*
`0xF1`|`0x03`|`0x01`

##Stroke End

The end of the stroke (a pen lift) is indicated by the 3 byte sequence:

BYTE_0|BYTE_1|BYTE_2
:-:|:-:|:-:
*Marker*|*Length*|*ID*
`0xF1`|`0x03`|`0x00`

Pen Data
--------

###X/Y POSITION

X and Y values are encoded as a two byte `short` values.

|BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|*Marker*|*Length*|*X HIGH*|*X LOW*|*Y HIGH*|*Y LOW*|
|`0xF1`|`0x06`|`VARIES`|`VARIES`|`VARIES`|`VARIES`|

####Example

```
unsigned char  buffer[] = { 0x61, 0x06, 0x03, 0x4F, 0x13, 0x61 };
unsigned int   i = 0;
unsigned short xPos = (buffer[i+2] << 8) | (buffer[i+3]); // get x value
unsigned short yPos = (buffer[i+4] << 8) | (buffer[i+5]); // get y value
```

####Notes
The Inkling Sketch Manager software scales the resulting raw `x` and `y` values by `x / 10.0f` and `y / 5.0f` when creating the `.WAC` InkML output files. e.g.

```
float x = xPos / 10.0f; // Scale like Inkling Sketch Manager
float y = xPos /  5.0f; // Scale like Inkling Sketch Manager
```

###X/Y TILT
----

Tilt X and Tilt Y values are encoded as a one byte `char` values.

BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5
:-:|:-:|:-:|:-:|:-:|:-:|:-:
*Marker*|*Length*|*X TILT*|*Y TILT*|*UNUSED*|*UNUSED*
`0x65`|`0x06`|`VARIES`|`VARIES`|`0x00`|`0x00`|

####Example

```
unsigned char  buffer[] = { 0x65, 0x06, 0x3D, 0x28 0x00, 0x00 };
unsigned int   i = 0;
char xTilt = buffer[i+2];  // get tilt x value
char yTilt = buffer[i+3];  // get tilt y value
```

####Notes
...


###PRESSURE
----

Pressure is encoded as a two byte `unsigned short` value.

BYTE_0|BYTE_1|BYTE_2|BYTE_3|BYTE_4|BYTE_5
:-:|:-:|:-:|:-:|:-:|:-:|:-:
*Marker*|*Length*|*UNUSED*|*UNUSED*|*PRES. HIGH*|*PRES. LOW*|
`0x64`|`0x06`|`0x00`|`0x00`|`VARIES`|`VARIES`|

####Example

```
unsigned char  buffer[] = { 0x64, 0x06, 0x00, 0x00, 0x00, 0xAD };
unsigned int   i = 0;
unsigned short pressure = (buffer[i+4] << 8) | (buffer[i+5]); // get pressure 
```

####Notes
...


COUNTER/TIMER MARKER
--------------------

Beginning

0xC2 0x06 0x00 0x00 0x00 0x01

several counters then

0xC2 0x06 0x12 0x00 0x10 0x40

then sequential until the end of the file
0xC2 0x06 0x11 0x00 0x00 ?

The counter starts @ zero when the power is turned on.



UNKNOWN SEQUENCES
--------------------

The "0xC70E" sequence.  Happens when there are sensor obstructions.

e.g.
c7,0e,04,04,00,00,03,00,00,00,4a,2a,00,00w

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



