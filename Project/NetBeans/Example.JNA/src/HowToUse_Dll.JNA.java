/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// Example with the possibilty to call MediaInfo library with:
// - a file name
// - a buffer
//   - data is from a RandomAccessFile object (local file)
//   - data is from a FileUrl object (an URL, HTTP or HTTPS)
//     - without authentication
//     - with Amazon S3 authentication (input format is in the form "https://AWSAccessKeyId:AWSSecretAccessKey@s3.amazonaws.com/bucketname/filename"
 
// For "By buffer" interface, example from a file
import java.io.RandomAccessFile;

// For "By buffer" interface, example from an URL
import java.net.URL;
import java.net.HttpURLConnection;
import java.io.InputStream;

// For "By buffer" interface, example from an URL and with Amazon S3 authentication
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.security.SignatureException;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import javax.xml.bind.DatatypeConverter;

class HowToUse_Dll
{

public static void main(String... args) throws Exception
{
    String FileName = "Example.ogg";
    if (args.length > 0)
        FileName = args[0];
        
    //Comment this line and uncomment the next one if you would like to test the "by buffer" interface
    if (false)
    //if (true)
    {
        if (FileName.startsWith("http://") || FileName.startsWith("https://"))
            ByBuffer_URL (FileName);
        else
            ByBuffer_LocalFile (FileName);
    }
    else
        ByFileName (FileName);
}

private static void ByFileName(String FileName) throws Exception
{
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
    if (MI.Open(FileName)>0)
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
    To_Display += MI.Get(MediaInfo.StreamKind.General, 0, 2, MediaInfo.InfoKind.Text);

    To_Display += "\r\n\r\nCount_Get with StreamKind=Stream_Audio\r\n";
    To_Display += MI.Count_Get(MediaInfo.StreamKind.Audio, -1);

    To_Display += "\r\n\r\nGet with Stream=General and Parameter=\"AudioCount\"\r\n";
    To_Display += MI.Get(MediaInfo.StreamKind.General, 0, "AudioCount", MediaInfo.InfoKind.Text, MediaInfo.InfoKind.Name);

    To_Display += "\r\n\r\nGet with Stream=Audio and Parameter=\"StreamCount\"\r\n";
    To_Display += MI.Get(MediaInfo.StreamKind.Audio, 0, "StreamCount", MediaInfo.InfoKind.Text, MediaInfo.InfoKind.Name);

    To_Display += "\r\n\r\nGet with Stream=General and Parameter=\"FileSize\"\r\n";
    To_Display += MI.Get(MediaInfo.StreamKind.General, 0, "FileSize", MediaInfo.InfoKind.Text, MediaInfo.InfoKind.Name);

    To_Display += "\r\n\r\nClose\r\n";
    MI.Close();

    System.out.println(To_Display);
}

private static void ByBuffer_LocalFile(String FileName) throws Exception
{
    FileName=FileName.replace(" ", "%20");

    //Initializing MediaInfo
    MediaInfo MI = new MediaInfo();

    //From: preparing an example file for reading
    RandomAccessFile From = new RandomAccessFile(FileName, "r"); //From file

    //From: preparing a memory buffer for reading
    byte[] From_Buffer = new byte[64*1024];
    int    From_Buffer_Size; //The size of the read file buffer

    //Preparing to fill MediaInfo with a buffer
    MI.Open_Buffer_Init(From.length(), 0);

    //The parsing loop
    do
    {
        //Reading data somewhere, do what you want for this.
        From_Buffer_Size = From.read(From_Buffer);

        //Sending the buffer to MediaInfo
        int Result = MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);
        if ((Result & 8) == 8) // Status.Finalized
            break;

        //Testing if MediaInfo request to go elsewhere
        if (MI.Open_Buffer_Continue_GoTo_Get() != -1)
        {
            long newPos = MI.Open_Buffer_Continue_GoTo_Get();
            From.seek(newPos); //Position the file
            MI.Open_Buffer_Init(From.length(), newPos); //Informing MediaInfo we have seek
        }
    }
    while (From_Buffer_Size > 0);

    //Finalizing
    MI.Open_Buffer_Finalize(); //This is the end of the stream, MediaInfo must finnish some work

    //Get() example
    String To_Display = new String();
    To_Display += "Get with Stream=General and Parameter=\"Format\": ";
    To_Display += MI.Get(MediaInfo.StreamKind.General, 0, "Format", MediaInfo.InfoKind.Text, MediaInfo.InfoKind.Name);
    To_Display += "\r\n";
    To_Display += "Get with Stream=Video and Parameter=\"Format_Settings_GOP\": ";
    To_Display += MI.Get(MediaInfo.StreamKind.Video, 0, "Format_Settings_GOP", MediaInfo.InfoKind.Text, MediaInfo.InfoKind.Name);
    System.out.println(To_Display);
}

