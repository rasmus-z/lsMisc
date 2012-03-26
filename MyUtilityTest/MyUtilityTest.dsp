# Microsoft Developer Studio Project File - Name="MyUtilityTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MyUtilityTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MyUtilityTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MyUtilityTest.mak" CFG="MyUtilityTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MyUtilityTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MyUtilityTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MyUtilityTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "MyUtilityTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MyUtilityTest - Win32 Release"
# Name "MyUtilityTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\addBaseElement.cpp
# End Source File
# Begin Source File

SOURCE=..\addCharset.cpp
# End Source File
# Begin Source File

SOURCE=..\browseFolder.cpp
# End Source File
# Begin Source File

SOURCE=..\CenterWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\ChangeDirPos.cpp
# End Source File
# Begin Source File

SOURCE=..\ChangeFilenamable.cpp
# End Source File
# Begin Source File

SOURCE=..\ChangeFilenamable2.cpp
# End Source File
# Begin Source File

SOURCE=..\ConfirmErrorRetry.cpp
# End Source File
# Begin Source File

SOURCE=..\CreateCompleteDirectory.cpp
# End Source File
# Begin Source File

SOURCE=..\CreateFolderIniPath.cpp
# End Source File
# Begin Source File

SOURCE=..\GetClipboardTexts.cpp
# End Source File
# Begin Source File

SOURCE=..\GetDesktopDir.cpp
# End Source File
# Begin Source File

SOURCE=..\GetDirFromPath.cpp
# End Source File
# Begin Source File

SOURCE=..\GetFilableUserName.cpp
# End Source File
# Begin Source File

SOURCE=..\GetFormattedTime.cpp
# End Source File
# Begin Source File

SOURCE=..\getFromHtmlLikeDataW.cpp
# End Source File
# Begin Source File

SOURCE=..\GetModuleDirectory.cpp
# End Source File
# Begin Source File

SOURCE=..\GetSelItems.cpp
# End Source File
# Begin Source File

SOURCE=..\GetSpecialFolderPath.cpp
# End Source File
# Begin Source File

SOURCE=..\GetTopParent.cpp
# End Source File
# Begin Source File

SOURCE=..\GetUnexistingFile.cpp
# End Source File
# Begin Source File

SOURCE=..\GetURLFromClipboard.cpp
# End Source File
# Begin Source File

SOURCE=..\HtmlClipboard.cpp
# End Source File
# Begin Source File

SOURCE=..\IsAllDigit.cpp
# End Source File
# Begin Source File

SOURCE=..\IsHasExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\IsInDesktop.cpp
# End Source File
# Begin Source File

SOURCE=..\IsRegKeyExists.cpp
# End Source File
# Begin Source File

SOURCE=..\IsSamePath.cpp
# End Source File
# Begin Source File

SOURCE=..\IsTextFileExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\IsValidString.cpp
# End Source File
# Begin Source File

SOURCE=..\IsWindowsNT.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=..\myStrStrI.cpp
# End Source File
# Begin Source File

SOURCE=..\ProcessList.cpp
# End Source File
# Begin Source File

SOURCE=..\Registory.cpp
# End Source File
# Begin Source File

SOURCE=..\RemoveLastInValidURLChar.cpp
# End Source File
# Begin Source File

SOURCE=..\ResolveLink.cpp
# End Source File
# Begin Source File

SOURCE=..\SetClipboardText.cpp
# End Source File
# Begin Source File

SOURCE=..\SHDeleteFile.cpp
# End Source File
# Begin Source File

SOURCE=..\SHMoveFile.cpp
# End Source File
# Begin Source File

SOURCE=..\SJISToEUC.cpp
# End Source File
# Begin Source File

SOURCE=..\SkipSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\StdStringReplace.cpp
# End Source File
# Begin Source File

SOURCE=..\UnHtmlEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\UrlEncode2.cpp
# End Source File
# Begin Source File

SOURCE=..\UTF16toUTF8.cpp
# End Source File
# Begin Source File

SOURCE=..\WritePrivateProfileInt.cpp
# End Source File
# Begin Source File

SOURCE=..\XBrowseForFolder.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\addBaseElement.h
# End Source File
# Begin Source File

SOURCE=..\addCharset.h
# End Source File
# Begin Source File

SOURCE=..\AnyCloser.h
# End Source File
# Begin Source File

SOURCE=..\browseFolder.h
# End Source File
# Begin Source File

SOURCE=..\CBool.h
# End Source File
# Begin Source File

