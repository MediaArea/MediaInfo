/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Core.h"
#include "Help.h"
#include "Config.h"
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************
//---------------------------------------------------------------------------
String Program_Name = __T("MediaInfo");

void Set_Program_Name(const String &Name)
{
    Program_Name = Name;
    #if defined(_MSC_VER)
        Program_Name = Program_Name.substr(Program_Name.rfind('\\')+1);
        Program_Name = Program_Name.substr(0, Program_Name.find('.'));
    #else
        Program_Name = Program_Name.substr(Program_Name.rfind('/')+1);
    #endif
}

//---------------------------------------------------------------------------
int Help()
{
    STRINGOUT(String(__T("Usage: \" [-Options...] FileName1 [Filename2...]\"")).insert(8, Program_Name));
    TEXTOUT("");
    TEXTOUT("Options:");
    TEXTOUT("--Help, -h");
    TEXTOUT("                    Display this help and exit");
    TEXTOUT("--Help-Output");
    TEXTOUT("                    Display help for Output= option");
    TEXTOUT("--Help-AnOption");
    TEXTOUT("                    Display help for \"AnOption\"");
    TEXTOUT("--Version");
    TEXTOUT("                    Display MediaInfo version and exit");
    TEXTOUT("");
    TEXTOUT("--Full, -f");
    TEXTOUT("                    Full information Display (all internal tags)");
    TEXTOUT("--Output=HTML");
    TEXTOUT("                    Full information Display with HTML tags");
    TEXTOUT("--Output=XML");
    TEXTOUT("                    Full information Display with XML tags");
    TEXTOUT("--Output=OLDXML");
    TEXTOUT("                    Full information Display with XML tags using the older");
    TEXTOUT("                    MediaInfo schema");
    TEXTOUT("--Output=JSON");
    TEXTOUT("                    Full information Display using JSON");
    TEXTOUT("--Output=EBUCore");
    TEXTOUT("                    Full information Display with EBUCore compliant XML tags");
    TEXTOUT("--Output=EBUCore_JSON");
    TEXTOUT("                    Full information Display with EBUCore 1.8 compliant JSON");
    TEXTOUT("--Output=PBCore");
    TEXTOUT("                    Full information Display with PBCore compliant XML tags");
    TEXTOUT("--Output=PBCore2");
    TEXTOUT("                    Full information Display with PBCore 2.0 compliant XML tags");
    TEXTOUT("--AcquisitionDataOutputMode=segmentParameter");
    TEXTOUT("                    Display Acquisition Data by segment then parameter (EBUCore");
    TEXTOUT("                    and NISO Z39.87 outputs)");
    TEXTOUT("--AcquisitionDataOutputMode=parameterSegment");
    TEXTOUT("                    Display Acquisition Data by parameter then segment (EBUCore");
    TEXTOUT("                    and NISO Z39.87 outputs)");
    TEXTOUT("--ExternalMetadata=...");
    TEXTOUT("                    Add external metadata to the output (EBUCore output)");
    TEXTOUT("--ExternalMetadataConfig=...");
    TEXTOUT("                    Output template for external metadata (EBUCore output)");
    TEXTOUT("--Info-Parameters");
    TEXTOUT("                    Display list of Inform= parameters");
    TEXTOUT("");
    TEXTOUT("--Language=raw");
    TEXTOUT("                    Display non-translated unique identifiers (internal text)");
    TEXTOUT("--Details=1");
    TEXTOUT("                    Display mediatrace info");
    TEXTOUT("--inform_version=1");
    TEXTOUT("                    Add MediaInfoLib version to the text output");
    TEXTOUT("--inform_timestamp=1");
    TEXTOUT("                    Add report creation timestamp to the text output");
    TEXTOUT("--File_TestContinuousFileNames=0");
    TEXTOUT("                    Disable image sequence detection");
    TEXTOUT("--LogFile=...");
    TEXTOUT("                    Save the output in the specified file");
    TEXTOUT("--BOM");
    TEXTOUT("                    Byte order mark for UTF-8 output");
    TEXTOUT("");
    TEXTOUT("--Ssl_CertificateFileName=...");
    TEXTOUT("                    File name of the SSL certificate.");
    TEXTOUT("                    The default format is \"PEM\" and can be changed");
    TEXTOUT("                    with --Ssl_CertificateFormat.");
    TEXTOUT("--Ssl_CertificateFormat=...");
    TEXTOUT("                    File format of the SSL certificate.");
    TEXTOUT("                    Supported formats are \"PEM\" and \"DER\"");
    TEXTOUT("--Ssl_PrivateKeyFileName=...");
    TEXTOUT("                    File name of the SSL private key.");
    TEXTOUT("                    The default format is \"PEM\" and can be changed");
    TEXTOUT("                    with --Ssl_PrivateKeyFormat.");
    TEXTOUT("                    Note: private key with a password is not supported.");
    TEXTOUT("--Ssl_PrivateKeyFormat=...");
    TEXTOUT("                    File format of the SSL private key.");
    TEXTOUT("                    Supported formats are \"PEM\" and \"DER\"");
    TEXTOUT("--Ssl_CertificateAuthorityFileName=...");
    TEXTOUT("                    File name of the SSL certificate authorities");
    TEXTOUT("                    to verify the peer with.");
    TEXTOUT("--Ssl_CertificateAuthorityPath=...");
    TEXTOUT("                    Path of the SSL certificate authorities");
    TEXTOUT("                    to verify the peer with.");
    TEXTOUT("--Ssl_CertificateRevocationListFileName=...");
    TEXTOUT("                    File name of the SSL certificate revocation list.");
    TEXTOUT("                    The format is \"PEM\"");
    TEXTOUT("--Ssl_IgnoreSecurity=...");
    TEXTOUT("                    Does not verify the authenticity of the peer's certificate");
    TEXTOUT("                    Use it at your own risks");
    TEXTOUT("--Ssh_PublicKeyFileName=...");
    TEXTOUT("                    File name of the SSH private key.");
    TEXTOUT("                    Default is $HOME/.ssh/id_rsa.pub or $HOME/.ssh/id_dsa.pub");
    TEXTOUT("                    if the HOME environment variable is set, and just");
    TEXTOUT("                    \"id_rsa.pub\" or \"id_dsa.pub\" in the current directory");
    TEXTOUT("                    if HOME is not set.");
    TEXTOUT("                    Note: you need to set both public and private key.");
    TEXTOUT("--Ssh_PrivateKeyFileName=...");
    TEXTOUT("                    File name of the SSH private key.");
    TEXTOUT("                    Default is $HOME/.ssh/id_rsa or $HOME/.ssh/id_dsa");
    TEXTOUT("                    if the HOME environment variable is set, and just");
    TEXTOUT("                    \"id_rsa\" or \"id_dsa\" in the current directory");
    TEXTOUT("                    if HOME is not set.");
    TEXTOUT("                    Note: you need to set both public and private key.");
    TEXTOUT("                    Note: private key with a password is not supported.");
    TEXTOUT("--Ssh_KnownHostsFileName=...");
    TEXTOUT("                    File name of the known hosts");
    TEXTOUT("                    The format is the OpenSSH file format (libssh2)");
    TEXTOUT("                    Default is $HOME/.ssh/known_hosts");
    TEXTOUT("                    if the HOME environment variable is set, and just");
    TEXTOUT("                    \"known_hosts\" in the current directory");
    TEXTOUT("                    if HOME is not set.");
    TEXTOUT("--Ssh_IgnoreSecurity");
    TEXTOUT("                    Does not verify the authenticity of the peer");
    TEXTOUT("                    (you don't need to accept the key with ssh first)");
    TEXTOUT("                    Use it at your own risks");

    return MI_OK;
}

