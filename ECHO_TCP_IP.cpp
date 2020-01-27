// ECHO_TCP_IP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ECHO_TCP_IP.h"
#include "ECHO_TCP_IPDlg.h"

#include <Afxsock.h>
#include <shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECHO_TCP_IPApp

BEGIN_MESSAGE_MAP(CECHO_TCP_IPApp, CWinApp)
    //{{AFX_MSG_MAP(CECHO_TCP_IPApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECHO_TCP_IPApp construction

CECHO_TCP_IPApp::CECHO_TCP_IPApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CECHO_TCP_IPApp object

CECHO_TCP_IPApp theApp;

HCURSOR hCursor;

/////////////////////////////////////////////////////////////////////////////
// CECHO_TCP_IPApp initialization

BOOL CECHO_TCP_IPApp::InitInstance()
{

    AfxEnableControlContainer();

    AfxSocketInit();

    AfxInitRichEdit();
    //LoadLibrary("c:\\win98\\riched.dll");


    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

#ifdef _AFXDLL
    Enable3dControls();			// Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    SECURITY_ATTRIBUTES sec;
    sec.bInheritHandle=true;
    sec.lpSecurityDescriptor=NULL;
    sec.nLength=sizeof(sec);
    char *name="ECHOTCPIP";
    HANDLE event=OpenEvent(EVENT_ALL_ACCESS,false,name);
    /*	if (event==NULL)
    	{
    		CreateEvent(NULL,0,0,name);
    	}
    	else {
    		MessageBox(NULL,"ЧАТ УЖЕ ЗАПУЩЕН!\r\nДля активации чата щелкните по иконке на системном трее\r\n","В Н И М А Н И Е !",MB_OK);
    		//SetEvent(event);
    		return 123;
    	}
    */

    CECHO_TCP_IPDlg dlg;

    LPSTR buf=GetCommandLine();

    CString command=buf;
    if(command.Find("hide")>0) dlg.m_hide=true;

    dlg.hIcon=LoadIcon("ICON_SYSTRAY");
    //WARNING!!!
    m_pMainWnd = NULL;
    int nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
//		ExitWindowsEx(
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

//	AfxMessageBox("EXIT APPLICATION");
    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}
