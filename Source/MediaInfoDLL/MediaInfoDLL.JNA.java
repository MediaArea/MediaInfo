/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// Note: the original stuff was well packaged with Java style,
// but I (the main developer) prefer to keep an easiest for me
// way to have all sources and example in the same place
// Removed stuff:
// "package net.sourceforge.mediainfo;"
// directory was /net/sourceforge/mediainfo

import static java.util.Collections.singletonMap;

import java.lang.reflect.Method;
import java.net.URL;

import com.sun.jna.FunctionMapper;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.NativeLibrary;
import com.sun.jna.Pointer;
import com.sun.jna.WString;

class MediaInfo
{
    static String LibraryPath="mediainfo";
    static
    {
        // libmediainfo for linux depends on libzen
        try
        {
            // We need to load dependencies first, because we know where our native libs are (e.g. Java Web Start Cache).
            // If we do not, the system will look for dependencies, but only in the library path.
            String os=System.getProperty("os.name");
            if (os!=null && !os.toLowerCase().startsWith("windows") && !os.toLowerCase().startsWith("mac"))
            {
                final ClassLoader loader=MediaInfo.class.getClassLoader();
                final String LocalPath;
                if (loader!=null)
                {
                    LocalPath=loader.getResource(MediaInfo.class.getName().replace('.', '/')+ ".class").getPath().replace("MediaInfo.class", "");
                    try
                    {
                        NativeLibrary.getInstance(LocalPath+"libzen.so.0"); // Local path
                    }
                    catch (LinkageError e)
                    {
                        NativeLibrary.getInstance("zen"); // Default path
                    }
                }
                else
                {
                    LocalPath="";
                    NativeLibrary.getInstance("zen"); // Default path
                }
                if (LocalPath.length()>0)
                {
                    try
                    {
                        NativeLibrary.getInstance(LocalPath+"libmediainfo.so.0"); // Local path
                        LibraryPath=LocalPath+"libmediainfo.so.0";
                    }
                    catch (LinkageError e)
                    {
                    }
                }
            }
        }
        catch (LinkageError  e)
        {
            //Logger.getLogger(MediaInfo.class.getName()).warning("Failed to preload libzen");
        }
    }

    //Internal stuff
    interface MediaInfoDLL_Internal extends Library
    {

        MediaInfoDLL_Internal INSTANCE = (MediaInfoDLL_Internal) Native.loadLibrary(LibraryPath, MediaInfoDLL_Internal.class, singletonMap(OPTION_FUNCTION_MAPPER, new FunctionMapper()
            {

                @Override
                public String getFunctionName(NativeLibrary lib, Method method)
                {
                    return "MediaInfo_" + method.getName();
                }
            }
        ));


        //Constructor/Destructor
        Pointer New();
        void Delete(Pointer Handle);

        //File
        int Open(Pointer Handle, WString file);
        int Open_Buffer_Init(Pointer handle, long length, long offset);
        int Open_Buffer_Continue(Pointer handle, byte[] buffer, int size);
        long Open_Buffer_Continue_GoTo_Get(Pointer handle);
        int Open_Buffer_Finalize(Pointer handle);
        void Close(Pointer Handle);

        //Infos
        WString Inform(Pointer Handle, int Reserved);
        WString Get(Pointer Handle, int StreamKind, int StreamNumber, WString parameter, int infoKind, int searchKind);
        WString GetI(Pointer Handle, int StreamKind, int StreamNumber, int parameterIndex, int infoKind);
        int     Count_Get(Pointer Handle, int StreamKind, int StreamNumber);

        //Options
        WString Option(Pointer Handle, WString option, WString value);
    }
    private Pointer Handle;

    public enum StreamKind {
        General,
        Video,
        Audio,
        Text,
        Other,
        Image,
        Menu;
    }

    //Enums
    public enum InfoKind {
        /**
         * Unique name of parameter.
         */
        Name,

        /**
         * Value of parameter.
         */
        Text,

        /**
         * Unique name of measure unit of parameter.
         */
        Measure,

        Options,

        /**
         * Translated name of parameter.
         */
        Name_Text,

        /**
         * Translated name of measure unit.
         */
        Measure_Text,

        /**
         * More information about the parameter.
         */
        Info,

        /**
         * How this parameter is supported, could be N (No), B (Beta), R (Read only), W
         * (Read/Write).
         */
        HowTo,

        /**
         * Domain of this piece of information.
         */
        Domain;
    }

    public enum Status {
        None        (0x00),
        Accepted    (0x01),
        Filled      (0x02),
        Updated     (0x04),
        Finalized   (0x08);
        
