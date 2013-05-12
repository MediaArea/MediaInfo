/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

class HowToUse_Dll
{

public static void main(String... args) throws Exception
{
    String File_Name = "Example.ogg";
    if (args.length > 0)
        File_Name = args[0];

    String To_Display = "";

    //Info about the library

    To_Display += MediaInfo.Option_Static("Info_Version");

    To_Display += "\r\n\r\nInfo_Parameters\r\n";
    To_Display += MediaInfo.Option_Static("Info_Parameters");

    To_Display += "\r\n\r\nInfo_Capacities\r\n";
    To_Display += MediaInfo.Option_Static("Info_Capacities");

    To_Display += "\r\n\r\nInfo_Codecs\r\n";
    To_Display += MediaInfo.Option_Static("Info_Codecs");


    //An example of how to use the library

    MediaInfo MI = new MediaInfo();

    To_Display += "\r\n\r\nOpen\r\n";
    if (MI.Open(File_Name)>0)
            To_Display+="is OK\r\n";
    else
            To_Display+="has a problem\r\n";

    To_Display += "\r\n\r\nInform with Complete=false\r\n";
    MI.Option("Complete", "");
    To_Display += MI.Inform();

    To_Display += "\r\n\r\nInform with Complete=true\r\n";
    MI.Option("Complete", "1");
    To_Display += MI.Inform();

    To_Display += "\r\n\r\nCustom Inform\r\n";
    MI.Option("Inform", "General;Example : FileSize=%FileSize%");
    To_Display += MI.Inform();

    To_Display += "\r\n\r\nGetI with Stream=General and Parameter=2\r\n";
    To_Display += MI.Get(MediaInfo.Stream_General, 0, 2, MediaInfo.Info_Text);

    To_Display += "\r\n\r\nCount_Get with StreamKind=Stream_Audio\r\n";
    To_Display += MI.Count_Get(MediaInfo.Stream_Audio, -1);

    To_Display += "\r\n\r\nGet with Stream=General and Parameter=\"AudioCount\"\r\n";
    To_Display += MI.Get(MediaInfo.Stream_General, 0, "AudioCount", MediaInfo.Info_Text, MediaInfo.Info_Name);

    To_Display += "\r\n\r\nGet with Stream=Audio and Parameter=\"StreamCount\"\r\n";
    To_Display += MI.Get(MediaInfo.Stream_Audio, 0, "StreamCount", MediaInfo.Info_Text, MediaInfo.Info_Name);

    To_Display += "\r\n\r\nGet with Stream=General and Parameter=\"FileSize\"\r\n";
    To_Display += MI.Get(MediaInfo.Stream_General, 0, "FileSize", MediaInfo.Info_Text, MediaInfo.Info_Name);

    To_Display += "\r\n\r\nClose\r\n";
    MI.Close();

    System.out.println(To_Display);
    }
}
