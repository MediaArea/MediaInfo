/**
 * @(#)HowToUse_Dll.java 18.04.06 (dd.mm.yy)
 *
 * Copyright (2006) zenitram
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser Public License as published by the Free Software
 * Foundation; either version 2.1, or any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Boston, MA 02111.
 *
 * Contact: zenitram@users.sourceforge.net
 **/


import java.io.*;

class HowToUse_Dll
{

public static void main(String[] argv)
{
	String filename = "Example.ogg";
	if (argv.length > 0)
	    filename = argv[0];
	
	if (!new File(filename).isFile())
	    {
		System.err.println("File does not exist:" + filename);
		return;
	    }
	
	try
	{
	    System.out.println(displayFileSummary(filename));
	}
	catch (HandleNotInitializedException e) {
	    System.err.println(e.getMessage());
	}
	catch (Exception e)
	    {
		System.out.println("Exception:" + e.getMessage());
	    }
}


/**
 * An example of how to use the library
 */
    static String displayFileSummary(String filename) throws Exception
    {
	/* Path to the library */
	MediaInfo.setLibraryName("MediaInfo.dll");
	
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
	MI.Open(filename);

	To_Display += "\r\n\r\nInform with Complete=false\r\n";
	MI.Option("Complete", "");
	To_Display += MI.Inform();

	To_Display += "\r\n\r\nInform with Complete=true\r\n";
	MI.Option("Complete", "1");
	To_Display += MI.Inform();

	To_Display += "\r\n\r\nCustom Inform\r\n";
	MI.Option("Inform", "General;Example : FileSize=%FileSize%");
	To_Display += MI.Inform();

	To_Display += "\r\n\r\nGetI with Stream=General and Parameter=13\r\n";
	To_Display += MI.Get(MediaInfo.Stream_General, 0, 46, MediaInfo.Info_Text);

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

        return To_Display;
    }
}
