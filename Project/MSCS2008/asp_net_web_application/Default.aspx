<%
//  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 //
 // Use of this source code is governed by a BSD-style license that can
 // be found in the License.html file in the root of the source tree.
 ///

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Microsoft Asp.net C# example
//
// To make this example working, you must put MediaInfo.Dll and Example.ogg
// in the "./Bin/" folder
// and add MediaInfoDll.cs to your project
//
// Know issue: I don't know what is the default path when a file
//             is open without path.
//             Complete file name (path+filename) must be provided
//             Don't hesitate to contact Info@MediaArea.net if you know how
//             to open Example.ogg in the bin directory whereever is located
//             the project.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
%>


<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="asp_net_web_application._Default" %>
<%@Import Namespace="MediaInfoLib" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
    <%
        //Test if version of DLL is compatible : 3rd argument is "version of DLL tested;Your application name;Your application version"
        String ToDisplay;
        MediaInfo MI = new MediaInfo();

        ToDisplay = MI.Option("Info_Version", "0.7.0.0;MediaInfoDLL_Example_CS;0.7.0.0");
        if (ToDisplay.Length == 0)
        {
            Response.Write("MediaInfo.Dll: this version of the DLL is not compatible");
            return;
        }

        //Information about MediaInfo
        ToDisplay += "\r\n\r\nInfo_Parameters\r\n";
        ToDisplay += MI.Option("Info_Parameters");

        ToDisplay += "\r\n\r\nInfo_Capacities\r\n";
        ToDisplay += MI.Option("Info_Capacities");

        ToDisplay += "\r\n\r\nInfo_Codecs\r\n";
        ToDisplay += MI.Option("Info_Codecs");

        //An example of how to use the library
        ToDisplay += "\r\n\r\nOpen\r\n";
        MI.Open("Example.ogg"); //Must be the complete path (ie "C:\\Example.ogg")

        ToDisplay += "\r\n\r\nInform with Complete=false\r\n";
        MI.Option("Complete");
        ToDisplay += MI.Inform();

        ToDisplay += "\r\n\r\nInform with Complete=true\r\n";
        MI.Option("Complete", "1");
        ToDisplay += MI.Inform();

        ToDisplay += "\r\n\r\nCustom Inform\r\n";
        MI.Option("Inform", "General;File size is %FileSize% bytes");
        ToDisplay += MI.Inform();

        ToDisplay += "\r\n\r\nGet with Stream=General and Parameter='FileSize'\r\n";
        ToDisplay += MI.Get(0, 0, "FileSize");

        ToDisplay += "\r\n\r\nGet with Stream=General and Parameter=46\r\n";
        ToDisplay += MI.Get(0, 0, 46);

        ToDisplay += "\r\n\r\nCount_Get with StreamKind=Stream_Audio\r\n";
        ToDisplay += MI.Count_Get(StreamKind.Audio);

        ToDisplay += "\r\n\r\nGet with Stream=General and Parameter='AudioCount'\r\n";
        ToDisplay += MI.Get(StreamKind.General, 0, "AudioCount");

        ToDisplay += "\r\n\r\nGet with Stream=Audio and Parameter='StreamCount'\r\n";
        ToDisplay += MI.Get(StreamKind.Audio, 0, "StreamCount");

        ToDisplay += "\r\n\r\nClose\r\n";
        MI.Close();

        //Formating to HTML
        ToDisplay=ToDisplay.Replace("\r\n", "<br />\r\n");
        
        //Displaying the text
        Response.Write(ToDisplay);
        %>
    </div>
    </form>
</body>
</html>
