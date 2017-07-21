# Microsoft Developer Studio Project File - Name="ResizableLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ResizableLib - Win32 UnicodeDebug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "ResizableLib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "ResizableLib.mak" CFG="ResizableLib - Win32 UnicodeDebug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "ResizableLib - Win32 UnicodeDebug" ("Win32 (x86) Static Library" 用)
!MESSAGE "ResizableLib - Win32 UnicodeRelease" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ResizableLib - Win32 UnicodeDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ResizableLib___Win32_UnicodeDebug"
# PROP BASE Intermediate_Dir "ResizableLib___Win32_UnicodeDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UnicodeDebug"
# PROP Intermediate_Dir "UnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "L:\MSSDK\2003\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ResizableLib - Win32 UnicodeRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ResizableLib___Win32_UnicodeRelease"
# PROP BASE Intermediate_Dir "ResizableLib___Win32_UnicodeRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UnicodeRelease"
# PROP Intermediate_Dir "UnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "L:\MSSDK\2003\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ResizableLib - Win32 UnicodeDebug"
# Name "ResizableLib - Win32 UnicodeRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ResizableDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableGrip.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableMinMax.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizablePage.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizablePageEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableSheetEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableState.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ResizableDialog.h
# End Source File
# Begin Source File

SOURCE=.\ResizableFormView.h
# End Source File
# Begin Source File

SOURCE=.\ResizableGrip.h
# End Source File
# Begin Source File

SOURCE=.\ResizableLayout.h
# End Source File
# Begin Source File

SOURCE=.\ResizableMinMax.h
# End Source File
# Begin Source File

SOURCE=.\ResizablePage.h
# End Source File
# Begin Source File

SOURCE=.\ResizablePageEx.h
# End Source File
# Begin Source File

SOURCE=.\ResizableSheet.h
# End Source File
# Begin Source File

SOURCE=.\ResizableSheetEx.h
# End Source File
# Begin Source File

SOURCE=.\ResizableState.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Doc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Docs\DoDoxygen.bat
# End Source File
# Begin Source File

SOURCE=.\Docs\doxyfile.txt
# End Source File
# End Group
# Begin Source File

SOURCE=".\Artistic-License.txt"
# End Source File
# End Target
# End Project