        private int value;
        private Status(int value) {this.value = value;}
        public  int getValue(int value) {return value;}
    }

    //Constructor/Destructor
    public MediaInfo()
    {
        Handle = MediaInfoDLL_Internal.INSTANCE.New();
    }

     public void dispose()
    {
        if (Handle == null)
            throw new IllegalStateException();

        MediaInfoDLL_Internal.INSTANCE.Delete(Handle);
        Handle = null;
    }

    @Override
    protected void finalize() throws Throwable
    {
        if (Handle != null)
            dispose();
    }

    //File
    /**
     * Open a file and collect information about it (technical information and tags).
     *
     * @param file full name of the file to open
     * @return 1 if file was opened, 0 if file was not not opened
     */
    public int Open(String File_Name)
    {
        return MediaInfoDLL_Internal.INSTANCE.Open(Handle, new WString(File_Name));
    }

	public int Open_Buffer_Init(long length, long offset)
    {
        return MediaInfoDLL_Internal.INSTANCE.Open_Buffer_Init(Handle, length, offset);
    }

	/**
	 *  Open a stream and collect information about it (technical information and tags) (By buffer, Continue)

	 * @param buffer pointer to the stream
	 * @param size Count of bytes to read
	 * @return a bitfield 
				bit 0: Is Accepted (format is known) 
				bit 1: Is Filled (main data is collected) 
				bit 2: Is Updated (some data have beed updated, example: duration for a real time MPEG-TS stream) 
				bit 3: Is Finalized (No more data is needed, will not use further data) 
				bit 4-15: Reserved 
				bit 16-31: User defined
	 */
	public int Open_Buffer_Continue(byte[] buffer, int size)
    {
        return MediaInfoDLL_Internal.INSTANCE.Open_Buffer_Continue(Handle, buffer, size);
    }

	public long Open_Buffer_Continue_GoTo_Get()
    {
        return MediaInfoDLL_Internal.INSTANCE.Open_Buffer_Continue_GoTo_Get(Handle);
    }

	public int Open_Buffer_Finalize()
    {
        return MediaInfoDLL_Internal.INSTANCE.Open_Buffer_Finalize(Handle);
    }

    /**
     * Close a file opened before with Open().
     *
     */
    public void Close()
    {
        MediaInfoDLL_Internal.INSTANCE.Close(Handle);
    }

    //Information
    /**
     * Get all details about a file.
     *
     * @return All details about a file in one string
     */
    public String Inform()
    {
        return MediaInfoDLL_Internal.INSTANCE.Inform(Handle, 0).toString();
    }

    /**
     * Get a piece of information about a file (parameter is a string).
     *
     * @param StreamKind Kind of Stream (general, video, audio...)
     * @param StreamNumber Stream number in Kind of Stream (first, second...)
     * @param parameter Parameter you are looking for in the Stream (Codec, width, bitrate...),
     *            in string format ("Codec", "Width"...)
     * @return a string about information you search, an empty string if there is a problem
     */
    public String Get(StreamKind StreamKind, int StreamNumber, String parameter)
    {
        return Get(StreamKind, StreamNumber, parameter, InfoKind.Text, InfoKind.Name);
    }


    /**
     * Get a piece of information about a file (parameter is a string).
     *
     * @param StreamKind Kind of Stream (general, video, audio...)
     * @param StreamNumber Stream number in Kind of Stream (first, second...)
     * @param parameter Parameter you are looking for in the Stream (Codec, width, bitrate...),
     *            in string format ("Codec", "Width"...)
     * @param infoKind Kind of information you want about the parameter (the text, the measure,
     *            the help...)
     * @param searchKind Where to look for the parameter
     */
    public String Get(StreamKind StreamKind, int StreamNumber, String parameter, InfoKind infoKind)
    {
        return Get(StreamKind, StreamNumber, parameter, infoKind, InfoKind.Name);
    }


    /**
     * Get a piece of information about a file (parameter is a string).
     *
     * @param StreamKind Kind of Stream (general, video, audio...)
     * @param StreamNumber Stream number in Kind of Stream (first, second...)
     * @param parameter Parameter you are looking for in the Stream (Codec, width, bitrate...),
     *            in string format ("Codec", "Width"...)
     * @param infoKind Kind of information you want about the parameter (the text, the measure,
     *            the help...)
     * @param searchKind Where to look for the parameter
     * @return a string about information you search, an empty string if there is a problem
     */
    public String Get(StreamKind StreamKind, int StreamNumber, String parameter, InfoKind infoKind, InfoKind searchKind)
    {
        return MediaInfoDLL_Internal.INSTANCE.Get(Handle, StreamKind.ordinal(), StreamNumber, new WString(parameter), infoKind.ordinal(), searchKind.ordinal()).toString();
    }


