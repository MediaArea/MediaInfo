/**
 * @(#)MediaInfoDLL.java 19.06.06 (dd.mm.yy)
 *
 * Copyright (2006) Bro3
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
 * Contact: bro3@users.sourceforge.net
 **/


import org.xvolks.jnative.JNative;
import org.xvolks.jnative.Type;
import org.xvolks.jnative.pointers.Pointer;
import org.xvolks.jnative.pointers.memory.MemoryBlockFactory;
import org.xvolks.jnative.exceptions.NativeException;
import org.xvolks.jnative.pointers.memory.NativeMemoryBlock;

import java.io.*;


/**
 * Class to retrieve info about media files.
 * MediaInfo library (http://mediainfo.sourceforge.net) is used
 * by the help of JNative (http://jnative.sourceforge.net)
 * to obtain technical the info about the files.
 *
 * Example:
 *
 * <pre>
 * import java.io.*;
 *  
 * class MediaInfo_Doc_Example {
 *     
 *     public static void main(String [] argv) {
 * 	
 * 	try {
 * 	    String toDisplay = "";
 * 	    
 * 	    toDisplay += "Version:" + MediaInfo.Option_Static("Info_Version");
 * 	    
 * 	    MediaInfo mi = new MediaInfo();
 * 
 * 	    if (mi.Open("Mediafile.avi") > 0) {
 * 	    
 * 		toDisplay += "\nFileSize:"+ mi.Get(MediaInfo.Stream_General, 0, "FileSize", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 		toDisplay += "\nPlayTime/String3:"+ mi.Get(0, 0, "PlayTime/String3", 1, 0);
 * 	    
 * 		toDisplay += "\nCodec/Info:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Video, 0, "Codec/Info", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 	    
 * 		toDisplay += "\nBitRate/String:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Video, 0, "BitRate/String", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 		
 * 		toDisplay += "\nAspectRatio:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Video, 0, "AspectRatio", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 	    
 * 		toDisplay += "\nFrameRate/String:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Video, 0, "FrameRate/String", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 		
 * 		toDisplay += "\nFrameCount:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Video, 0, "FrameCount", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 	    
 * 	    
 * 		toDisplay += "\nStreamKind:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Audio, 0, "StreamKind", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 
 * 		toDisplay += "\nStreamCount:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Audio, 0, "StreamCount", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 	    
 * 		toDisplay += "\nCodec/String:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Audio, 0, "Codec/String", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 	    
 * 		toDisplay += "\nBitRate/String:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Audio, 0, "BitRate/String", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 		
 * 		toDisplay += "\nBitRate_Mode:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Audio, 0, "BitRate_Mode", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 		
 * 		toDisplay += "\nChannel(s)/String:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Audio, 0, "Channel(s)/String", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 		
 * 		toDisplay += "\nSamplingRate/String:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Audio, 0, "SamplingRate/String", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 		
 * 		toDisplay += "\nResolution:";
 * 		toDisplay += mi.Get(MediaInfo.Stream_Audio, 0, "Resolution", MediaInfo.Info_Text, MediaInfo.Info_Name);
 * 		
 * 		System.out.println(toDisplay);
 * 
 * 		mi.Close();
 * 		mi.Delete();
 * 	    }
 * 	}
 *  	catch (Exception e) {
 *  	    System.out.println("Exception:" + e.getMessage());
 * 	}
 *    }
 * }
 * </pre>
 *
 *
 * @version 1.0,  &nbsp; June-2006
 * @since 1.4
 * @author bro3@users.sourceforge.net
 * @author zenitram@users.sourceforge.net
 */
class MediaInfo {
    
    /* static_fields */
    
    final public static int Stream_General       = 0;
    final public static int Stream_Video         = 1;
    final public static int Stream_Audio         = 2;
    final public static int Stream_Text          = 3;
    final public static int Stream_Chapters      = 4;
    final public static int Stream_Image         = 5;
    final public static int Stream_Menu          = 6;
    final public static int Stream_Max           = 7;

