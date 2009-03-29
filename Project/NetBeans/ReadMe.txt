Java binding for MediaInfo DLL/SO

Two alternatives are possible depends on which "binding interface" you desire:
- JNA     http://jna.dev.java.net
- JNAtive http://jnative.sourceforge.net

Note:
NetBeans example may be sometimes outdated, because I didn't find how to set NetBeans for linking to an existant files instead of copying it in the NetBeans/src directory.
If there is a problem, try first to copy HowToUse_Dll.*.java and MediaInfoDLL.*.java from the /Source directory