    /**
     * Get a piece of information about a file (parameter is an integer).
     *

     * @param StreamKind Kind of Stream (general, video, audio...)
     * @param StreamNumber Stream number in Kind of Stream (first, second...)
     * @param parameter Parameter you are looking for in the Stream (Codec, width, bitrate...),
     *            in integer format (first parameter, second parameter...)
     * @return a string about information you search, an empty string if there is a problem
     */
    public String get(StreamKind StreamKind, int StreamNumber, int parameterIndex)
    {
        return Get(StreamKind, StreamNumber, parameterIndex, InfoKind.Text);
    }


    /**
     * Get a piece of information about a file (parameter is an integer).
     *

     * @param StreamKind Kind of Stream (general, video, audio...)
     * @param StreamNumber Stream number in Kind of Stream (first, second...)
     * @param parameter Parameter you are looking for in the Stream (Codec, width, bitrate...),
     *            in integer format (first parameter, second parameter...)
     * @param infoKind Kind of information you want about the parameter (the text, the measure,
     *            the help...)
     * @return a string about information you search, an empty string if there is a problem
     */
    public String Get(StreamKind StreamKind, int StreamNumber, int parameterIndex, InfoKind infoKind)
    {
        return MediaInfoDLL_Internal.INSTANCE.GetI(Handle, StreamKind.ordinal(), StreamNumber, parameterIndex, infoKind.ordinal()).toString();
    }

    /**
     * Count of Streams of a Stream kind (StreamNumber not filled), or count of piece of
     * information in this Stream.
     *

     * @param StreamKind Kind of Stream (general, video, audio...)
     * @return number of Streams of the given Stream kind
     */
    public int Count_Get(StreamKind StreamKind)
    {
        //We should use NativeLong for -1, but it fails on 64-bit
        //int Count_Get(Pointer Handle, int StreamKind, NativeLong StreamNumber);
        //return MediaInfoDLL_Internal.INSTANCE.Count_Get(Handle, StreamKind.ordinal(), -1);
        //so we use slower Get() with a character string
        String StreamCount = Get(StreamKind, 0, "StreamCount");
        if (StreamCount == null || StreamCount.length() == 0)
            return 0;
        return Integer.parseInt(StreamCount);
    }


    /**
     * Count of Streams of a Stream kind (StreamNumber not filled), or count of piece of
     * information in this Stream.
     *
     * @param StreamKind Kind of Stream (general, video, audio...)
     * @param StreamNumber Stream number in this kind of Stream (first, second...)
     * @return number of Streams of the given Stream kind
     */
    public int Count_Get(StreamKind StreamKind, int StreamNumber)
    {
        return MediaInfoDLL_Internal.INSTANCE.Count_Get(Handle, StreamKind.ordinal(), StreamNumber);
    }



    //Options
    /**
     * Configure or get information about MediaInfo.
     *
     * @param Option The name of option
     * @return Depends on the option: by default "" (nothing) means No, other means Yes
     */
    public String Option(String Option)
    {
        return MediaInfoDLL_Internal.INSTANCE.Option(Handle, new WString(Option), new WString("")).toString();
    }

    /**
     * Configure or get information about MediaInfo.
     *
     * @param Option The name of option
     * @param Value The value of option
     * @return Depends on the option: by default "" (nothing) means No, other means Yes
     */
    public String Option(String Option, String Value)
    {
        return MediaInfoDLL_Internal.INSTANCE.Option(Handle, new WString(Option), new WString(Value)).toString();
    }

    /**
     * Configure or get information about MediaInfo (Static version).
     *
     * @param Option The name of option
     * @return Depends on the option: by default "" (nothing) means No, other means Yes
     */
    public static String Option_Static(String Option)
    {
        return MediaInfoDLL_Internal.INSTANCE.Option(MediaInfoDLL_Internal.INSTANCE.New(), new WString(Option), new WString("")).toString();
    }

    /**
     * Configure or get information about MediaInfo(Static version).
     *
     * @param Option The name of option
     * @param Value The value of option
     * @return Depends on the option: by default "" (nothing) means No, other means Yes
     */
    public static String Option_Static(String Option, String Value)
    {
        return MediaInfoDLL_Internal.INSTANCE.Option(MediaInfoDLL_Internal.INSTANCE.New(), new WString(Option), new WString(Value)).toString();
    }
}