    final public static int Info_Name            = 0;
    final public static int Info_Text            = 1;
    final public static int Info_Measure         = 2;
    final public static int Info_Options         = 3;
    final public static int Info_Name_Text       = 4;
    final public static int Info_Measure_Text    = 5;
    final public static int Info_Info            = 6;
    final public static int Info_HowTo           = 7;
    final public static int Info_Max             = 8;
    
    
    /* The MediaInfo handle */
    private String handle = null;
    private JNative new_jnative;

    /* The library to be used */
    private static String libraryName = "";

    
    /**
     * Constructor that initializes the new MediaInfo object.
     * @throws NativeException  JNative Exception.
     */
    public MediaInfo() throws NativeException, Exception {
	setLibraryName();
	New();
    }


    /**
     * Constructor that initializes the new MediaInfo object.
     * @param libraryName       name of libarary to be used
     * @throws NativeException  JNative Exception
     */
    public MediaInfo(String libraryName) throws NativeException, Exception {
	setLibraryName(libraryName);
	New();
    }


    /**
     * Method New initializes the MediaInfo handle
     * @throws NativeException  JNative Exception
     */
    private void New() throws NativeException, Exception {
	
	/* Getting the handle */
	new_jnative = new JNative(libraryName, "MediaInfo_New");
	new_jnative.setRetVal(Type.INT);
	new_jnative.invoke();
	handle = new_jnative.getRetVal();
    }
    
    
    /**
     * Opens a media file.
     * Overloads method {@link #Open(int, int, int, int)}
     * @param  begin                          buffer with the begining of datas
     * @param  beginSize                      size of begin
     * @return                                1 for success and 0 for failure
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     * @see                                   #Open(int, int, int, int)
     */
    public int Open(int begin, int beginSize) throws HandleNotInitializedException, NativeException, Exception {
	return Open(begin, beginSize, 0, 0);
    }
    

    /**
     * Opens a media file.
     * @param  begin                          buffer with the begining of datas
     * @param  beginSize                      size of begin
     * @param  end                            buffer with the end of datas
     * @param  endSize                        size of end
     * @return                                1 for success and 0 for failure
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     */
    public int Open(int begin, int beginSize, int end, int endSize) throws HandleNotInitializedException, NativeException, Exception {
	
	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");

	/* Opening media file */
	JNative jnative = new JNative(libraryName, "MediaInfo_Open_Buffer");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, handle);
	jnative.setParameter(1, Type.INT, String.valueOf(begin));
	jnative.setParameter(2, Type.INT, String.valueOf(beginSize));
	jnative.setParameter(3, Type.INT, String.valueOf(end));
	jnative.setParameter(4, Type.INT, String.valueOf(endSize));
	jnative.invoke();
	
	int ret = Integer.parseInt(jnative.getRetVal());
	jnative.dispose();
	
