// ECHO_TCP_IP.h : main header file for the ECHO_TCP_IP application
//

#if !defined(AFX_ECHO_TCP_IP_H__0BA1A175_2D54_42EE_8E42_46CE66A526C3__INCLUDED_)
#define AFX_ECHO_TCP_IP_H__0BA1A175_2D54_42EE_8E42_46CE66A526C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CECHO_TCP_IPApp:
// See ECHO_TCP_IP.cpp for the implementation of this class
//

#define GetData UpdateData(true)
#define PutData UpdateData(false)

class CECHO_TCP_IPApp : public CWinApp
{
public:
    CECHO_TCP_IPApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CECHO_TCP_IPApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CECHO_TCP_IPApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECHO_TCP_IP_H__0BA1A175_2D54_42EE_8E42_46CE66A526C3__INCLUDED_)
