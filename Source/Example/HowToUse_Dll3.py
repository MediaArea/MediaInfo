##  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 #
 #  Use of this source code is governed by a BSD-style license that can
 #  be found in the License.html file in the root of the source tree.
 ##

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Python (Windows)  example
#
# To make this example working, you must put MediaInfo.Dll and test.avi
# in the same folder
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Should be "import MediaInfoDLL" but does not work, why?
# How to import MediaInfoDLL.py correctly?
# Example following
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

from MediaInfoDLL3 import *

MI = MediaInfo()

Version=MI.Option_Static("Info_Version", "0.7.7.0;MediaInfoDLL_Example_Python;0.7.7.0")
if Version=="":
    print("MediaInfo.Dll: this version of the DLL is not compatible")
    exit


#Information about MediaInfo
print("Info_Parameters")
MI.Option_Static("Info_Parameters")

print("")
print("Info_Capacities")
print(MI.Option_Static("Info_Capacities"))

print("")
print("Info_Codecs")
print(MI.Option_Static("Info_Codecs"))


#An example of how to use the library
print("")
print("Open")
MI.Open("Example.ogg")

print("")
print("Inform with Complete=false")
MI.Option_Static("Complete")
print(MI.Inform())

print("")
print("Inform with Complete=true")
MI.Option_Static("Complete", "1")
print(MI.Inform())

print("")
print("Custom Inform")
MI.Option_Static("Inform", "General;Example : FileSize=%FileSize%")
print(MI.Inform())

print("")
print("Get with Stream=General and Parameter='FileSize'")
print(MI.Get(Stream.General, 0, "FileSize"))

print("")
print("GetI with Stream=General and Parameter=46")
print(MI.GetI(Stream.General, 0, 46))

print("")
print("Count_Get with StreamKind=Stream_Audio")
print(MI.Count_Get(Stream.Audio))

print("")
print("Get with Stream=General and Parameter='AudioCount'")
print(MI.Get(Stream.General, 0, "AudioCount"))

print("")
print("Get with Stream=Audio and Parameter='StreamCount'")
print(MI.Get(Stream.Audio, 0, "StreamCount"))

print("")
print("Close")
MI.Close()
