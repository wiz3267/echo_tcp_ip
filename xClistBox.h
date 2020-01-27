#if !defined(AFX_XCLISTBOX_H__7A85875D_E370_46E3_87A4_EC6E149A2984__INCLUDED_)
#define AFX_XCLISTBOX_H__7A85875D_E370_46E3_87A4_EC6E149A2984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xClistBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// xClistBox window

class xClistBox : public CListBox
{
// Construction
public:
    xClistBox();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(xClistBox)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~xClistBox();

    // Generated message map functions
protected:
    //{{AFX_MSG(xClistBox)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCLISTBOX_H__7A85875D_E370_46E3_87A4_EC6E149A2984__INCLUDED_)
