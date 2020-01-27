// SettingBox.cpp : implementation file
//

#include "stdafx.h"
#include <atlbase.h>
#include "ECHO_TCP_IP.h"
#include "ECHO_TCP_IPDlg.h"
#include "SettingBox.h"
#include "RegKeyAdv.h"
#include <Shlwapi.h>

#ifdef _DEBUG

	#define new DEBUG_NEW

#undef THIS_FILE

	static char THIS_FILE[] = __FILE__;

#endif


#define CHAT_PORT 3000
#define MAX_FONT_SIZE 700
#define MIN_FONT_SIZE 200

extern CECHO_TCP_IPDlg * dialog;
extern CString _S;

void WizSelectionCharFormat(CRichEditCtrl & rich, CHARFORMAT & charformat);

bool jbuttons;

CSettingBox::CSettingBox(CWnd* pParent /*=NULL*/)
    : CDialog(CSettingBox::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSettingBox)
    m_greeting = _T("");
    m_nick_name = _T("");
    m_connect_on_start = FALSE;
    m_run_server_on_start = FALSE;
    m_port = 0;
    m_font_face = _T("");
    m_load_on_system_start = FALSE;
    m_sound_enable = FALSE;
    m_ip_server = _T("");
    m_font_size = 0;
    m_no_autoscroll = FALSE;
    m_system_message = FALSE;
    m_auto_clear = FALSE;
    m_auto_chat_show = FALSE;
    m_enable_write_log_file = FALSE;
    //}}AFX_DATA_INIT
    TryConnect=false;
}


void CSettingBox::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSettingBox)
    DDX_Control(pDX, IDC_SLIDER_FONT_SIZE, m_slider_font_size);
    DDX_Control(pDX, IDC_NICK_NAME, m_nick_win);
    DDX_Control(pDX, IDC_SERVER_STOP, m_server_stop);
    DDX_Control(pDX, IDC_PORT, m_port_edit);
    DDX_Control(pDX, IDC_CONNECT, m_connect);
    DDX_Control(pDX, IDC_SERVER_RUN, m_run_server);
    DDX_Control(pDX, IDC_COLOR_BOX, m_color_box);
    DDX_Text(pDX, IDC_GREETING, m_greeting);
    DDX_Text(pDX, IDC_NICK_NAME, m_nick_name);
    DDV_MaxChars(pDX, m_nick_name, 16);
    DDX_Check(pDX, IDC_CONNECT_ON_START, m_connect_on_start);
    DDX_Check(pDX, IDC_RUN_SERVER_ON_START, m_run_server_on_start);
    DDX_Text(pDX, IDC_PORT, m_port);
    DDX_Text(pDX, IDC_COLOR_BOX, m_font_face);
    DDX_Check(pDX, IDC_LOAD_ON_SYSTEM_START, m_load_on_system_start);
    DDX_Check(pDX, IDC_SOUND_ENABLE, m_sound_enable);
    DDX_Text(pDX, IDC_IP_SERVER, m_ip_server);
    DDX_Text(pDX, IDC_FONT_SIZE, m_font_size);
    DDX_Check(pDX, IDC_NO_AUTO_SCROLL, m_no_autoscroll);
    DDX_Check(pDX, IDC_SYSTEM_MESSAGE, m_system_message);
    DDX_Check(pDX, IDC_AUTO_CLEAR, m_auto_clear);
    DDX_Check(pDX, IDC_AUTO_CHAT_SHOW, m_auto_chat_show);
    DDX_Check(pDX, IDC_ENABLE_WRITE_LOG_FILE, m_enable_write_log_file);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingBox, CDialog)
    //{{AFX_MSG_MAP(CSettingBox)
    ON_BN_CLICKED(IDC_SELECT_COLOR, OnSelectColor)
    ON_BN_CLICKED(IDC_SELECT_FONT, OnSelectFont)
    ON_BN_CLICKED(IDC_CONNECT, OnConnect)
    ON_BN_CLICKED(IDC_SERVER_RUN, OnServerRun)
    ON_BN_CLICKED(IDC_CONNECT_ON_START, OnConnectOnStart)
    ON_BN_CLICKED(IDC_RUN_SERVER_ON_START, OnRunServerOnStart)
    ON_BN_CLICKED(IDC_SERVER_STOP, OnServerStop)
    ON_EN_UPDATE(IDC_FONT_SIZE, OnUpdateFontSize)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FONT_SIZE, OnCustomdrawSliderFontSize)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//применить