private static void ByBuffer_URL(String FileName) throws Exception
{
    String Amazon_S3_Date = new String(); // Amazon only
    String Amazon_S3_Authorization = new String(); // Amazon only

    // Parsing user input
    FileName = FileName.replace(" ", "%20");
    int FileName_Amazon_S3_End = FileName.indexOf("@s3.amazonaws.com/"); //Testing if Amazon is used and a AccessKeyId/SecretAccessKey is provided, TODO: use a less ugly method
    if (FileName_Amazon_S3_End != -1)
    {
        // Amazon only, computing the Authorization value
        // See http://docs.aws.amazon.com/AmazonS3/latest/dev/RESTAuthentication.html
        int FileName_Amazon_S3_Begin = FileName.indexOf("://") + 3;
        int FileName_Amazon_S3_Middle = FileName.indexOf(":", FileName_Amazon_S3_Begin);
        String Amazon_S3_AWSAccessKeyId = FileName.substring(FileName_Amazon_S3_Begin, FileName_Amazon_S3_Middle);
        String Amazon_S3_AWSSecretAccessKey = FileName.substring(FileName_Amazon_S3_Middle + 1, FileName_Amazon_S3_End);
        FileName = FileName.replace(FileName.substring(FileName_Amazon_S3_Begin, FileName_Amazon_S3_End + 1), new String());
        Amazon_S3_Date = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss Z", Locale.US).format(new Date());
        String Amazon_S3_ToSign = "GET\n\n\n"+Amazon_S3_Date+"\n"+new URL(FileName).getPath();
        Amazon_S3_Authorization = "AWS "+Amazon_S3_AWSAccessKeyId+":"+calculateHmacSHA1(Amazon_S3_ToSign, Amazon_S3_AWSSecretAccessKey);
    }
    URL FileUrl = new URL(FileName);
    
    //From: preparing an example file for reading
    HttpURLConnection HttpFrom = (HttpURLConnection ) FileUrl.openConnection();
    if (!Amazon_S3_Authorization.isEmpty())
    {
        HttpFrom.setRequestProperty("Date", Amazon_S3_Date); // Amazon only
        HttpFrom.setRequestProperty("Authorization", Amazon_S3_Authorization); // Amazon only
    }
    HttpFrom.connect();
    if (HttpFrom.getResponseCode() != 200)
    {
        java.util.Scanner S = new java.util.Scanner(HttpFrom.getErrorStream());
        S.useDelimiter("\\Z");
        System.out.println(S.next());
        return;
    }
    InputStream From = HttpFrom.getInputStream();
    long From_Length = Long.parseLong(HttpFrom.getHeaderField("Content-Length")); //.getContentLengthLong(), for 2GB+ file support is available only in Java 7 and lot of distros have Java 6

    //From: preparing a memory buffer for reading
    byte[] From_Buffer = new byte[64*1024];
    int    From_Buffer_Size; //The size of the read file buffer

    //Initializing MediaInfo
    MediaInfo MI = new MediaInfo();

    //Preparing to fill MediaInfo with a buffer
    MI.Open_Buffer_Init(From_Length, 0);

    //The parsing loop
    do
    {
        //Reading data somewhere, do what you want for this.
        From_Buffer_Size = From.read(From_Buffer);

        //Sending the buffer to MediaInfo
        int Result = MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);
        if ((Result & 8) == 8) // Status.Finalized
            break;

        //Testing if MediaInfo request to go elsewhere
        if (MI.Open_Buffer_Continue_GoTo_Get() != -1)
        {
            long newPos = MI.Open_Buffer_Continue_GoTo_Get();
            From.close();
            HttpFrom.disconnect();
            HttpFrom = (HttpURLConnection ) FileUrl.openConnection();
            if (!Amazon_S3_Authorization.isEmpty())
            {
                HttpFrom.setRequestProperty("Date", Amazon_S3_Date); // Amazon only
                HttpFrom.setRequestProperty("Authorization", Amazon_S3_Authorization); // Amazon only
            }
            HttpFrom.setRequestProperty("Range", "bytes=" + newPos + "-");
            HttpFrom.connect();
            if (HttpFrom.getResponseCode() != 206)
            {
                java.util.Scanner S = new java.util.Scanner(HttpFrom.getErrorStream());
                S.useDelimiter("\\Z");
                System.out.println(S.next());
                return;
            }
            From = HttpFrom.getInputStream();
            MI.Open_Buffer_Init(From_Length, newPos); //Informing MediaInfo we have seek, TODO: take newPos from HTTP Content-Range value
        }
    }
    while (From_Buffer_Size > 0);

    //Finalizing
    MI.Open_Buffer_Finalize(); //This is the end of the stream, MediaInfo must finish some work

    //Get() example
    String To_Display = new String();
    To_Display += "Get with Stream=General and Parameter=\"Format\": ";
    To_Display += MI.Get(MediaInfo.StreamKind.General, 0, "Format", MediaInfo.InfoKind.Text, MediaInfo.InfoKind.Name);
    To_Display += "\r\n";
    To_Display += "Get with Stream=Video and Parameter=\"Format_Settings_GOP\": ";
    To_Display += MI.Get(MediaInfo.StreamKind.Video, 0, "Format_Settings_GOP", MediaInfo.InfoKind.Text, MediaInfo.InfoKind.Name);
    System.out.println(To_Display);
}

/**
* Computes HMAC signature.
* @param data The data to be signed.
* @param secretKey The signing secret key.
* @return The Base64-encoded HMAC signature.
* @throws java.security.SignatureException when signature generation fails
*/
private static String calculateHmacSHA1(String data, String secretKey)
throws java.security.SignatureException
{
    String Result;

    try
    {
        Mac mac = Mac.getInstance("HmacSHA1"); // Create an HMAC instance
        mac.init(new SecretKeySpec(secretKey.getBytes(), "HmacSHA1")); // Initialize the HMAC instance with the secret key
        Result = DatatypeConverter.printBase64Binary(mac.doFinal(data.getBytes()));
    }
    catch (Exception e)
    {
        throw new SignatureException(e.getMessage());
    }
    return Result;
}

}
