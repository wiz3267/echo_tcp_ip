// xClistBox.cpp : implementation file
//

#include "stdafx.h"
#include "echo_tcp_ip.h"
#include "xClistBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// xClistBox

xClistBox::xClistBox()
{
}

xClistBox::~xClistBox()
{
}


BEGIN_MESSAGE_MAP(xClistBox, CListBox)
    //{{AFX_MSG_MAP(xClistBox)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// xClistBox message handlers

HBRUSH xClistBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);

    HBRUSH hbr=CreateSolidBrush(RGB(0,255,0));

    // TODO: Change any attributes of the DC here

    // TODO: Return a different brush if the default is not desired
    return hbr;
}