void CSettingBox::OnOK()
{
    if (!UpdateData()) return;

    if (m_port==0 || m_port>65535)
    {
        MessageBox("Номер порта должен быть в диапазоне 1..65535", "Исправте значение");

        m_port_edit.SetFocus();
        m_port_edit.SetSel(0,-1);
        return;
    }


    m_cremote_ip=m_ip_server;

    if (m_font_size<MIN_FONT_SIZE) m_font_size=MIN_FONT_SIZE;
    if (m_font_size>MAX_FONT_SIZE) m_font_size=MAX_FONT_SIZE;

    PutData;

    WriteSettings();

    if (m_load_on_system_start == false)
    {

        CRegKey reg;
        reg.Create(HKEY_CURRENT_USER,
                   "Software\\Microsoft\\Windows\\CurrentVersion\\Run");

        reg.DeleteValue("TCP_IP_CHAT");

        reg.Close();

    }

    CDialog::OnOK();
}

bool CSettingBox::LoadSettings()
{
    CRegKeyAdv reg;
    //reg.Create(HKEY_CURRENT_USER, RegistryBase);
    reg.QueryValue(MyColor, "Color");
    if (!MyColor) MyColor=RGB(0,255,255);

    m_nick_name=reg.QueryString("NickName");
    if (m_nick_name.IsEmpty()) m_nick_name="Guest";
    m_greeting=reg.QueryString("Greeting");
//	if (m_greeting.IsEmpty()) m_greeting="Hello, All!";

    m_font_face=reg.QueryString("FontFace");
    if (m_font_face.IsEmpty()) m_font_face="Tahoma";

    DWORD tmp;
    reg.QueryValue(tmp,"FontSize");
    m_font_size=tmp;
    if (m_font_size<MIN_FONT_SIZE) m_font_size=MIN_FONT_SIZE;
    if (m_font_size>MAX_FONT_SIZE) m_font_size=MAX_FONT_SIZE;

    m_cremote_ip=reg.QueryString("IpAddrString");

    reg.QueryValue(tmp,"ConnectOnStart");
    m_connect_on_start=tmp;

    reg.QueryValue(tmp,"RunServerOnStart");
    m_run_server_on_start=tmp;

    reg.QueryValue(tmp,"LoadOnSystemStart");
    m_load_on_system_start=tmp;

    reg.QueryValue(tmp,"SoundEnable");
    m_sound_enable=tmp;

    reg.QueryValue(m_port,"Port");
    if (!m_port) m_port=CHAT_PORT;

    reg.QueryValue(tmp,"SystemMessage");
    m_system_message=tmp;

    reg.QueryValue(tmp,"NoAutoScroll");
    m_no_autoscroll=tmp;

    reg.QueryValue(tmp,"AutoClear");
    m_auto_clear=tmp;

    reg.QueryValue(tmp, "AutoShow");
    m_auto_chat_show=tmp;

    reg.QueryValue(tmp, "EnableWriteLogFile");
    m_enable_write_log_file=tmp;


    reg.Close();

    WriteSettings();

    return true;
}

