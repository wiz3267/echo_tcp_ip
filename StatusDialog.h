#if !defined(AFX_STATUSDIALOG_H__18C2A69B_A44A_4E9E_A90B_704BD8C2196C__INCLUDED_)
#define AFX_STATUSDIALOG_H__18C2A69B_A44A_4E9E_A90B_704BD8C2196C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog dialog

class CStatusDialog : public CDialog
{
// Construction
public:
    CStatusDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CStatusDialog)
    enum { IDD = IDD_STATUS_DIALOG };
    CString	m_status;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CStatusDialog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CStatusDialog)
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSDIALOG_H__18C2A69B_A44A_4E9E_A90B_704BD8C2196C__INCLUDED_)
