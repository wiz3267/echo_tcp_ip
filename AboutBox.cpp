// AboutBox.cpp : implementation file
#include "stdafx.h"
#include "ECHO_TCP_IP.h"
#include "AboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AboutBox dialog


AboutBox::AboutBox(CWnd* pParent /*=NULL*/)
    : CDialog(AboutBox::IDD, pParent)
{
    //{{AFX_DATA_INIT(AboutBox)
    m_info = _T("");
    //}}AFX_DATA_INIT
}

void AboutBox::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(AboutBox)
    DDX_Text(pDX, IDC_INFO, m_info);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(AboutBox, CDialog)
    //{{AFX_MSG_MAP(AboutBox)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AboutBox message handlers




void AboutBox::OnOK()
{

    CDialog::OnOK();
}

extern char __MAIN__DATE__[];
extern char __MAIN__TIME__[];

BOOL AboutBox::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here

//	CString str;
//	LoadString(PRODUCTVERSION, str);

    m_info=CString("Дата создания: ")+CString(__MAIN__DATE__)+", "+__MAIN__TIME__;

    UpdateData(false);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