bool CSettingBox::WriteSettings()
{
    //GetData;
    CRegKeyAdv reg;
    //m_ip_server.GetAddress(ipAddr);
    //reg.Open(HKEY_CURRENT_USER, RegistryBase);
    reg.SetValue(MyColor, "Color");
    reg.SetString(m_nick_name,"NickName");
    reg.SetString(m_greeting, "Greeting");
    reg.SetString(m_font_face, "FontFace");
    reg.SetValue(m_font_size, "FontSize");
    reg.SetString(m_cremote_ip, "IpAddrString");
    reg.SetValue(m_connect_on_start,"ConnectOnStart");
    reg.SetValue(m_run_server_on_start,"RunServerOnStart");
    reg.SetValue(m_port,"Port");
    reg.SetValue(m_load_on_system_start,"LoadOnSystemStart");
    reg.SetValue(m_sound_enable,"SoundEnable");
    reg.SetValue(m_system_message,"SystemMessage");
    reg.SetValue(m_no_autoscroll,"NoAutoscroll");
    reg.SetValue(m_auto_clear, "AutoClear");
    reg.SetValue(m_auto_chat_show, "AutoShow");
    reg.SetValue(m_enable_write_log_file, "EnableWriteLogFile");

    return true;
}

void CSettingBox::OnSelectColor()
{
    // TODO: Add your control notification handler code here
    CColorDialog clr;
    clr.m_cc.rgbResult=MyColor;
    clr.m_cc.Flags|=CC_RGBINIT;
    clr.DoModal();

    if (clr.GetColor() ==0) MessageBox("Этот цвет не может быть выбран!");
    else
    {
        MyColor=clr.GetColor();
        SetColorBox();
    }
    //Info_Exchange();

}

void CSettingBox::OnSelectFont()
{
    CFontDialog font;

//	LOGFONT * lf=new LOGFONT;
    LOGFONT lf= {0}; //new LOGFONT;
//	ZeroMemory(lf, sizeof(LOGFONT));


//	lf.lfCharSet=RUSSIAN_CHARSET;
//	lf.lfPitchAndFamily=VARIABLE_PITCH|FF_ROMAN;

    CHOOSEFONT & cf=font.m_cf;

    cf.lStructSize=sizeof(CHOOSEFONT);
    cf.lpLogFont=&lf;
    cf.iPointSize=m_font_size;
    cf.Flags|=CF_INITTOLOGFONTSTRUCT;
    cf.Flags&=~CF_EFFECTS;
    cf.Flags&=~CF_BOTH;
    cf.Flags|=CF_SCREENFONTS;
    cf.Flags|=CF_NOSCRIPTSEL;
    cf.nFontType=SCREEN_FONTTYPE;

    StrCpyN(lf.lfFaceName, m_font_face,32);
    //StrCpyN(lf->lfFaceName, "Хай. Hello,32);
    lf.lfHeight=m_font_size/10;


    if (font.DoModal() == IDOK)
    {
        m_font_face=font.GetFaceName();
        //m_font_size=font.GetSize();
        PutData;
        SetColorBox();
    }

    //delete lf;


}

/*void CSettingBox::InfoSend(CClientSocket *ConnectedSocket)
{
	//посылаем на удаленный компьютер
	GetData;
	INFO_EXCHANGE info;
	info.cbSize=sizeof(info);
	StrCpyN(info.fontface,m_font_face.GetBuffer(0), info.FONTFACE_MAX);
	StrCpyN(info.nick, m_nick_name.GetBuffer(0), info.NICK_MAX);
	info.fontsize=m_font_size;
	info.color=MyColor;
	BYTE esc=info.ESCAPE_CODE;
	if (ConnectedSocket)
	{
		ConnectedSocket->Send(&esc,1);
		ConnectedSocket->Send(&info, sizeof(info));
	}

  pragma

}*/

BOOL CSettingBox::OnInitDialog()
{
    CDialog::OnInitDialog();

    GetData;

    m_ip_server=m_cremote_ip;

    SetColorBox();

    m_slider_font_size.SetRange(200,700,true);
    m_slider_font_size.SetPos(m_font_size);

    PutData;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

//соединиться
void CSettingBox::OnConnect()
{
    GetData;

    m_cremote_ip=m_ip_server;


    if (dialog->OnConnect())
    {
        TryConnect=true;
        m_connect.EnableWindow(false);
        m_run_server.EnableWindow(false);
        m_nick_win.EnableWindow(false);

    }

}

CUserSettings CSettingBox::GetUserSettings()
{
    CUserSettings set;
    set.cf.cbSize=sizeof(set);
    set.cf.yHeight=m_font_size;
    set.cf.crTextColor=MyColor;
    set.cf.bCharSet=DEFAULT_CHARSET;

    set.cf.dwMask=
        CFM_COLOR	|
        CFM_SIZE	|
        CFM_CHARSET |
        CFM_FACE
        ;
    set.cf.bPitchAndFamily=VARIABLE_PITCH;
    StrCpyN(set.cf.szFaceName, m_font_face, LF_FACESIZE);

    StrCpyN(set.NickName,m_nick_name, set.NMAX);
    return set;
}

void CSettingBox::OnServerRun()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    dialog->OnRunServer();
}

