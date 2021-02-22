# Microsoft Developer Studio Project File - Name="SurfX3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SurfX3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SurfX3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SurfX3D.mak" CFG="SurfX3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SurfX3D - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SurfX3D - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SurfX3D - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy /y Release\SurfX3D.exe ..\*.*
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SurfX3D - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=xcopy /y Debug\SurfX3D.exe ..\*.*
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SurfX3D - Win32 Release"
# Name "SurfX3D - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DlgNewSurf.cpp
# End Source File
# Begin Source File

SOURCE=.\FormF.cpp
# End Source File
# Begin Source File

SOURCE=.\FormN.cpp
# End Source File
# Begin Source File

SOURCE=.\FormP.cpp
# End Source File
# Begin Source File

SOURCE=.\FormZ.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot3DF.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot3DP.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot3DT.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot3DZ.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SurfX3D.cpp
# End Source File
# Begin Source File

SOURCE=.\SurfX3DDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SurfX3DView.cpp
# End Source File
# Begin Source File

SOURCE=.\TriMesh.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DlgNewSurf.h
# End Source File
# Begin Source File

SOURCE=.\FormF.h
# End Source File
# Begin Source File

SOURCE=.\FormN.h
# End Source File
# Begin Source File

SOURCE=.\FormP.h
# End Source File
# Begin Source File

SOURCE=.\FormZ.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Plot3D.h
# End Source File
# Begin Source File

SOURCE=.\Plot3DF.h
# End Source File
# Begin Source File

SOURCE=.\Plot3DP.h
# End Source File
# Begin Source File

SOURCE=.\Plot3DT.h
# End Source File
# Begin Source File

SOURCE=.\Plot3DZ.h
# End Source File
# Begin Source File

SOURCE=.\PlotCalc.h
# End Source File
# Begin Source File

SOURCE=.\PlotSplitter.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# PROP Ignore_Default_Tool 1
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SurfX3D.h
# End Source File
# Begin Source File

SOURCE=.\SurfX3DDoc.h
# End Source File
# Begin Source File

SOURCE=.\SurfX3DView.h
# End Source File
# Begin Source File

SOURCE=.\TriMesh.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Rendering.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SurfX3D.ico
# End Source File
# Begin Source File

SOURCE=.\SurfX3D.rc
# End Source File
# Begin Source File

SOURCE=.\res\SurfX3D.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SurfX3DDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\trefoil.bmp
# End Source File
# End Group
# Begin Group "Doc"

# PROP Default_Filter "*.doc *.txt"
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# End Group
# End Target
# End Project