SOURCE=..\CenterWindow.h
# End Source File
# Begin Source File

SOURCE=..\ChangeDirPos.h
# End Source File
# Begin Source File

SOURCE=..\ChangeFilenamable.h
# End Source File
# Begin Source File

SOURCE=..\ChangeFilenamable2.h
# End Source File
# Begin Source File

SOURCE=..\CInt.h
# End Source File
# Begin Source File

SOURCE=..\ConfirmErrorRetry.h
# End Source File
# Begin Source File

SOURCE=..\CreateCompleteDirectory.h
# End Source File
# Begin Source File

SOURCE=..\CreateFolderIniPath.h
# End Source File
# Begin Source File

SOURCE=..\EveryCloser.h
# End Source File
# Begin Source File

SOURCE=..\GetClipboardTexts.h
# End Source File
# Begin Source File

SOURCE=..\GetDesktopDir.h
# End Source File
# Begin Source File

SOURCE=..\GetDirFromPath.h
# End Source File
# Begin Source File

SOURCE=..\GetFilableUserName.h
# End Source File
# Begin Source File

SOURCE=..\GetFormattedTime.h
# End Source File
# Begin Source File

SOURCE=..\getFromHtmlLikeDataW.h
# End Source File
# Begin Source File

SOURCE=..\GetIETempFile.h
# End Source File
# Begin Source File

SOURCE=..\GetModuleDirectory.h
# End Source File
# Begin Source File

SOURCE=..\GetSelItems.h
# End Source File
# Begin Source File

SOURCE=..\GetSpecialFolderPath.h
# End Source File
# Begin Source File

SOURCE=..\GetTopParent.h
# End Source File
# Begin Source File

SOURCE=..\GetUnexistingFile.h
# End Source File
# Begin Source File

SOURCE=..\GetURLFromClipboard.h
# End Source File
# Begin Source File

SOURCE=..\HelpDefines.h
# End Source File
# Begin Source File

SOURCE=..\HtmlClipboard.h
# End Source File
# Begin Source File

SOURCE=..\IsAllDigit.h
# End Source File
# Begin Source File

SOURCE=..\IsDirectory.h
# End Source File
# Begin Source File

SOURCE=..\IsHasExtension.h
# End Source File
# Begin Source File

SOURCE=..\IsInDesktop.h
# End Source File
# Begin Source File

SOURCE=..\IsRegKeyExists.h
# End Source File
# Begin Source File

SOURCE=..\IsSamePath.h
# End Source File
# Begin Source File

SOURCE=..\IsTextFileExtension.h
# End Source File
# Begin Source File

SOURCE=..\IsValidString.h
# End Source File
# Begin Source File

SOURCE=..\IsWindowsNT.h
# End Source File
# Begin Source File

SOURCE=..\Lock.h
# End Source File
# Begin Source File

SOURCE=..\MFCHelper.h
# End Source File
# Begin Source File

SOURCE=..\myStrStrI.h
# End Source File
# Begin Source File

SOURCE=..\ProcessList.h
# End Source File
# Begin Source File

SOURCE=..\Registory.h
# End Source File
# Begin Source File

SOURCE=..\RemoveLastInValidURLChar.h
# End Source File
# Begin Source File

SOURCE=..\ResolveLink.h
# End Source File
# Begin Source File

SOURCE=..\SetClipboardText.h
# End Source File
# Begin Source File

SOURCE=..\SHDeleteFile.h
# End Source File
# Begin Source File

SOURCE=..\SHMoveFile.h
# End Source File
# Begin Source File

SOURCE=..\SJISToEUC.h
# End Source File
# Begin Source File

SOURCE=..\SkipSpace.h
# End Source File
# Begin Source File

SOURCE=..\StdStringReplace.h
# End Source File
# Begin Source File

SOURCE=..\tlsMap.h
# End Source File
# Begin Source File

SOURCE=..\tstring.h
# End Source File
# Begin Source File

SOURCE=..\TypeHelp.h
# End Source File
# Begin Source File

SOURCE=..\UnHtmlEntity.h
# End Source File
# Begin Source File

SOURCE=..\UrlEncode2.h
# End Source File
# Begin Source File

SOURCE=..\UTF16toUTF8.h
# End Source File
# Begin Source File

SOURCE=..\verify.h
# End Source File
# Begin Source File

SOURCE=..\WritePrivateProfileInt.h
# End Source File
# Begin Source File

SOURCE=..\XBrowseForFolder.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