void CSettingBox::OnConnectOnStart()
{
    // TODO: Add your control notification handler code here
    GetData;
    if (m_connect_on_start && m_run_server_on_start)
        m_run_server_on_start=false;

    PutData;
}

void CSettingBox::OnRunServerOnStart()
{
    // TODO: Add your control notification handler code here

    GetData;
    if (m_connect_on_start && m_run_server_on_start)
        m_connect_on_start=false;
    PutData;

}

void CSettingBox::OnServerStop()
{
    // TODO: Add your control notification handler code here
    dialog->OnMenuStopServer();

}

//горят кнопки запуск сервера, соединится
//RunServer==1 запущен сервер
//IsConnected==1 если подключены к другому
void CSettingBox::InitButtons(bool RunServer,bool IsConnected)
{
    //если сервер запущен
    if (RunServer)
    {
        m_run_server.EnableWindow(false);
        m_server_stop.EnableWindow(true);
        m_connect.EnableWindow(false);
        m_port_edit.EnableWindow(false);
    }
    else
    {
        m_run_server.EnableWindow(true);
        m_server_stop.EnableWindow(false);
        m_connect.EnableWindow(true);
        m_port_edit.EnableWindow(true);
    }

    //если есть соединение
    if (IsConnected)
    {
        m_run_server.EnableWindow(false);
        if (!RunServer)m_server_stop.EnableWindow(false);
        m_connect.EnableWindow(false);
        m_port_edit.EnableWindow(false);
        m_nick_win.EnableWindow(false);
    }
    //иначе нет соидинения
    else if (!IsConnected && !RunServer)
    {
        m_connect.EnableWindow(true);
        m_nick_win.EnableWindow(true);
    }

    if (TryConnect)
    {
        m_connect.EnableWindow(false);
        m_run_server.EnableWindow(false);
    }
}

int CSettingBox::DoModal(bool jbut)
{
    jbuttons=jbut;

    return CDialog::DoModal();
}

void CSettingBox::SetColorBox()
{
    m_color_box.SetBackgroundColor(0,0);

    CHARFORMAT a= {0};
    //настраиваем системные настройки
    a.yHeight=m_font_size;
    a.crTextColor=MyColor;
    a.cbSize=sizeof(CHARFORMAT);
    
	strcpy(a.szFaceName,m_font_face);
 
	a.dwMask=
        CFM_COLOR	|
        CFM_SIZE	|
        CFM_FACE	|
        CFM_CHARSET
        ;
    
	a.bPitchAndFamily=VARIABLE_PITCH;
    
	//a.bCharSet=RUSSIAN_CHARSET;
    a.bCharSet=DEFAULT_CHARSET;

    m_color_box.SetWindowText("");
    
	WizSelectionCharFormat(m_color_box, a);

    CString v=m_font_face;//+" тест";
    //CString v=" тест";
    m_color_box.ReplaceSel(v);

}

void CSettingBox::OnUpdateFontSize()
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function to send the EM_SETEVENTMASK message to the control
    // with the ENM_UPDATE flag ORed into the lParam mask.

    // TODO: Add your control notification handler code here

    GetData;
    SetColorBox();
}

void CSettingBox::OnCustomdrawSliderFontSize(NMHDR* pNMHDR, LRESULT* pResult)
{
    GetData;

    int pos=m_slider_font_size.GetPos();

    m_font_size=pos;

    PutData;

    SetColorBox();

    *pResult = 0;
}
