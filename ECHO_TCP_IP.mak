# Microsoft Developer Studio Generated NMAKE File, Based on ECHO_TCP_IP.dsp
!IF "$(CFG)" == ""
CFG=ECHO_TCP_IP - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ECHO_TCP_IP - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ECHO_TCP_IP - Win32 Release" && "$(CFG)" != "ECHO_TCP_IP - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ECHO_TCP_IP.mak" CFG="ECHO_TCP_IP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ECHO_TCP_IP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ECHO_TCP_IP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ECHO_TCP_IP - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\ECHO_TCP_IP_release.exe" "$(OUTDIR)\ECHO_TCP_IP.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutBox.obj"
	-@erase "$(INTDIR)\AboutBox.sbr"
	-@erase "$(INTDIR)\ECHO_TCP_IP.obj"
	-@erase "$(INTDIR)\ECHO_TCP_IP.pch"
	-@erase "$(INTDIR)\ECHO_TCP_IP.res"
	-@erase "$(INTDIR)\ECHO_TCP_IP.sbr"
	-@erase "$(INTDIR)\ECHO_TCP_IPDlg.obj"
	-@erase "$(INTDIR)\ECHO_TCP_IPDlg.sbr"
	-@erase "$(INTDIR)\SettingBox.obj"
	-@erase "$(INTDIR)\SettingBox.sbr"
	-@erase "$(INTDIR)\StatusDialog.obj"
	-@erase "$(INTDIR)\StatusDialog.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\xClistBox.obj"
	-@erase "$(INTDIR)\xClistBox.sbr"
	-@erase "$(OUTDIR)\ECHO_TCP_IP.bsc"
	-@erase ".\ECHO_TCP_IP_release.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ECHO_TCP_IP.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\ECHO_TCP_IP.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ECHO_TCP_IP.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutBox.sbr" \
	"$(INTDIR)\ECHO_TCP_IP.sbr" \
	"$(INTDIR)\ECHO_TCP_IPDlg.sbr" \
	"$(INTDIR)\SettingBox.sbr" \
	"$(INTDIR)\StatusDialog.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\xClistBox.sbr"

"$(OUTDIR)\ECHO_TCP_IP.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ECHO_TCP_IP_release.pdb" /machine:I386 /out:"ECHO_TCP_IP_release.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AboutBox.obj" \
	"$(INTDIR)\ECHO_TCP_IP.obj" \
	"$(INTDIR)\ECHO_TCP_IPDlg.obj" \
	"$(INTDIR)\SettingBox.obj" \
	"$(INTDIR)\StatusDialog.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\xClistBox.obj" \
	"$(INTDIR)\ECHO_TCP_IP.res"

".\ECHO_TCP_IP_release.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ECHO_TCP_IP - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ECHO_TCP_IP.exe" "$(OUTDIR)\ECHO_TCP_IP.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutBox.obj"
	-@erase "$(INTDIR)\AboutBox.sbr"
	-@erase "$(INTDIR)\ECHO_TCP_IP.obj"
	-@erase "$(INTDIR)\ECHO_TCP_IP.pch"
	-@erase "$(INTDIR)\ECHO_TCP_IP.res"
	-@erase "$(INTDIR)\ECHO_TCP_IP.sbr"
	-@erase "$(INTDIR)\ECHO_TCP_IPDlg.obj"
	-@erase "$(INTDIR)\ECHO_TCP_IPDlg.sbr"
	-@erase "$(INTDIR)\SettingBox.obj"
	-@erase "$(INTDIR)\SettingBox.sbr"
	-@erase "$(INTDIR)\StatusDialog.obj"
	-@erase "$(INTDIR)\StatusDialog.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\xClistBox.obj"
	-@erase "$(INTDIR)\xClistBox.sbr"
	-@erase "$(OUTDIR)\ECHO_TCP_IP.bsc"
	-@erase "$(OUTDIR)\ECHO_TCP_IP.exe"
	-@erase "$(OUTDIR)\ECHO_TCP_IP.ilk"
	-@erase "$(OUTDIR)\ECHO_TCP_IP.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ECHO_TCP_IP.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\ECHO_TCP_IP.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ECHO_TCP_IP.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutBox.sbr" \
	"$(INTDIR)\ECHO_TCP_IP.sbr" \
	"$(INTDIR)\ECHO_TCP_IPDlg.sbr" \
	"$(INTDIR)\SettingBox.sbr" \
	"$(INTDIR)\StatusDialog.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\xClistBox.sbr"

"$(OUTDIR)\ECHO_TCP_IP.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ECHO_TCP_IP.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ECHO_TCP_IP.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AboutBox.obj" \
	"$(INTDIR)\ECHO_TCP_IP.obj" \
	"$(INTDIR)\ECHO_TCP_IPDlg.obj" \
	"$(INTDIR)\SettingBox.obj" \
	"$(INTDIR)\StatusDialog.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\xClistBox.obj" \
	"$(INTDIR)\ECHO_TCP_IP.res"

"$(OUTDIR)\ECHO_TCP_IP.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ECHO_TCP_IP.dep")
!INCLUDE "ECHO_TCP_IP.dep"
!ELSE 
!MESSAGE Warning: cannot find "ECHO_TCP_IP.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ECHO_TCP_IP - Win32 Release" || "$(CFG)" == "ECHO_TCP_IP - Win32 Debug"
SOURCE=.\AboutBox.cpp

"$(INTDIR)\AboutBox.obj"	"$(INTDIR)\AboutBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ECHO_TCP_IP.pch"


SOURCE=.\ECHO_TCP_IP.cpp

"$(INTDIR)\ECHO_TCP_IP.obj"	"$(INTDIR)\ECHO_TCP_IP.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ECHO_TCP_IP.pch"


SOURCE=.\ECHO_TCP_IP.rc

"$(INTDIR)\ECHO_TCP_IP.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ECHO_TCP_IPDlg.cpp

"$(INTDIR)\ECHO_TCP_IPDlg.obj"	"$(INTDIR)\ECHO_TCP_IPDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ECHO_TCP_IP.pch"


SOURCE=.\SettingBox.cpp

"$(INTDIR)\SettingBox.obj"	"$(INTDIR)\SettingBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ECHO_TCP_IP.pch"


SOURCE=.\StatusDialog.cpp

"$(INTDIR)\StatusDialog.obj"	"$(INTDIR)\StatusDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ECHO_TCP_IP.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ECHO_TCP_IP - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ECHO_TCP_IP.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ECHO_TCP_IP.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ECHO_TCP_IP - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ECHO_TCP_IP.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ECHO_TCP_IP.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\xClistBox.cpp

"$(INTDIR)\xClistBox.obj"	"$(INTDIR)\xClistBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ECHO_TCP_IP.pch"



!ENDIF 