//---------------------------------------------------------------------------
int Help_Nothing()
{
    STRINGOUT(String(__T("Usage: \" [-Options...] FileName1 [Filename2...]\"")).insert(8, Program_Name));
    STRINGOUT(String(__T("\" --Help\" for displaying more information")).insert(1, Program_Name));

    return MI_OK;
}

//---------------------------------------------------------------------------
int Help_Output()
{
    TEXTOUT("--Output=...  Specify a template (BETA)");
    STRINGOUT(String(__T("Usage: \" --Output=[xxx;]Text FileName\"")).insert(8, Program_Name));
    TEXTOUT("");
    TEXTOUT("xxx can be: General, Video, Audio, Text, Chapter, Image, Menu");
    TEXTOUT("Text can be the template text, or a filename");
    TEXTOUT("     Filename must be in the form file://filename");
    TEXTOUT("");
    TEXTOUT("See --Info-Parameters for available parameters in the text");
    TEXTOUT("(Parameters must be surrounded by \"%\" sign)");
    TEXTOUT("");
    STRINGOUT(String(__T("Usage: \" --Output=Video;%AspectRatio% FileName\"")).insert(8, Program_Name));
    TEXTOUT("");
    STRINGOUT(String(__T("Usage: \" --Output=Video;file://Video.txt FileName\"")).insert(8, Program_Name));
    TEXTOUT("and Video.txt contains ");
    TEXTOUT("\"%DisplayAspectRatio%\"        for Video Aspect Ratio.");
    TEXTOUT("");
    STRINGOUT(String(__T("Usage: \" --Output=file://Text.txt FileName\"")).insert(8, Program_Name));
    TEXTOUT("and Text.txt contains");
    TEXTOUT("\"Video;%DisplayAspectRatio%\"  for Video Aspect Ratio.");
    TEXTOUT("\"Audio;%Format%\"              for Audio Format.");

    return MI_ERROR;
}


//---------------------------------------------------------------------------
int Usage()
{
    Help_Nothing();
    return MI_ERROR;
}
