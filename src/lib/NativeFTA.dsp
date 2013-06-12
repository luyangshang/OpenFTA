# Microsoft Developer Studio Project File - Name="NativeFTA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=NativeFTA - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NativeFTA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NativeFTA.mak" CFG="NativeFTA - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NativeFTA - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NativeFTA - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NativeFTA - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "Appn" /I "Analysis" /I "Widgets" /I "Utilities" /I "Canvas" /I "PrimaryEvents" /I "JNI" /I "..\gui\FTAGUI" /I "C:\j2sdk1.4.2_04\include" /I "C:\j2sdk1.4.2_04\include\win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/fta.dll"

!ELSEIF  "$(CFG)" == "NativeFTA - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "Appn" /I "Analysis" /I "Widgets" /I "Utilities" /I "Canvas" /I "PrimaryEvents" /I "JNI" /I "..\gui\FTAGUI" /I "C:\j2sdk1.4.2_04\include" /I "C:\j2sdk1.4.2_04\include\win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "DEBUG" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/fta.dll" /pdbtype:sept /libpath:"C:\j2sdk1.4.2_04\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug\fta.dll ..\..
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "NativeFTA - Win32 Release"
# Name "NativeFTA - Win32 Debug"
# Begin Group "Appn"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Appn\analysis_pd.c
# End Source File
# Begin Source File

SOURCE=.\Appn\bits.c
# End Source File
# Begin Source File

SOURCE=.\Appn\display_item.c
# End Source File
# Begin Source File

SOURCE=.\Appn\display_tree.c
# End Source File
# Begin Source File

SOURCE=.\Appn\edit_pd.c
# End Source File
# Begin Source File

SOURCE=.\Appn\event_list.c
# End Source File
# Begin Source File

SOURCE=.\Appn\file_pd.c
# End Source File
# Begin Source File

SOURCE=.\Appn\FileDialogs.c
# End Source File
# Begin Source File

SOURCE=.\Appn\item.c
# End Source File
# Begin Source File

SOURCE=.\Appn\level.c
# End Source File
# Begin Source File

SOURCE=.\Appn\print_report_pd.c
# End Source File
# Begin Source File

SOURCE=.\Appn\segments.c
# End Source File
# Begin Source File

SOURCE=.\Appn\symbol.c
# End Source File
# Begin Source File

SOURCE=.\Appn\symbol_menu.c
# End Source File
# Begin Source File

SOURCE=.\Appn\symbol_text.c
# End Source File
# Begin Source File

SOURCE=.\Appn\tree_pic.c
# End Source File
# Begin Source File

SOURCE=.\Appn\treeUtil.c
# End Source File
# Begin Source File

SOURCE=.\Appn\undo.c
# End Source File
# Begin Source File

SOURCE=.\Appn\util.c
# End Source File
# Begin Source File

SOURCE=.\Appn\view_pd.c
# End Source File
# End Group
# Begin Group "Analysis"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Analysis\BaseTimeEstimate.c
# End Source File
# Begin Source File

SOURCE=.\Analysis\CutSetsByAlgebraicMethod.c
# End Source File
# Begin Source File

SOURCE=.\Analysis\CutSetsByComboMethod.c
# End Source File
# Begin Source File

SOURCE=.\Analysis\CutSetsCommon.c
# End Source File
# Begin Source File

SOURCE=.\Analysis\FailureStatistics.c
# End Source File
# Begin Source File

SOURCE=.\Analysis\MonteCarloSimulation.c
# End Source File
# Begin Source File

SOURCE=.\Analysis\NormalisedBooleanExpressions.c
# End Source File
# Begin Source File

SOURCE=.\Analysis\ProbabilityAnalysis.c
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Utilities\AssertUtil.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\Block.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\FileUtilities.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\list_suu.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\list_suu_utilities.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\MemoryUtil.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\RandomNumbers.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\Record_Io.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\StatisticalMaths.c
# End Source File
# Begin Source File

SOURCE=.\Utilities\StringUtilities.c
# End Source File
# End Group
# Begin Group "Canvas"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Canvas\Canvas.c
# End Source File
# Begin Source File

SOURCE=.\Canvas\JavaCanvas.c
# End Source File
# End Group
# Begin Group "Primary Events"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PrimaryEvents\PED_shell.c
# End Source File
# Begin Source File

SOURCE=.\PrimaryEvents\primary_event.c
# End Source File
# Begin Source File

SOURCE=.\PrimaryEvents\primary_events_database.c
# End Source File
# End Group
# Begin Group "Widgets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Widgets\drawingarea.c
# End Source File
# End Group
# Begin Group "JNI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\JNI\NativeCutSetsDialog.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativeFTACanvas.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativeFTAFrame.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativeGateDialog.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativeIntermediateEventDialog.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativeMonteCarloDialog.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativeNumericalProbabilityDialog.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativePEDFrame.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativeTransferTreeDialog.c
# End Source File
# Begin Source File

SOURCE=.\JNI\NativeTreeTextDialog.c
# End Source File
# End Group
# End Target
# End Project
