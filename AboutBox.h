#if !defined(AFX_ABOUTBOX_H__481D1888_F6FA_4B7F_B006_414C113F3141__INCLUDED_)
#define AFX_ABOUTBOX_H__481D1888_F6FA_4B7F_B006_414C113F3141__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AboutBox dialog

class AboutBox : public CDialog
{
// Construction
public:
    AboutBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(AboutBox)
    enum { IDD = IDD_ABOUT_BOX };
    CString	m_info;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(AboutBox)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(AboutBox)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTBOX_H__481D1888_F6FA_4B7F_B006_414C113F3141__INCLUDED_)
