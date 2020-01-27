// StatusDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ECHO_TCP_IP.h"
#include "StatusDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog dialog


CStatusDialog::CStatusDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CStatusDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CStatusDialog)
    m_status = _T("");
    //}}AFX_DATA_INIT
}


void CStatusDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CStatusDialog)
    DDX_Text(pDX, IDC_EDIT1, m_status);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusDialog, CDialog)
    //{{AFX_MSG_MAP(CStatusDialog)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog message handlers

void CStatusDialog::OnOK()
{
    // TODO: Add extra validation here

}
