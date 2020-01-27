; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CECHO_TCP_IPDlg
LastTemplate=CListBox
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "echo_tcp_ip.h"
LastPage=0

ClassCount=7
Class1=AboutBox
Class2=CECHO_TCP_IPApp
Class3=CAboutDlg
Class4=CECHO_TCP_IPDlg
Class5=CSettingBox
Class6=CStatusDialog

ResourceCount=7
Resource1=IDD_ECHO_TCP_IP_DIALOG
Resource2=IDD_DIALOG1
Resource3=IDD_STATUS_DIALOG
Resource4=IDD_ABOUT_BOX
Resource5=IDD_ABOUTBOX (English (U.S.))
Class7=xClistBox
Resource6=IDD_SETTINGBOX
Resource7=IDR_MAIN_MENU

[CLS:AboutBox]
Type=0
BaseClass=CDialog
HeaderFile=AboutBox.h
ImplementationFile=AboutBox.cpp
LastObject=AboutBox

[CLS:CECHO_TCP_IPApp]
Type=0
BaseClass=CWinApp
HeaderFile=ECHO_TCP_IP.h
ImplementationFile=ECHO_TCP_IP.cpp
LastObject=CECHO_TCP_IPApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ECHO_TCP_IPDlg.cpp
ImplementationFile=ECHO_TCP_IPDlg.cpp
LastObject=CAboutDlg
Filter=D
VirtualFilter=dWC

[CLS:CECHO_TCP_IPDlg]
Type=0
BaseClass=CDialog
HeaderFile=ECHO_TCP_IPDlg.h
ImplementationFile=ECHO_TCP_IPDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_INPUT

[CLS:CSettingBox]
Type=0
BaseClass=CDialog
HeaderFile=SettingBox.h
ImplementationFile=SettingBox.cpp
Filter=D
VirtualFilter=dWC
LastObject=CSettingBox

[CLS:CStatusDialog]
Type=0
BaseClass=CDialog
HeaderFile=StatusDialog.h
ImplementationFile=StatusDialog.cpp

[DLG:IDD_ABOUT_BOX]
Type=1
Class=AboutBox
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_INFO,static,1342308352
Control5=IDC_STATIC,static,1342177283
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_ECHO_TCP_IP_DIALOG]
Type=1
Class=CECHO_TCP_IPDlg
ControlCount=4
Control1=IDC_INPUT,RICHEDIT,1350635716
Control2=IDC_RICHEDIT,RICHEDIT,1352673348
Control3=IDC_INPUT_BOX,edit,1350633600
Control4=IDC_USER_LIST,listbox,1352728913

[DLG:IDD_SETTINGBOX]
Type=1
Class=CSettingBox
ControlCount=42
Control1=IDOK,button,1342242817
Control2=IDC_CONNECT,button,1342242816
Control3=IDC_CONNECT_ON_START,button,1342242819
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_SELECT_COLOR,button,1342242816
Control7=IDC_SELECT_FONT,button,1342242816
Control8=IDC_FONT_SIZE,edit,1082196096
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_GREETING,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_NICK_NAME,edit,1350631552
Control15=IDC_STATIC,button,1342177287
Control16=IDC_COLOR_BOX,RICHEDIT,1350631556
Control17=IDC_STATIC,button,1342177287
Control18=IDC_RUN_SERVER_ON_START,button,1342242819
Control19=IDC_PORT,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_SERVER_RUN,button,1342242816
Control22=IDC_SERVER_STOP,button,1342242816
Control23=IDC_LOAD_ON_SYSTEM_START,button,1342242819
Control24=IDC_STATIC,button,1342177287
Control25=IDC_SOUND_ENABLE,button,1342242819
Control26=IDC_STATIC,button,1342177287
Control27=IDC_IP_SERVER,edit,1350631552
Control28=IDC_NO_AUTO_SCROLL,button,1342242819
Control29=IDC_SYSTEM_MESSAGE,button,1342242819
Control30=IDC_CHECK3,button,1476460547
Control31=IDC_AUTO_CLEAR,button,1342242819
Control32=IDC_AUTO_CHAT_SHOW,button,1342242819
Control33=IDC_STATIC,static,1342308352
Control34=IDC_EDIT1,edit,1350631584
Control35=IDC_SLIDER_FONT_SIZE,msctls_trackbar32,1342242840
Control36=IDC_CHECK4,button,1476460547
Control37=IDC_CHECK6,button,1476460547
Control38=IDC_enabled_chat_game,button,1476460547
Control39=IDC_STATIC,button,1342177287
Control40=IDC_CHECK8,button,1476460547
Control41=IDC_ENABLE_WRITE_LOG_FILE,button,1342242819
Control42=IDC_CONNECT_ABORT,button,1342242816

[DLG:IDD_STATUS_DIALOG]
Type=1
Class=CStatusDialog
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_EDIT1,edit,1350631556

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAIN_MENU]
Type=1
Class=?
Command1=IDC_CLEAR_LOG
Command2=ID_MENU_LOAD_LOG
Command3=ID_FILE_SEND
Command4=ID_FILE_EXIT
Command5=ID_MENU_RUN_SERVER
Command6=ID_MENU_STOP_SERVER
Command7=IDC_DISCONNECT
Command8=IDC_CONNECT
Command9=ID_MENU_SETTING_BOX
Command10=ID_ABOUT
CommandCount=10

[CLS:xClistBox]
Type=0
HeaderFile=xClistBox.h
ImplementationFile=xClistBox.cpp
BaseClass=CListBox
Filter=W
VirtualFilter=bWC
LastObject=ID_ABOUT

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