	return ret;
    }
    

    /**
     * Opens a media file.
     * @param  filename                       the filename
     * @return                                1 for success and 0 for failure
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     */
    public int Open(String filename) throws HandleNotInitializedException, NativeException, Exception {
	
	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");
	
	Pointer fileNamePointer = new Pointer(MemoryBlockFactory.createMemoryBlock((filename.length()*2)+2));
	/* Setting the memory with the byte array returned in UTF-16, Little Endian format */
	fileNamePointer.setMemory(filename.getBytes("UTF-16LE"));

	/* Opening media file */
	JNative jnative = new JNative(libraryName, "MediaInfo_Open");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, handle);
	jnative.setParameter(1, fileNamePointer);
	jnative.invoke();

	int ret = Integer.parseInt(jnative.getRetVal());
	jnative.dispose();
	fileNamePointer.dispose();

	return ret;
    }

    
    /**
     * Gets the file info, (if available) according to the previous options set by {@link #Option(String, String)}
     * @return                                the file info
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     */
    public String Inform() throws HandleNotInitializedException, NativeException, Exception {

	String fileInfo = "";

	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");

	/* Getting file info */
	JNative jnative = new JNative(libraryName, "MediaInfo_Inform");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, handle);
	jnative.setParameter(1, Type.INT, "0"); //Necessary for backward compatibility
	jnative.invoke();

	int retAddr = Integer.parseInt(jnative.getRetVal());
	int length = findLength(retAddr);

	if (length > 0) {
	    Pointer retPointer = new Pointer(new NativeMemoryBlock(retAddr, length));
	    fileInfo = new String(retPointer.getMemory(), "UTF-16LE");
	    retPointer.dispose();
	}

	jnative.dispose();

	return fileInfo;
    }
    
    
    /**
     * Gets the specific info according to the parameters.
     * Overloads method {@link #Get(int, int, String, int, int)}.
     * @param streamKind                      type of stream. Can be any of the Stream_XX values {@link <a href="#field_detail">Field details</a>}
     * @param streamNumber                    stream number to process
     * @param parameter                       parameter string (list of strings is available with Option("Info_Parameters");
     * @return                                information
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     * @see                                   #Get(int, int, String, int, int)
     */
    public String Get(int streamKind, int streamNumber, String parameter) throws HandleNotInitializedException, NativeException, Exception {
	return Get(streamKind, streamNumber, parameter, MediaInfo.Info_Name, MediaInfo.Info_Text);
    }
    
    
    /**
     * Gets the specific info according to the parameters.
     * Overloads method {@link #Get(int, int, String, int, int)}
     * @param streamKind                      type of stream. Can be any of the Stream_XX values {@link <a href="#field_detail">Field details</a>}
     * @param streamNumber                    stream to process
     * @param parameter                       parameter string (list of strings is available with Option("Info_Parameters");
     * @param infoKind                        type of info. Can be any of the Info_XX values {@link <a href="#field_detail">Field details</a>}
     * @return                                desired information
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     * @see                                   #Get(int, int, String, int, int)
     */
    public String Get(int streamKind, int streamNumber, String parameter, int infoKind) throws HandleNotInitializedException, NativeException, Exception {
	return Get(streamKind, streamNumber, parameter, infoKind, MediaInfo.Info_Name);
    }
    
    
    /**
     * Gets the specific file info according to the parameters.
     * @param streamKind                      type of stream. Can be any of the Stream_XX values {@link <a href="#field_detail">Field details</a>}
     * @param streamNumber                    stream to process
     * @param parameter                       parameter string (list of strings is available with Option("Info_Parameters");
     * @param infoKind                        type of info. Can be any of the Info_XX values {@link <a href="#field_detail">Field details</a>}
     * @param searchKind                      type of search. Can be any of the Info_XX values {@link <a href="#field_detail">Field details</a>}
     * @return                                desired information
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     */
    public String Get(int streamKind, int streamNumber, String parameter, int infoKind, int searchKind) throws HandleNotInitializedException, NativeException, Exception {

	String fileInfo = "";

	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");

	Pointer parameterPointer = new Pointer(MemoryBlockFactory.createMemoryBlock((parameter.length()*2)+2));
	/* Setting the memory with the byte array returned in UTF-16, Little Endian format */
	parameterPointer.setMemory(parameter.getBytes("UTF-16LE"));

	/* Getting file info */
	JNative jnative = new JNative(libraryName, "MediaInfo_Get");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, handle);
	jnative.setParameter(1, Type.INT, String.valueOf(streamKind));
	jnative.setParameter(2, Type.INT, String.valueOf(streamNumber));
	jnative.setParameter(3, parameterPointer);
	jnative.setParameter(4, Type.INT, String.valueOf(infoKind));
	jnative.setParameter(5, Type.INT, String.valueOf(searchKind));
	jnative.invoke();

	int retAddr = Integer.parseInt(jnative.getRetVal());
	int length = findLength(retAddr);

	if (length > 0) {
	    Pointer retPointer = new Pointer(new NativeMemoryBlock(retAddr, length));
	    fileInfo = new String(retPointer.getMemory(), "UTF-16LE");
	    retPointer.dispose();
	}

	jnative.dispose();
	parameterPointer.dispose();

	return fileInfo;
    }
    
    
    /**
     * Gets the specific file info according to the parameters.
     * Overloads method {@link #Get(int, int, int, int)}.
     * @param streamKind                      type of stream. Can be any of the Stream_XX values {@link <a href="#field_detail">Field details</a>}
     * @param streamNumber                    stream to process
     * @param parameter                       parameter position (count of parameters is available with Count_Get(streamKind, streamNumber) )
     * @return                                desired information
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     * @see                                   #Get(int, int, int, int)
    */
    public String Get(int streamKind, int streamNumber, int parameter) throws HandleNotInitializedException, NativeException, Exception {
	return Get(streamKind, streamNumber, parameter, MediaInfo.Info_Text);
    }
    
    
    /**
     * Gets the specific file info according to the parameters.
     * @param streamKind                      type of stream. Can be any of the Stream_XX values {@link <a href="#field_detail">Field details</a>}
     * @param streamNumber                    stream to process
     * @param parameter                       parameter position (count of parameters is available with Count_Get(streamKind, streamNumber) )
     * @param infoKind                        type of info. Can be any of the Info_XX values {@link <a href="#field_detail">Field details</a>}
     * @return                                desired information
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
    */
    public String Get(int streamKind, int streamNumber, int parameter, int infoKind) throws HandleNotInitializedException, NativeException, Exception {

	String fileInfo = "";

	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");
 
	/* Getting file info */
	JNative jnative = new JNative(libraryName, "MediaInfo_GetI");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, handle);
	jnative.setParameter(1, Type.INT, String.valueOf(streamKind));
	jnative.setParameter(2, Type.INT, String.valueOf(streamNumber));
	jnative.setParameter(3, Type.INT, String.valueOf(parameter));
	jnative.setParameter(4, Type.INT, String.valueOf(infoKind));
	jnative.invoke();

	int retAddr = Integer.parseInt(jnative.getRetVal());
	int length = findLength(retAddr);

	if (length > 0) {
	    Pointer retPointer = new Pointer(new NativeMemoryBlock(retAddr, length));
	    byte [] retVal = retPointer.getMemory();

	    fileInfo = new String(retPointer.getMemory(), "UTF-16LE");
	    retPointer.dispose();
	}
	jnative.dispose();

	return fileInfo;
    }
    
    
    /**
     * Sets the option
     * Overloads method {@link #Option(String, String)}
     * @param option                          name of option
     * @return                                desired information or status of the option
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     * @see #Option(String, String)
     */
    public String Option(String option) throws HandleNotInitializedException, NativeException, Exception {
	return Option(option, "");
    }
    
    
    /**
     * Sets the option with value
     * @param option                          name of option
     * @param value                           option value
     * @return                                desired information or status of the option
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
    */
    public String Option(String option, String value) throws HandleNotInitializedException, NativeException, Exception {

	String fileInfo = "";

	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");

	/* Allocating and setting the memory with the byte array returned in UTF-16, Little Endian format */
	Pointer optionPointer = new Pointer(MemoryBlockFactory.createMemoryBlock((option.length()*2)+2));
	optionPointer.setMemory(option.getBytes("UTF-16LE"));

	/* Allocating and setting the memory with the byte array returned in UTF-16, Little Endian format */
	Pointer valuePointer = new Pointer(MemoryBlockFactory.createMemoryBlock((value.length()*2)+2));
	valuePointer.setMemory(value.getBytes("UTF-16LE"));

	/* Getting file info */
	JNative jnative = new JNative(libraryName, "MediaInfo_Option");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, handle);
	jnative.setParameter(1, optionPointer);
	jnative.setParameter(2, valuePointer);
	jnative.invoke();

	int retAddr = Integer.parseInt(jnative.getRetVal());
	int length = findLength(retAddr);

	if (length > 0) {

	    Pointer retPointer = new Pointer(new NativeMemoryBlock(retAddr, length));
	    byte [] retVal = retPointer.getMemory();

	    fileInfo = new String(retPointer.getMemory(), "UTF-16LE");
	    retPointer.dispose();
	}

	optionPointer.dispose();
	valuePointer.dispose();
	jnative.dispose();

	return fileInfo;
    }
    

    /**
     * Sets the option (you do not need to create a MediaInfo handle)
     * Overloads method {@link #Option_Static(String, String)}
     * @param option                          name of option
     * @return                                desired information or status of the option
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     * @see                                   #Option_Static(String, String)
     */
    static public String Option_Static(String option) throws HandleNotInitializedException, NativeException, Exception {
	return Option_Static(option, "");
    }
    
    
    /**
     * Sets the option (you do not need to create a MediaInfo handle)
     * @param option                          name of option
     * @param value                           option value
     * @return                                desired information or status of the option
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
    */
    static public String Option_Static(String option, String value) throws HandleNotInitializedException, NativeException, Exception {
	
	if (libraryName.equals(""))
	    setLibraryName();
	
	String fileInfo = "";
	
	/* Allocating and setting the memory with the byte array returned in UTF-16, Little Endian format */
	Pointer optionPointer = new Pointer(MemoryBlockFactory.createMemoryBlock((option.length()*2)+2));
	optionPointer.setMemory(option.getBytes("UTF-16LE"));

	/* Allocating and setting the memory with the byte array returned in UTF-16, Little Endian format */
	Pointer valuePointer = new Pointer(MemoryBlockFactory.createMemoryBlock((value.length()*2)+2));
	valuePointer.setMemory(value.getBytes("UTF-16LE"));

	/* Getting file info */
	JNative jnative = new JNative(libraryName, "MediaInfo_Option");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, "0");
	jnative.setParameter(1, optionPointer);
	jnative.setParameter(2, valuePointer);
	jnative.invoke();

	int retAddr = Integer.parseInt(jnative.getRetVal());
	int length = findLength(retAddr);

	if (length > 0) {
	    Pointer retPointer = new Pointer(new NativeMemoryBlock(retAddr, length));
	    byte [] retVal = retPointer.getMemory();

	    fileInfo = new String(retPointer.getMemory(), "UTF-16LE");
	    retPointer.dispose();
	}

	optionPointer.dispose();
	valuePointer.dispose();
	jnative.dispose();

	return fileInfo;
    }


    /**
     * Gets the state of the libaray
     * @return                                state of the library (between 0 and 10000)
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
    */
    public int State_Get() throws HandleNotInitializedException, NativeException, Exception {

	int retval;

	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");

	/* Getting file info */
	JNative jnative = new JNative(libraryName, "MediaInfo_State_Get");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, handle);
	jnative.invoke();

	retval = Integer.parseInt(jnative.getRetVal());
	jnative.dispose();

	return retval;
    }
    
    
    /**
     * Gets the count of streams
     * Overloads method {@link #Count_Get(int, int)}.
     * @param streamKind                      type of stream. Can be any of the Stream_XX values {@link <a href="#field_detail">Field details</a>}
     * @return                                count of streams
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     * @see                                   #Count_Get(int, int)
     */
    public int Count_Get(int streamKind) throws HandleNotInitializedException, NativeException, Exception {
	return Count_Get(streamKind, -1);
    }
    
    
    /**
     * Gets the count of streams
     * @param streamKind                      type of stream. Can be any of the Stream_XX values {@link <a href="#field_detail">Field details</a>}
     * @param streamNumber                    stream to process
     * @return                                count of parameters for a specific stream
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     */
    public int Count_Get(int streamKind, int streamNumber) throws HandleNotInitializedException, NativeException, Exception {

	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");

	/* Getting file info */
	JNative jnative = new JNative(libraryName, "MediaInfo_Count_Get");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.INT, handle);
	jnative.setParameter(1, Type.INT, String.valueOf(streamKind));
	jnative.setParameter(2, Type.INT, String.valueOf(streamNumber));
	jnative.invoke();

	int retval = Integer.parseInt(jnative.getRetVal());
	jnative.dispose();

	return retval;
    }


    /**
     * Deletes the handle
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     */
    public void Delete() throws HandleNotInitializedException, NativeException, Exception {

	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");

	/* Free native memory */
	JNative jnative = new JNative(libraryName, "MediaInfo_Delete");
	jnative.setParameter(0, Type.INT, handle);
	jnative.invoke();
	jnative.dispose();

	/* disposing the jnative instance that created the handle */
	new_jnative.dispose();
    }


    /**
     * Closes the handle
     * @throws HandleNotInitializedException  if the handle is null
     * @throws NativeException                JNative Exception
     */
    public void Close() throws HandleNotInitializedException, NativeException, Exception {

	if (handle == null)
	    throw new HandleNotInitializedException("Handle is not initialized.");

	/* Free native memory */
	JNative jnative = new JNative(libraryName, "MediaInfo_Close");
	jnative.setParameter(0, Type.INT, handle);
	jnative.invoke();
	jnative.dispose();
    }


     /**
     * Finds the the length of the string starting at address ptr.
     * @param ptr              start address of the string
     * @return                 the length of the string
     * @throws NativeException JNative Exception
     */
    public int lstrlenW(int ptr) throws NativeException, Exception {

	/* Getting file info */
	JNative jnative = new JNative(libraryName, "lstrlenW");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, Type.STRING, String.valueOf(ptr));
	jnative.invoke();

	int retval = Integer.parseInt(jnative.getRetVal());
	jnative.dispose();

	return retval;
    }


    /**
     * Copies the string value to memory starting at address ptr
     * @param value            the value to be copied
     * @param ptr              address where the string should be copied
     * @return                 the length of the string
     * @throws NativeException JNative Exception
     */
    public int lstrcpyW(String value, int ptr) throws NativeException, Exception {

	int retval;

	/* Allocating and setting the memory with the byte array returned in UTF-16, Little Endian format */
	Pointer valuePointer = new Pointer(MemoryBlockFactory.createMemoryBlock((value.length()*2)+2));
	valuePointer.setMemory(value.getBytes("UTF-16LE"));

	/* Getting file info */
	JNative jnative = new JNative(libraryName, "lstrcpyW");
	jnative.setRetVal(Type.INT);
	jnative.setParameter(0, valuePointer);
	jnative.setParameter(1, Type.INT, String.valueOf(ptr));
	jnative.invoke();

	retval = Integer.parseInt(jnative.getRetVal());
	jnative.dispose();
	valuePointer.dispose();

	return retval;
    }


    /**
     * Finds the length of the string starting at address addr
     * @param addr             address of the string
     * @return                 the length of the Unicode string
     * @throws NativeException JNative Exception
     */
    static int findLength(int addr) throws NativeException {

	byte [] strEnd = {0,0};
	int howFarToSearch = 10000;
	int length = 0;
	int temp = 0;

	while (true) {

	    temp = JNative.searchNativePattern(addr + length, strEnd, howFarToSearch);

	    /* The strEnd wasn't found */
	    if (temp == -1) {
		howFarToSearch += 10000;
	    }
	    else {
		length += temp;

		if ((addr + length) % 2 == 0) {
		    break;
		}
		/* Adding 1 so it will start at the last addr+1 */
		length += 1;
	    }
	}
	return length;
    }
    
    
    /**
     * Method isWindows
     * @return                  true if OS is windows
     */
    private static boolean isWindows() { 
	String os = System.getProperty("os.name"); 
	return (os != null && os.toLowerCase().startsWith("windows")) ? true : false; 
    }
    
    
    /**
     * Sets the default name of the library to be used.
     * If windows -> "MediaInfo.dll" else -> "libmediainfo.so.0"
     */
    public static void setLibraryName() {
	
	if (libraryName.equals("")) {
	    if (isWindows())
		setLibraryName("MediaInfo.dll");
	    else
		setLibraryName("libmediainfo.so.0");
	}
    }
    

    /**
     * Sets the name of the library to be used.
     * @param libName            name of the library
     */
    public static void setLibraryName(String libName) {
	libraryName = libName;
    }
    
}


/**
 * Exception thrown if the handle isn't initialized.
 */
class HandleNotInitializedException extends Exception {
    
    HandleNotInitializedException(String msg) {
	super(msg);
    }
}
