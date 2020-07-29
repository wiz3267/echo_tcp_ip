// ECHO_TCP_IPDlg.cpp : implementation file
//

#include "stdafx.h"
#include <shellapi.h>
#include <Afxsock.h>
#include <atlbase.h>
#include <vector>

#include <mmsystem.h>
#include "ECHO_TCP_IP.h"
#include "ECHO_TCP_IPDlg.h"
#include "RegKeyAdv.h"
#include "sockets.h"
#include "aboutbox.h"
#include "statusdialog.h"

CString ENDL="\r\n";

CString	PathTextLog,PathRtfLog;

CFile RtfFile;

CString _S="";

int ShowSendRecvInfo=false;

const int LIST_ITEM_HEIGHT=30;
const UINT MAX_MESSAGE_LEN=1024;

int INPUT_HEI=300;

const BYTE RING_SYMBOL='Ё';	//символ, по принятии которого звучит "вызов"
const BYTE RING_SYMBOL2='Й';	//символ, по принятии которого звучит "вызов"

char __MAIN__DATE__[]=__DATE__;
char __MAIN__TIME__[]=__TIME__;

const CString CopyRightString="TCP/IP Chat. (c) 2005 - 2010 Евгений Лапшин";

#define	ID_POPUP_SHOW	0xF001
#define	ID_POPUP_EXIT	0xF002

//COLORREF BackGroundColor=RGB(255,255,255);
COLORREF BackGroundColor=RGB(20,20,20);

CUserSettings
				LocalSettings,	//наши настройки
				SystemSettings,	//системные настройки
				HistorySettings;//настройки для показа истории

DWORD UnID=0;

/*

  typedef struct _charformat2 {
  UINT        cbSize;
  DWORD       dwMask;
  DWORD       dwEffects;
  LONG        yHeight;
  LONG        yOffset;
  COLORREF    crTextColor;
  BYTE        bCharSet;
  BYTE        bPitchAndFamily;
  TCHAR       szFaceName[LF_FACESIZE];
  WORD        wWeight;
  SHORT       sSpacing;
  COLORREF    crBackColor;
  LCID        lcid;
  DWORD       dwReserved;
  SHORT       sStyle;
  WORD        wKerning;
  BYTE        bUnderlineType;
  BYTE        bAnimation;
  BYTE        bRevAuthor;
  BYTE        bReserved1;
} CHARFORMAT2;


  typedef struct _charformat { 
  UINT     cbSize; 
  DWORD    dwMask; 
  DWORD    dwEffects; 
  LONG     yHeight; 
  LONG     yOffset; 
  COLORREF crTextColor; 
  BYTE     bCharSet; 
  BYTE     bPitchAndFamily; 
  TCHAR    szFaceName[LF_FACESIZE]; 
} CHARFORMAT; 

  */


void WizSelectionCharFormat(CRichEditCtrl & rich, CHARFORMAT & charformat)
{
        CHARFORMAT ch=charformat;

        
		CHARFORMAT2 ch2;

		ch2.cbSize=0;
		ch2.dwMask=  
					CFM_COLOR	|
					CFM_SIZE	|
					CFM_FACE	|
					CFM_CHARSET;

		ch2.dwEffects=0;
		ch2.yHeight=0;
		ch2.yOffset=0;
		ch2.crTextColor=0;
		ch2.bCharSet=0;
		ch2.bPitchAndFamily=0;

		//TCHAR       szFaceName[LF_FACESIZE];

		ch2.wWeight=0;
		ch2.sSpacing=0;
		ch2.crBackColor=0;
		ch2.lcid=0;
		ch2.dwReserved=0;
		ch2.sStyle=0;
		ch2.wKerning=0;
		ch2.bUnderlineType=0;
		ch2.bAnimation=0;
		ch2.bRevAuthor=0;
//		ch2.bReserved1=0;


		ch2.cbSize=sizeof(ch2);
		//ch2.dwMask=ch.dwMask;
		ch2.dwEffects=ch.dwEffects;
		ch2.yHeight=ch.yHeight;
		ch2.yOffset=ch.yOffset;
		ch2.crTextColor=ch.crTextColor;
		
		ch2.bCharSet=DEFAULT_CHARSET;

		
		ch2.bPitchAndFamily=ch.bPitchAndFamily;
		strcpy(ch2.szFaceName, ch.szFaceName);
		
		ch2.crBackColor=RGB(0,255,0);

		LOGFONT lf= {0};
        lf.lfCharSet=ch.bCharSet;
        lf.lfHeight=LIST_ITEM_HEIGHT;
        lf.lfWidth=10;
        lf.lfPitchAndFamily=ch.bPitchAndFamily;
        lf.lfCharSet=DEFAULT_CHARSET;

		rich.SetSelectionCharFormat(ch2);

}

//переводит число в CString
CString itos(int n)
{
    CString m;
    m.Format("%i",n);
    return m;
}

static DWORD CALLBACK
MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
    CFile* pFile = (CFile*) dwCookie;

    pFile->Write(pbBuff, cb);
    *pcb = cb;

    return 0;
}


//возвращает истину если все символы строки пробельные
bool AllSpace(CString &s)
{
    for(int i=0; i<s.GetLength(); i++)
    {
        if (s[i]!=' ') return false;
    }

    return true;
}

//bool need_show_cursor=true;
bool need_invalidate=false;

CString RegistryBase="Software\\TcpIpChat\\";

#define NO_NEW_LINE false
#define NEW_LINE true

#define PutData UpdateData(false)
#define GetData UpdateData(true)

CECHO_TCP_IPDlg* dialog=NULL;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatusDialog * StatusDialog;

#define STOP_SOUND 1
#define SOUND_STOP 1

CString GetLocalTimeString()
{
    SYSTEMTIME t;
    GetLocalTime(&t);

    CString c;

    //c.Format("{%02i:%02i:%02i.%03i} ", t.wHour, t.wMinute, t.wSecond,t.wMilliseconds);
    c.Format("{%02i.%02i.%02i} [%02i:%02i.%02i] ", t.wDay, t.wMonth, t.wYear,
             t.wHour, t.wMinute, t.wSecond);

    return c;
}



struct CURSORINFO
{
    CRichEditCtrl* rich;
    
	char String[2];
    char sym;	//символ, используемый в качестве курсора
    int count;
    
	CURSORINFO()
    {
        sym='_';
        String[0]=sym;
        String[1]=0;
        count=0;
    }
    
	void Show()
    {
        String[0]=sym;
        count=0;
    }
    
	void Hide()
    {
        String[0]=' ';
        count=0;
    }

    void LinkRichEdit(CRichEditCtrl * r)
    {
        rich=r;
    }


    //стираем курсор
    void Erase()
    {
        //стираем последний символ в RichEdit'оре
        //rich->SendMessage(WM_KEYDOWN, VK_BACK,0);
    }

    //отрисовка курсора
    void Draw() {}
    /*	{
    			CHARFORMAT a={0};
    			//настраиваем системные настройки
    			a.cbSize=sizeof(CHARFORMAT);
    			a.yHeight=LocalSettings.cf.yHeight;
    			a.crTextColor=RGB(255,255,255);
    			//strcpy(a.szFaceName,"Terminal");
    			a.dwMask=
    						CFM_COLOR	|
    						CFM_SIZE	|
    						CFM_FACE
    					;
    			a.bPitchAndFamily=FIXED_PITCH;
    			a.bCharSet=DEFAULT_CHARSET;
    			

				

    			rich->ReplaceSel(Cursor.String);
    	}
    */


} Cursor;

CServerSocket* ServerSocket=NULL;

const DWORD InfoTimeOut=4000;	//время на получение информации от пользователя

//информация о присоединеных пользователях
struct CConnectedUserInfo
{
    CConnectedUserInfo() : CountInfoReceive(0), socket_connect(NULL) {}
    CClientSocket* socket_connect;	//сокет удаленного пользователя
    CString ip;	//ip адрес
    CString domainaddr; //доменное имя

    DWORD unicalid;
    DWORD startime;	//время подключения
    CUserSettings RemoteSettings;	//настройка удаленного пользователя
    void SendInfo(CUserSettings &info)
    {
        BYTE buf=ESCAPE_CODE;

        int size=1+sizeof(info);
        BYTE *bufer=new BYTE[size];
        bufer[0]=ESCAPE_CODE;
        CopyMemory(bufer+1,&info,sizeof(info));

        socket_connect->SendData(bufer,size);

        delete bufer;

        if (ShowSendRecvInfo)
        {
            CString m;
            m.Format("Send info для sock=%i ",DWORD(socket_connect));
            m+=_S+RemoteSettings.NickName+"("+itos(RemoteSettings.uid)+")";
            m+=_S+" о "+info.NickName+"("+itos(info.uid)+")"+ENDL;
            dialog->ChatWindowAddStringUID(m,SYSTEM_UID);
        }
    }
    int CountInfoReceive;

    CString RecvString;	//принимаемые текстовые данные

    bool	EchoToClient;//=false;
    bool	ConvertOemToAnsi;//=false;

};

std::vector<CConnectedUserInfo> UserInfo;


enum
{
	SOUND_BLA,
	SOUND_MLA,
	SOUND_CLICK,
	SOUND_RING1,
	SOUND_RING2,
	SOUND_MESSAGE1,
	SOUND_MESSAGE2,
	SOUND_LOL1,
	SOUND_LOL2,
	SOUND_NEWUSER,
	SOUND_DISCONNECT,
	SOUND_SOUND1,
	SOUND_SOUND2,
	SOUND_SOUND3,
	SOUND_SOUND4,
	SOUND_SOUND5,
	SOUND_SOUND6
};



void CECHO_TCP_IPDlg::Sound(int type, int stop=false)
{
    if (!SettingBox.m_sound_enable) return;
    char *fname=NULL;
    switch (type)
    {

		case SOUND_BLA:
		{
			fname="bla.wav";
			break;
		}


		case SOUND_CLICK:
		{
			fname="click.wav";
			break;
		}

		case SOUND_RING1:
		{
			fname="ring1.wav";
			break;
		}

		case SOUND_RING2:
		{
			fname="ring2.wav";
			break;
		}


		case SOUND_MESSAGE1:
		{
			fname="message.wav";
			break;
		}
		
		case SOUND_MESSAGE2:
		{
			fname="message.wav";
			break;
		}


		case SOUND_LOL1:
		{

			fname="lol1.wav";
			break;
		}

		case SOUND_LOL2:
		{

			fname="lol2.wav";
			break;
		}


		case SOUND_NEWUSER:
		{
			fname="newuser.wav";
			break;
		}

		case SOUND_DISCONNECT:
		{
			fname="disconnect.wav";
			break;
		}


		case SOUND_SOUND1:
		{
			fname="sound1.wav";
			break;
		}

		case SOUND_SOUND2:
		{
			fname="sound2.wav";
			break;
		}
		case SOUND_SOUND3:
		{
			fname="sound3.wav";
			break;
		}
		case SOUND_SOUND4:
		{
			fname="sound4.wav";
			break;
		}
		case SOUND_SOUND5:
		{
			fname="sound5.wav";
			break;
		}
		case SOUND_SOUND6:
		{
			fname="sound6.wav";
			break;
		}


    }

    if (fname)
    {
        if (stop==1)	sndPlaySound(NULL,SND_ASYNC);
        sndPlaySound(_S+"Sound\\"+fname,SND_ASYNC | SND_NOSTOP);
    }
}

int GetUserIndex(DWORD sock)
{

    for(int i=0; i<UserInfo.size(); i++)
        if ( DWORD(UserInfo[i].socket_connect) ==sock) return i;

    //if (i== UserInfo.size())
    //potencial bug
    return -1;
}


//посылает строку всем пользователям
void send_all(const CString& m)
{

    if (UserInfo.size()==0) return;

    //если мы сервер
    if (ServerSocket)
    {

        //поочереди посылаем строку каждому пользователю из списка
        for(int i=0; i<UserInfo.size(); i++)
        {
            UserInfo[i].socket_connect->SendStringWithUID(m,0);
        }
    }

    //иначе мы клиент и посылаем строку серверу
    else
    {
        UserInfo[0].socket_connect->SendStringWithUID(m,UnID);
    }

}

//CClientSocket* ConnectedSocket=NULL;

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
public:
    virtual BOOL DestroyWindow();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    afx_msg void OnMenuLoadLog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    ON_COMMAND(ID_MENU_LOAD_LOG, OnMenuLoadLog)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECHO_TCP_IPDlg dialog

CECHO_TCP_IPDlg::CECHO_TCP_IPDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CECHO_TCP_IPDlg::IDD, pParent)
{
    dialog=this;
    //{{AFX_DATA_INIT(CECHO_TCP_IPDlg)
    m_richtext = _T(" ");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hide=false;

    CHARFORMAT &a=SystemSettings.cf;
    //настраиваем системные настройки
    a.yHeight=200;
    a.crTextColor=RGB(50,255,100);
    a.cbSize=sizeof(CHARFORMAT);
    strcpy(a.szFaceName,"System");
    //strcpy(a.szFaceName,"");
    a.dwMask=
        CFM_COLOR	|
        CFM_SIZE	|
        CFM_FACE	|
        CFM_CHARSET
        ;
    a.bPitchAndFamily=VARIABLE_PITCH;
    //a.bCharSet=RUSSIAN_CHARSET;
    a.bCharSet=DEFAULT_CHARSET;
    strcpy(SystemSettings.NickName, "System");

    CHARFORMAT b=SystemSettings.cf;
    b.crTextColor=RGB(240,200,250);

    HistorySettings.cf=b;

}

void CECHO_TCP_IPDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CECHO_TCP_IPDlg)
    DDX_Control(pDX, IDC_USER_LIST, m_user_list);
    DDX_Control(pDX, IDC_INPUT, m_input);
    DDX_Control(pDX, IDC_INPUT_BOX, m_input_box);
    DDX_Control(pDX, IDC_RICHEDIT, m_richedit);
    DDX_Text(pDX, IDC_RICHEDIT, m_richtext);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CECHO_TCP_IPDlg, CDialog)
    //{{AFX_MSG_MAP(CECHO_TCP_IPDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_SEND, OnSend)
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_WM_CLOSE()
    ON_WM_RBUTTONDOWN()
    ON_WM_SIZE()
    ON_COMMAND(ID_MENU_CONNECT, OnMenuConnect)
    ON_COMMAND(ID_MENU_RUN_SERVER, OnMenuRunServer)
    ON_COMMAND(ID_MENU_SETTING_BOX, OnMenuSettingBox)
    ON_COMMAND(ID_FILE_EXIT, OnFileExit)
    ON_NOTIFY(EN_SETFOCUS, IDC_INPUT, OnSetfocusInput)
    ON_NOTIFY(NM_CLICK, IDC_INPUT, OnClickInput)
    ON_EN_KILLFOCUS(IDC_INPUT_BOX, OnKillfocusInputBox)
    ON_EN_SETFOCUS(IDC_INPUT_BOX, OnSetfocusInputBox)
    ON_NOTIFY(NM_RETURN, IDC_INPUT, OnReturnInput)
    ON_COMMAND(ID_MENU_STOP_SERVER, OnMenuStopServer)
    ON_WM_MOUSEWHEEL()
    ON_COMMAND(ID_ABOUT, OnAbout)
    ON_WM_DESTROY()
    ON_COMMAND(ID_FILE_SEND, OnFileSend)
    ON_COMMAND(IDC_CLEAR_LOG, OnClearLog)
    ON_COMMAND(IDC_CRASH, OnCrash)
    ON_COMMAND(IDC_DISCONNECT, OnDisconnect)
    ON_NOTIFY(NM_CLICK, IDC_RICHEDIT, OnClickRichedit)
    ON_NOTIFY(NM_RCLICK, IDC_RICHEDIT, OnRclickRichedit)
    ON_NOTIFY(NM_DBLCLK, IDC_RICHEDIT, OnDblclkRichedit)
    ON_NOTIFY(NM_RDBLCLK, IDC_RICHEDIT, OnRdblclkRichedit)
    ON_LBN_SELCHANGE(IDC_USER_LIST, OnSelchangeUserList)
    ON_WM_DRAWITEM()
    ON_COMMAND(ID_MENU_LOAD_LOG, OnMenuLoadLog)
    ON_WM_MEASUREITEM()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_CONNECT, OnConnect)
    ON_WM_CHAR()
    ON_BN_CLICKED(IDC_RUN_SERVER, OnRunServer)
    //	ON_COMMAND(ID_CRASH_SERVER, OnCrashServer)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECHO_TCP_IPDlg message handlers

BOOL CECHO_TCP_IPDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    LoadLibrary("riched32.dll");

    SYSTEMTIME time;
    GetLocalTime(&time);

    PathTextLog.Format("logs\\%02i-%02i-%i_[%02i_%02i_%02i].log",time.wDay,time.wMonth,time.wYear,time.wHour,time.wMinute,time.wSecond);
    PathRtfLog.Format("logs\\%02i-%02i-%i_[%02i_%02i_%02i].rtf",time.wDay,time.wMonth,time.wYear,time.wHour,time.wMinute,time.wSecond);


//	m_richedit.SetFocus();

    //генерируем сами себе свой идентификатор
rest:
    srand(GetTickCount());
    UnID=1+rand();

    //это возможно в случае если rand()+1 станет равным нулю
    if (UnID==0) goto rest;


    //делаем системный шрифт "Tahoma"
    strcpy(SystemSettings.cf.szFaceName,"Tahoma");


    char buf[1024];

    //получаем командную строку
    char *command=GetCommandLine();

    CString Cscom=command;


    //если имеется ключевое слово HIDE
    if (strstr(command,"hide")==NULL)
    {
        CRegKey reg;
        reg.Create(HKEY_CURRENT_USER,
                   "Software\\Microsoft\\Windows\\CurrentVersion\\Run");

        //получаем текушую директорию
        GetCurrentDirectory(1024,buf);

        //добавляем название приложения
        strcat(buf,"\\echo_tcp_ip.exe hide");

        reg.SetValue(buf,"TCP_IP_CHAT");
        reg.Close();

        GetCurrentDirectory(1024,buf);
        reg.Create(HKEY_CURRENT_USER,"Software\\ECHO_TCP_IP");
        reg.SetValue(buf,"CurDir");
        reg.Close();
    }
    //запускаем скрыто
    else
    {
        CRegKey reg;
        reg.Create(HKEY_CURRENT_USER,"Software\\ECHO_TCP_IP");
        DWORD count=1024;
        //получаем текущий путь
        reg.QueryValue(buf,"CurDir",&count);
        reg.Close();

        SetCurrentDirectory(buf);
    }

    AddIcon();

    Cursor.LinkRichEdit(&m_richedit);

    SetTimer(1, 100,NULL);

    SettingBox.LoadSettings();
    
	LocalSettings=SettingBox.GetUserSettings();
    
	ShowSendRecvInfo=SettingBox.m_system_message;


    m_richedit.SetBackgroundColor(false, BackGroundColor);
//	m_user_list.SetBackgroundColor(false, RGB(10,10,10));

    {
        CHARFORMAT cf= {0};

        cf=LocalSettings.cf;

        cf.yHeight=INPUT_HEI;

        m_input.SetBackgroundColor(false,BackGroundColor);
        

		WizSelectionCharFormat(m_input, cf);


        //m_input.SetEventMask()
    }


    ChatWindowAddStringUID("Ok"+ENDL,SYSTEM_UID);

    CRect r;
    GetWindowRect(&r);

    CRegKeyAdv reg;

    DWORD x,y,cx,cy;
    reg.QueryValue( x, "PosX");
    reg.QueryValue( y, "PosY");
    reg.QueryValue( cx, "WinCX");
    reg.QueryValue( cy, "WinCY");
    if (!cx && !cy) cx=r.Width(), cy=r.Height();
    MoveWindow(x,y, cx,cy);


    PutData;

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    m_input_box.SetFocus();

    if (SettingBox.m_connect_on_start) OnConnect();
    else if (SettingBox.m_run_server_on_start) OnRunServer();

    SynchroMenuButtons();

    SetTitle();



    return FALSE;  // return TRUE  unless you set the focus to a control
}

void CECHO_TCP_IPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

void CECHO_TCP_IPDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CECHO_TCP_IPDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CECHO_TCP_IPDlg::OnRunServer()
{
    ServerSocket=new CServerSocket;
    UnID=0;

    if (!ServerSocket->Create(SettingBox.m_port, SOCK_STREAM, FD_WRITE
                              | FD_READ | FD_ACCEPT | FD_CONNECT | FD_CLOSE))
    {
        delete ServerSocket;
        ServerSocket=NULL;
        MessageBox("Не удается создать сервер");
        return;
    }

    CString mes;
    mes.Format("Запуск сервера(порт %i). Ожидаются подключения..."+ENDL, SettingBox.m_port);
    ChatWindowAddStringUID(mes,SYSTEM_UID);

    ServerSocket->Listen(1);

    SynchroMenuButtons();

    m_input_box.SetFocus();

    LocalSettings.uid=0;	//признак сервера
}

void CECHO_TCP_IPDlg::OnReceive(const CString &str, DWORD sock)
{
    CString z=str;

    BYTE k=str[0];
    if (k!=RING_SYMBOL) Sound(SOUND_CLICK);
    else Sound(SOUND_RING2);

    ScrollRich();

    //это применимо только к серверу
    ChatWindowAddStringUID(z, UserInfo[GetUserIndex(sock)].RemoteSettings.uid);

    if (SettingBox.m_auto_chat_show) ShowWindow(SW_SHOWNORMAL);
}

//послать целую строку
void CECHO_TCP_IPDlg::OnSend()
{
    CString tmp,src;
    m_input.GetWindowText(src);
    if (src.GetLength()==0) return;

    if (src.GetLength()>MAX_MESSAGE_LEN)
    {
        MessageBox("Посылаемое ваше сообщение слишком велико!\r\nУменьшите размер сообщение и повторите попытку", "Внимание!");
        return;
    }

    if (AllSpace(src)) return;

    if (src.GetLength() == 2)
    {
        if (
            (src[0]==13 && src[1]==10)
            ||
            ( src[0]==10 && src[1]==13)
        )

            return;
    }

    CString z;
    m_richedit.GetWindowText(z);
    char k=z.Right(2)[0];

    //if (k!='\r' && k!='\n') tmp+=ENDL;
    GetData;

    CString ss=src+ENDL;
    send_all(ss);
    ChatWindowAddStringUID(_S+LocalSettings.NickName+"> "+ss,UnID);

    m_input.SetWindowText("");
}

BOOL CECHO_TCP_IPDlg::PreTranslateMessage(MSG* pMsg)
{

    // TODO: Add your specialized code here and/or call the base class
    if (m_hide==true) ShowWindow(SW_HIDE);//,m_hide=false;

//	if (pMsg->message== WM_QUIT)
//	{
//		MessageBox("WM_QUIT");
//	}

    if (pMsg->message == WM_RBUTTONDOWN)
    {
        OnMenuSettingBox();
        return true;
    }

    if (pMsg->message == WM_COMMAND)
    {
        for(int i=0; i<UserInfo.size(); i++)
            if (DWORD(UserInfo[i].socket_connect) == pMsg->wParam)
                //удаляем пользователя
            {
                //DisconnectUser()
                //UserInfo[i].socket_connect->Close();
                OnRemoteClose(0, (DWORD)UserInfo[i].socket_connect);
                //delete UserInfo[i].socket_connect;
                //UserInfo.erase(&UserInfo[i]);
                //MessageBox("DISCONNECT ? ARE YOU SURE ?");
            }
    }

    if (pMsg->message == WM_KEYDOWN)
    {

        //if (GetFocus() != GetDlgItem(IDC_INPUT)) return true;

        BYTE key=pMsg->wParam;

        if (key==17) return true;

        if (key==27) return true;

        //ChatWindowAddStringUID("?", UnID, NO_NEW_LINE);

        //MessageBeep(MB_OK);

        //CString m;
        //m_input.GetWindowText(m);
        //m_input.SetWindowText(m+itos(key)+" ");

        switch (key)
        {
        case 13:
        {

            if (GetFocus() == GetDlgItem(IDC_INPUT_BOX))
            {
                /*	Cursor.Show();

                	ChatWindowAddStringUID(ENDL,SYSTEM_UID);

                	send_all(ENDL);
                */

            }
            //else
            if (GetFocus() == GetDlgItem(IDC_INPUT))
            {
                OnSend();
            }

            return true;
        }


        case VK_UP:
            m_richedit.LineScroll(-1);
            return 0;

        case VK_DOWN:
            m_richedit.LineScroll(1);
            return 0;
        }
    }

    /*	if ( pMsg->message == WM_CHAR)
    	{

    		ScrollRich();

    		BYTE m=pMsg->wParam;

    		if (m==13) return true;

    		if (GetFocus() != GetDlgItem(IDC_INPUT_BOX)) return false;


    		if (m!=RING_SYMBOL) Sound(SOUND_CLICK);
    		else		Sound(SOUND_RING);


    		if (m<32 && m!=8) return true;
    		Cursor.Show();
    		ChatWindowAddStringUID(m, UnID, NO_NEW_LINE);

    		//if (!ServerSocket) return false;
    		//если нет подключения к клиенту/серверу - не вводим ничего
    		if (UserInfo.size()==0) return true;

    		CString buf;
    		buf+=m;
    		//if (ConvertOemToAnsi) CharToOem(buf,buf);
    		send_all(buf);

    		return true;

    	}
    */

    return CDialog::PreTranslateMessage(pMsg);
}

void CECHO_TCP_IPDlg::OnTimer(UINT nIDEvent)
{

    SynchroMenuButtons();

    //if (GetFocus() != GetDlgItem(IDC_INPUT_BOX)) return;


    if (Cursor.count++ > 5)
    {
        Cursor.count=0;

        char& k=Cursor.String[0];
        if (k != Cursor.sym) k=Cursor.sym;
        else k=' ';

        Cursor.Erase();
        Cursor.Draw();

        if (need_invalidate)
        {
            m_richedit.InvalidateRect(NULL,false);
            m_richedit.UpdateWindow();
        }


    }

    CDialog::OnTimer(nIDEvent);
}


//к серверу присоединяется клиент
void  CECHO_TCP_IPDlg::OnClientAccept(int err)
{
    CConnectedUserInfo info;

    info.socket_connect=new CClientSocket;

    SOCKADDR sockaddr;
    sockaddr.sa_family=AF_INET;
    int addrlen=sizeof(sockaddr);
    ServerSocket->Accept(*info.socket_connect, &sockaddr, &addrlen);
//		ServerSocket->Accept(*info.socket_connect);

    SOCKADDR_IN *saddr=(SOCKADDR_IN*)&sockaddr;

    BYTE b1=saddr->sin_addr.S_un.S_un_b.s_b1;
    BYTE b2=saddr->sin_addr.S_un.S_un_b.s_b2;
    BYTE b3=saddr->sin_addr.S_un.S_un_b.s_b3;
    BYTE b4=saddr->sin_addr.S_un.S_un_b.s_b4;


    info.ip.Format("%i.%i.%i.%i"+ENDL,b1,b2,b3,b4);

    ChatWindowAddStringUID("Подключился пользователь с IP:"+info.ip,SYSTEM_UID);



    //теперь следует ожидать от пользователя управляющего
    //пакета. Если разрешены подключения от Telneta -
    //установить соотв. флаги (Echo для этого пользователя
    //конвертацию OEM и др.)

    //если приходит пакет с ESC==2 (старая верся chat)
    //проинформировать пользователя, что версия поменялась
    //и ему следует обновить программное обеспечение
    //если приходит новый формат пакет с ESC==3, далее два байта
    //размер пакета, и байт версии. проверяем на размер. Если совпадает, значит
    //версия и размер - значит версии идентчиный, принмаем
    //от пользователя его инфу (Ник,Цвет щрифт) и рассылаем эту инфу
    //остальным подключенным пользователям а вновь подлюченному
    //отсылаем информацию о сервере, и остальных подключенных пользователей.
    //время на получения информации ограничено TimeOut'ом
    //если по истечении тайм аута информация от пользователя не поступило-
    //сбрасываем данное подключения и не отсылаем остальным параметры
    //данного пользователя

    /*сервер присваивает подключенному пользователю уникальный
    идентификатор uid, который сохраняется на протяжении всего подключения
    клиенты, получая информацию о других пользователях, получают их uid
    клиенту может придти пакет на удаления пользователя с заданным
    uid
    */

    //сервер назначает клиенту UID
    //for(int i=0; i<UserInfo.size(); i++)
    //{
    //	if (UserInfo[i]
    //}
    info.RemoteSettings.uid=GetTickCount();
    info.startime=GetTickCount();
    strcpy(info.RemoteSettings.NickName,"Ожидаю инфу...");
    info.RemoteSettings.cf=SystemSettings.cf;
    info.RemoteSettings.size=sizeof(info.RemoteSettings);
    UserInfo.push_back(info);

    RefreshUserList();
}

/////// установка соединения с сервером
int CECHO_TCP_IPDlg::OnConnect()
{
    LocalSettings=SettingBox.GetUserSettings();
    CString Caption="В Н И М А Н И Е . . .";

    if (ServerSocket)
    {
        MessageBox("Соединиться с сервером невозможно, \r\nпоскольку в данным момент вы сами являетесь сервером!",Caption, MB_ICONINFORMATION);
        return false;
    }

    if (UserInfo.size())
    {
        MessageBox("Соединение с сервером уже установлено!\r\n",Caption,MB_ICONINFORMATION);
        return false;
    }

    if (AllSpace(SettingBox.m_nick_name))
    {
        MessageBox("Задайте в настройках свой ник.\r\nПустой ник недопустим!","Внимание!");
        return false;
    }

    SettingBox.TryConnect=true;

    //CConnectedUserInfo info;
    CClientSocket * socket = new CClientSocket;
    socket->Create();
    socket -> Connect(SettingBox.m_cremote_ip, SettingBox.m_port);
//	info.RemoteSettings.uid=0;	//это сервер
//	strcpy(info.RemoteSettings.NickName, "РОЖА");
//	UserInfo.push_back(info);

    CString k;

    ChatWindowAddStringUID("Попытка установить соединение c "+SettingBox.m_cremote_ip+":"+
                           itos(SettingBox.m_port)+ENDL,SYSTEM_UID);

//	RefreshUserList();

    SynchroMenuButtons();

    return true;
}

void CECHO_TCP_IPDlg::OnOK()
{
    //WriteLogFile();
    CDialog::OnOK();
//	MessageBox("OK");
}

//посылка сервером своей информацией остальным пользователям-клиентам
void CECHO_TCP_IPDlg::InfoExchange()
{
    if (ServerSocket)
        for(int i=0; i<UserInfo.size(); i++)
        {
            UserInfo[i].SendInfo(LocalSettings);
            //ChatWindowAddStringUID("InfoExchange - Server"+ENDL,SYSTEM_UID);
        }
    else if (UserInfo.size()>0)
    {
        UserInfo[0].SendInfo(LocalSettings);
        //ChatWindowAddStringUID("InfoExchange - Client"+ENDL,SYSTEM_UID);
    }


}

//посылка информацией другим пользователям ользователям
//source - номер пользователя, чья информацию будем передана остальным
void CECHO_TCP_IPDlg::InfoSend(int source)
{
    for(int i=0; i<UserInfo.size(); i++)
    {
        if (i==source) continue;
        UserInfo[i].SendInfo(UserInfo[source].RemoteSettings);
        //ChatWindowAddStringUID("InfoSend"+ENDL,SYSTEM_UID);
    }

}

void CECHO_TCP_IPDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    WriteLogFile();
    ShowWindow(SW_HIDE);


    /*if (UserInfo.size() !=0)
    {
    	if (MessageBox("Установлено соединение с другим компьютером!\r\nПодтверждаете выход?", "Внимание!", MB_OKCANCEL) == IDOK )
    	{
    		for (int i=0; i<UserInfo.size(); i++)
    		OnRemoteClose(0, (DWORD)UserInfo[i].socket_connect);


    		WriteLogFile();
    		CDialog::OnClose();
    	}


    }
    else
    {
    	WriteLogFile();
    	CDialog::OnClose();
    }*/

    AddIcon();
}


void CECHO_TCP_IPDlg::RefreshUserList()
{
    CString us;

    int nick_size=250;

    m_user_list.ResetContent();

    for(int i=0; i<UserInfo.size(); i++)
    {
        CString m=_S+UserInfo[i].RemoteSettings.NickName;

        //itos(DWORD(UserInfo[i].socket_connect))+" "+
        //" ("+ itos(UserInfo[i].RemoteSettings.uid)+")";

        //CHARFORMAT f=UserInfo[i].RemoteSettings.cf;
        //f.yHeight=30;

        CHARFORMATA cf=UserInfo[i].RemoteSettings.cf;

        cf.yHeight=nick_size;
        
		//WizSelectionCharFormat(m_user_list, cf);
        //WizSelectionCharFormat(m_user_list, UserInfo[i].RemoteSettings.cf);

        //m_user_list.ReplaceSel(m+ENDL);
        
		m_user_list.AddString(m);
        int n=m_user_list.GetCount();
        m_user_list.SetItemData(n-1,(ULONG) (&UserInfo[i].RemoteSettings) );

    }


    if (UserInfo.size()!=0)
    {
        CHARFORMATA cf=LocalSettings.cf;
        cf.yHeight=nick_size;
        
		//WizSelectionCharFormat(m_user_list, cf);
        //m_user_list.ReplaceSel(LocalSettings.NickName+ENDL);
        
		m_user_list.AddString(LocalSettings.NickName);
        
		int n=m_user_list.GetCount();
        m_user_list.SetItemData(n-1,(ULONG) (&LocalSettings));

    }

    SetTitle();


}


//вызывается при получения информации об удаленном пользователе
void CECHO_TCP_IPDlg::OnRemoteInfoReceive(int sender,
        CUserSettings IncomingRemote)
{

    //если это сервер
    if (ServerSocket)
    {
        //если это первая информация - это информация о пользователе
        if ( ++ (UserInfo[sender].CountInfoReceive) == 1)
        {

            CMenu *menu=GetMenu();
            CMenu *sub=menu->GetSubMenu(2);

            //добавляем в меню строчку с информацией о том, кого следует отключить
            sub->AppendMenu(MF_STRING, (DWORD)UserInfo[sender].socket_connect,_S+"отключить "+
                            UserInfo[sender].RemoteSettings.NickName+"("+itos(UserInfo[sender].RemoteSettings.uid)+")");

            CString c;

            //посылаем инфо присоеденившемуся клиенту об остальных клиентах если мы сервер
            //и о себе
            CString message;
            message=_S+"В чат вошел пользователь: "+ UserInfo[sender].RemoteSettings.NickName+ENDL;

            Sound(SOUND_NEWUSER,STOP_SOUND);
            ChatWindowAddStringUID(message,SYSTEM_UID);


            UserInfo[sender].SendInfo(LocalSettings);

            for(int i=0; i<UserInfo.size(); i++)
            {
                if (i!=sender)
                {
                    UserInfo[sender].SendInfo(UserInfo[i].RemoteSettings);
                    UserInfo[i].socket_connect->SendStringWithUID(message,SYSTEM_UID);
                }
            }


            //посылаем последние 20 строк
            int max=m_richedit.GetLineCount();
            for(int k=max-20; k<max; k++)
            {
                char buf[4096];
                int c=m_richedit.GetLine(k,buf);
                if (c==0) continue;
                buf[c]=0;

                UserInfo[sender].socket_connect->SendStringWithUID(_S+buf,HISTORY_UID);
            }

            CString z="Сейчас в чате:";
            z+=LocalSettings.NickName;
            int size=UserInfo.size();
            for(i=0; i<size; i++)
            {
                z+=", ";
                z+=UserInfo[i].RemoteSettings.NickName;
            }
            z+=ENDL;


            UserInfo[sender].socket_connect->SendStringWithUID(z,SYSTEM_UID);
            //если есть приветствие
            if (!AllSpace(SettingBox.m_greeting))
            {
                CString k;
                k+=dialog->SettingBox.m_greeting+ENDL;

                //посылаем свое приветствие присоединившемуся клиенту
                UserInfo[sender].socket_connect->SendStringWithUID(k,0);

                //свое приветствие отображаем в своем окне
                ChatWindowAddStringUID(_S+LocalSettings.NickName+">"+SettingBox.m_greeting,UnID,true);
            }

        }
    }

    //если не сервер - добавляем в список нового пользователя или обновляем
    if (!ServerSocket)
    {
        bool found=false;

        //если пользователь уже есть в списке
        for(int i=0; i<UserInfo.size(); i++)
        {
            // uid в имеющемся списке совпал с uid пришедшей инф
            if (UserInfo[i].RemoteSettings.uid == IncomingRemote.uid)
            {
                //обновляем информацию о пользователе
                UserInfo[i].RemoteSettings=IncomingRemote;
                found=true;
                break;
            }
        }

        //если в списке не нашлось uid переданной инфы, значит инфа о новом пользователе
        if (!found)
        {
            CConnectedUserInfo info;
            info.RemoteSettings=IncomingRemote;
            //добавляем в список нового пользователя
            UserInfo.push_back(info);

            /*if (!AllSpace(SettingBox.m_greeting))
            {
            	CString k;
            	k+=dialog->SettingBox.m_greeting+ENDL;

            	//посылаем свое приветствие присоединившемуся клиенту
            	UserInfo[UserInfo.size()-1].socket_connect->SendStringWithUID(k,0);
            }*/


        }

    }

    CString result;

    CString _R="получена инфа от ";

    if (ServerSocket)
        result+=_R+UserInfo[sender].RemoteSettings.NickName + "("+itos(UserInfo[sender].RemoteSettings.uid)+")";
    else
        result+=_R+IncomingRemote.NickName + "("+itos(IncomingRemote.uid)+")";

    result+=ENDL;

    if (ShowSendRecvInfo) ChatWindowAddStringUID(result, SYSTEM_UID);

    RefreshUserList();

}




//////////////////////////////////////////////////////////////////////////

void CECHO_TCP_IPDlg::EnableClientServerButtons(bool enable)
{
    CMenu *m=GetMenu();
    int k;
    enable == true ? k=MF_ENABLED :k=MF_GRAYED;
    m->EnableMenuItem(ID_MENU_RUN_SERVER, k);
    m->EnableMenuItem(ID_MENU_CONNECT, k);

}

//////////////////////////////////////////////////////////////////////////
//извещение приходит после попытки соединения с сервером
void CECHO_TCP_IPDlg::OnToServerConnect(int err, DWORD sock)
{
    int active;
    //ошибка
    if (err)
    {
        ChatWindowAddStringUID("Не удалось установить соединение! (err="+itos(err)+")"+ENDL,SYSTEM_UID);

        active=GetUserIndex(sock);

        if (active!=-1)
            UserInfo.erase(&UserInfo[active]); //вызывает expection

        SettingBox.TryConnect=false;
        SynchroMenuButtons();

        return;
    }

    CConnectedUserInfo info;
    info.socket_connect=(CClientSocket*)sock;
    info.RemoteSettings.uid=0;

    UserInfo.push_back(info);
    ChatWindowAddStringUID("Установлено соединение!"+ENDL,SYSTEM_UID);
    Sound(SOUND_NEWUSER,SOUND_STOP);

    InfoExchange();

    active=GetUserIndex(sock);
    /*	//еперь нужно послать информацию от всех сокетов к подключенному
    	for(int i=0; i<UserInfo.size(); i++)
    	{
    		if (i!=active)
    		UserInfo[active].SendInfo(UserInfo[i].RemoteSettings);
    	}
    */

//	send_all(_S+LocalSettings.NickName+" >> "+SettingBox.m_greeting+ENDL);
    if (!AllSpace(SettingBox.m_greeting))
    {
        send_all(_S+SettingBox.m_greeting+ENDL);

        ChatWindowAddStringUID(_S+LocalSettings.NickName+">"+SettingBox.m_greeting+ENDL,UnID,true);
    }

    SetTitle();
}

//////////////////////////////////////////////////////////////////////////
//функция считывает count байт по адресу ad
//возврашает true - блок успешно считался
//false - блок не смог считаться
bool ReceiveBlock(CClientSocket* socket, void *ad, int count)
{
    BYTE * addr=(BYTE*) ad;
    int n=0;
    do
    {
        //!!!!!!!!!!!!!!!!!!!!ПРОБЛЕМНЫЙ КОД!!!!!!!!!!!!!!!!!!!

        //попытка чтения из сокета count байт
        n=socket->Receive(addr,count);
        int err=GetLastError();
        //если считали столько, сколько было затребовано сразу выходим
        if (n==count) return true;

        //было закрыто соединение
        if (n==0)
        {
            CString error="Во время чтения данных из сокета "+
                          itos(DWORD(socket))+" разорвалось соединение";
            dialog->ChatWindowAddStringUID(
                error,SYSTEM_UID);
            socket->OnClose(err);
            send_all(error);
            return false;
        }
//WSAECONNABORTED
        //если произошла ошибка при чтении из сокета
        if (n==SOCKET_ERROR)
        {
            
            CString error="Ошибка при чтении данных из сокета "+itos(DWORD(socket))+
                          ", (err= "+itos(err)+")";
            dialog->ChatWindowAddStringUID(error
                                           ,SYSTEM_UID);
            socket->OnClose(err);
            send_all(error);
            return false;
        }

        addr+=n;
        count-=n;
        Sleep(1);
    }
    while (count!=0);

    return true;

}

//что-то пришло с удаленного компьютера
void CECHO_TCP_IPDlg::OnReceive(int err, DWORD sock)
{


    CUserSettings IncomingRemote;

    char a='\v';
    //ищем, если в списке пользователей пользователь, у которого
    //номер сокета совпадает с номером сокета пользователя, от которого
    //в данный момент приходят данные
    for(int sender=0; sender<UserInfo.size(); sender++)
        if ( DWORD(UserInfo[sender].socket_connect) == sock) break;

    //если не нашли от кого пришла инфа
    if (sender==UserInfo.size())
    {
        
        //спорный момент
        sender=0;
    }

    char buf;

    static int userinfo;

    CClientSocket * socket=(CClientSocket*)sock;

    Cursor.Show();
    if (!ReceiveBlock(socket,&buf,1))
    {
        return;
    }

    //принялу команду на удаление пользователя
    if (buf==ESC_USER_EXIT)
    {
        DWORD uid;

        //если не смогли принять ID пользователя
        if (!ReceiveBlock(socket, &uid,4))
        {
            ChatWindowAddStringUID(_S+"не смогли принять ID пользователя который в данный момент отключается (код ESC_ посылал "+UserInfo[sender].RemoteSettings.NickName +")",SYSTEM_UID);
            return;
        }

        for(int i=0; i<UserInfo.size(); i++)
        {
            if (UserInfo[i].RemoteSettings.uid==uid)
            {
                ChatWindowAddStringUID(_S+"удалили пользователя "+UserInfo[i].RemoteSettings.NickName,SYSTEM_UID);
                UserInfo.erase(&UserInfo[i]);
                RefreshUserList();
                return;
            }
        }

        if (i==UserInfo.size())
        {
            ChatWindowAddStringUID(_S+"принятый ID пользователя на удаление не найден в списке (код послал пользователь "+UserInfo[sender].RemoteSettings.NickName +")",SYSTEM_UID);
            return;
        }

    }

    //послали целую строку с указанием источника
    if (buf==ESC_SEND_UID_STRING)
    {
        WORD size;
        DWORD uid;
        //считываем размер строки
        if (!ReceiveBlock(socket,&size,2))
        {
            /// ошибка
            ChatWindowAddStringUID(_S+"ошибка чтения длины строки от пользователя "+UserInfo[sender].RemoteSettings.NickName ,SYSTEM_UID);
            return;

        }


        //считываем uid
        if (!ReceiveBlock(socket,&uid,4))
        {
            //ошибка
            ChatWindowAddStringUID(_S+"ошибка чтения ID пользователя от пользователя "+UserInfo[sender].RemoteSettings.NickName ,SYSTEM_UID);
            return;

        }

        //если размер строки корректен

        if (size>=1 && size<=MAX_MESSAGE_LEN)
        {
            BYTE * recvstr=new BYTE[size+1];

            if (!ReceiveBlock(socket,recvstr,size))
            {
                ChatWindowAddStringUID(_S+"ошибка принятия строки текста от пользователя "+UserInfo[sender].RemoteSettings.NickName ,SYSTEM_UID);
                return;
            }


            recvstr[size]=0;


            CString nick;
            for(int src=0; src<UserInfo.size(); src++)
            {
                CUserSettings &ui=UserInfo[src].RemoteSettings;
                if (ui.uid==uid) nick=_S+ui.NickName+"> ";
            }

            //если приняли только один символ
            if (size==1)
                ChatWindowAddStringUID(recvstr, uid, NO_NEW_LINE);
            else
                ChatWindowAddStringUID(nick+recvstr, uid, NEW_LINE);

            if (SettingBox.m_auto_chat_show)
            {
                m_hide=false;
                ShowWindow(SW_NORMAL);
                //ShowWindow(SW_RESTORE);
            }

            //если мы сервер, рассылаем всем остальным пользователям принятую строку
            if (ServerSocket)
            {
                for(int i=0; i<UserInfo.size(); i++)
                {
                    //эта проверка нужна для того, чтобы не посылать сообщение пользователю
                    //от которого пришло данное сообщение
                    //как вариант можно изменить алгоритм, чтобы при посылке сообщения
                    //у клиента, в его окне сообщения отображалось не мгновенно,
                    //а после принятия от сервера. Но это увеличит траффик.
                    if (UserInfo[i].RemoteSettings.uid!=uid)
                    {
                        UserInfo[i].socket_connect->SendStringWithUID(recvstr, uid);
                    }
                }
            }


            delete recvstr;

            return;
        }
        //иначе разрываем соединение с таким клиентом из-за
        //нарушения протокола (слишком длинное сообщение)
        else
        {
            socket->OnClose(0);
            return;
        }

    }

    if (buf== ESCAPE_CODE)
    {

        //чтение информации о пользователях

        CUserSettings usr;


        if (!ReceiveBlock(socket,&usr,sizeof(usr)))
        {
            //ошибка
            ChatWindowAddStringUID(_S+"Произошла ошибка при считывании информации о пользователе "+UserInfo[sender].RemoteSettings.NickName,SYSTEM_UID);
            return;
        }

        //если мы сервер
        if (ServerSocket)
        {
            //считываем информацию о пользователе в соответтвующий ему слот
            UserInfo[sender].RemoteSettings=usr;
        }
        else
            //если мы клиент
        {
            //считываем информацию о пользователе в одну наперед заданную
            //структурупеременную
            IncomingRemote = usr;
        }

        //получили информацию, отдаем ее функции OnRemoteInfoReceive
        OnRemoteInfoReceive(sender,IncomingRemote);

        //если сервер - рассылаем изменившуюся информацию об одном пользователе
        //всем остальным пользователям
        if (ServerSocket)
        {
            InfoSend(sender);
        }

        //изменяем заголовок окна
        SetTitle();
        return;

    }

    if (buf== OLD_ESC_CODE)
    {
        CString k="Используйте более новую версию чата. Используемая Вами версия не совместима с версией сервера!";
        socket->SendData(k+ENDL,sizeof(k)+sizeof(ENDL));
        ChatWindowAddStringUID("Клиент использует старую версию чата! Произведено отключение"+ENDL,SYSTEM_UID);
        Sleep(500);
        socket->Close();
        delete socket;//->Close();
        UserInfo.erase(&UserInfo[sender]);
        RefreshUserList();
        return;
    }

    CString s=buf;
    OnReceive(s, sock);

    //если сервер - делаем эхо остальным пользователям
    /*		if (ServerSocket)
    		{
    			for(int i=0; i<UserInfo.size(); i++)
    				//тому кто прислал не отсылаем
    				if (i!=sender)
    				{
    					UserInfo[i].socket_connect->SendData(&buf,1);
    				}

    		 }
    */

}

//////////////////////////////////////////////////////////////////////////

//закрывается удаленный пользователь
void CECHO_TCP_IPDlg::OnRemoteClose(int err, DWORD sock)
{

    //если мы сервер
    if (ServerSocket)
    {

        int active=GetUserIndex(sock);
        if (active==-1)
        {
            ChatWindowAddStringUID("ОШИБКА: отключился незарегистрированный пользователь!",SYSTEM_UID);
            return;
        }

        DWORD del_uid=UserInfo[active].RemoteSettings.uid;

        CString tmp=_S+UserInfo[active].RemoteSettings.NickName+"("+itos(del_uid)+") отключился"+ENDL;
        Sound(SOUND_DISCONNECT);
        ChatWindowAddStringUID(tmp,SYSTEM_UID);

        //удаляем из меню строку "отключить того-то.."
        CMenu *sub=GetMenu()->GetSubMenu(2);
        sub->DeleteMenu(sock,MF_BYCOMMAND);


        delete UserInfo[active].socket_connect;
        UserInfo.erase(&UserInfo[active]);

        BYTE packet[5];
        packet[0]=ESC_USER_EXIT;
        LPBYTE puid=LPBYTE(&del_uid);
        packet[1]=puid[0];
        packet[2]=puid[1];
        packet[3]=puid[2];
        packet[4]=puid[3];

        //посылаем остальным сообщение об отключении

        for(int i=0; i<UserInfo.size(); i++)
        {
            UserInfo[i].socket_connect->SendData(&packet,5);
            UserInfo[i].socket_connect->SendStringWithUID(tmp, SYSTEM_UID);
        }
    }
    //если мы клиент - значит отключилсь от сервера
    else
    {
        //очищаем список пользователей
        UserInfo.clear();
        ChatWindowAddStringUID("Сервер разорвал соединение"+ENDL,SYSTEM_UID);
        SettingBox.TryConnect=false;
    }


    SynchroMenuButtons();
    RefreshUserList();
    //EnableClientServerButtons(true);
}

void CECHO_TCP_IPDlg::OnLButtonDown(UINT nFlags, CPoint point )
{
    m_input_box.SetFocus();
    CDialog::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////


//#include "piano_plugin\pianores.h"
void CECHO_TCP_IPDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    OnMenuSettingBox();
//	CDTFM_GeneratorDlg piano;
//	piano.DoModal();
    CDialog::OnRButtonDown(nFlags, point);
}
//////////////////////////////////////////////////////////////////////////

void CECHO_TCP_IPDlg::SetTitle()
{
    if (UserInfo.size()==0) SetWindowText("TCP/IP Chat -  Нет подключенных пользователей!");
    else
    {

        CString m=_S+LocalSettings.NickName+ " говорит с ";
        for(int i=0; i<UserInfo.size(); i++)
        {
            m+=UserInfo[i].RemoteSettings.NickName;
            if (i<UserInfo.size()-1)
                m+=", ";
        }
        SetWindowText(m);
    }

    //OnAbout();
}
//////////////////////////////////////////////////////////////////////////

void CECHO_TCP_IPDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (m_richedit.m_hWnd)
    {
        CRect r, inp;
        GetClientRect(&r);
        m_input.GetWindowRect(&inp);

        int len2=100;
        int 
			len1=r.Width()-len2,
        	hei=r.Height()-inp.Height();

        m_richedit.MoveWindow(0,0,   len1, hei);
        m_input.   MoveWindow(0,hei, r.Width(), inp.Height());
        m_user_list.MoveWindow(len1,0, len2, hei);
    }

}
//////////////////////////////////////////////////////////////////////////

void CECHO_TCP_IPDlg::OnMenuConnect()
{
    OnConnect();
}

//////////////////////////////////////////////////////////////////////////

void CECHO_TCP_IPDlg::OnMenuRunServer()
{
    OnRunServer();

    SynchroMenuButtons();
}
//////////////////////////////////////////////////////////////////////////

void CECHO_TCP_IPDlg::OnMenuSettingBox()
{
    SettingBox.DoModal( ServerSocket ? false : true);
    
	LocalSettings=SettingBox.GetUserSettings();


    CHARFORMAT cf=LocalSettings.cf;
    cf.yHeight=INPUT_HEI;

    m_input.SetSel(0,-1);
    
	WizSelectionCharFormat(m_input, cf);

    m_input.SetSel(0,0);

    InfoExchange();
    
	SetTitle();
    
	ShowSendRecvInfo=SettingBox.m_system_message;
    
	RefreshUserList();

}


//////////////////////////////////////////////////////////////////////////
void CECHO_TCP_IPDlg::WriteLogFile()
{
    //если нет разрещения на запись лога не пишем логи
    if (SettingBox.m_enable_write_log_file == 0 ) return;

    //MessageBox("WRITE LOG FILE!");
    CreateDirectory("Logs",NULL);

    CFile file;
    if (file.Open(PathTextLog,file.modeWrite|file.modeCreate))
    {

        //CString txt;
        //m_richedit.GetWindowText(txt);
        GetData;

        //file.Write(txt.GetBuffer(0),txt.GetLength());
        file.Write(m_richtext.GetBuffer(0),m_richtext.GetLength());
        file.Close();
    }

    RtfFile.Open(PathRtfLog, CFile::modeCreate|CFile::modeWrite);
    EDITSTREAM es;
    es.dwCookie=(DWORD) &RtfFile;
    es.pfnCallback=MyStreamOutCallback;
    m_richedit.StreamOut(SF_RTF,es);

    RtfFile.Close();
}


void CECHO_TCP_IPDlg::OnFileExit()
{
    if (ServerSocket)
    {
        OnMenuStopServer();
        if (ServerSocket) return;
    }

    if (UserInfo.size() !=0)
    {
        if (MessageBox("В настоящей момент установлено подключение к чат-серверу.\r\n"
                       "Вы действительно желаете выйти из чата ?"," Внимание ! ", MB_OKCANCEL) == IDCANCEL)
            return;
    }




    OnOK();

}
//////////////////////////////////////////////////////////////////////////

void CECHO_TCP_IPDlg::OnSetfocusInput(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void CECHO_TCP_IPDlg::OnClickInput(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
//	Cursor.Hide();
//	OnRichTest("\b ");

//	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void CECHO_TCP_IPDlg::OnKillfocusInputBox()
{
    //нужно спрятать курсор в RichEdit
//	Cursor.Hide();
//	Cursor.ReDraw();
}

void CECHO_TCP_IPDlg::OnSetfocusInputBox()
{
    // TODO: Add your control notification handler code here
//	Cursor.Show();
//	Cursor.ReDraw();
}


void CECHO_TCP_IPDlg::OnReturnInput(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    //OnSend();
    *pResult = 0;
}


void CECHO_TCP_IPDlg::OnMenuStopServer()
{
    if (!ServerSocket) return;

    //если есть активные соединения - закрываем соединения
    if (UserInfo.size())
    {
        CString z="Установлено соединение с пользователями: ";
        for(int i=0; i<UserInfo.size(); i++)
        {
            z+=UserInfo[i].RemoteSettings.NickName;
            if (i<UserInfo.size()-1) z+=",";
        }

        z+="\r\nОстановка сервера повлечет за собой также разрыв активных соединений. Продолжить?";

        if ( MessageBox(z, "Остановка сервера", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
        {
            CMenu *sub=GetMenu()->GetSubMenu(2);
            for(int i=0; i<UserInfo.size(); i++)
            {

                sub->DeleteMenu(DWORD(UserInfo[i].socket_connect),MF_BYCOMMAND);

                UserInfo[i].socket_connect->SendStringWithUID("Cервер останавливается"+ENDL,SYSTEM_UID);
                delete UserInfo[i].socket_connect;



            }

            delete ServerSocket;
            ServerSocket=NULL;

            UserInfo.clear();

            ChatWindowAddStringUID("Сервер остановлен, все соединения разорваны"+ENDL,SYSTEM_UID);

            UserInfo.clear();

            RefreshUserList();
        }
    }
    //иначе закрываем сервер-сокет
    else
    {
        delete ServerSocket;
        ServerSocket=NULL;

        ChatWindowAddStringUID("Сервер остановлен"+ENDL,SYSTEM_UID);
res:
        UnID=1+rand();
        if (UnID==0) goto res;
    }

    SynchroMenuButtons();
}


//синхронизирую состояние меню и кнопок
void CECHO_TCP_IPDlg::SynchroMenuButtons()
{
#define mfe MF_ENABLED
#define mfd MF_GRAYED

    CMenu *m=GetMenu();

    int k;

    bool Server=false;
    bool ActiveConnections=false;

    if (ServerSocket) Server=true;
    if (UserInfo.size()) ActiveConnections=true;

    ActiveConnections || Server || SettingBox.TryConnect ? k=mfd: k=mfe;


    m->EnableMenuItem(ID_MENU_RUN_SERVER, k);

    if (SettingBox.m_hWnd) SettingBox.InitButtons(ServerSocket!=NULL, UserInfo.size() !=0);

    //сервер з
    m->EnableMenuItem(ID_MENU_STOP_SERVER, 	Server ?  mfe : mfd);

    if (Server)
    {
        m->EnableMenuItem(IDC_DISCONNECT, mfd);
        m->EnableMenuItem(IDC_CONNECT, mfd);
    }
    else
    {
        if (ActiveConnections)
        {
            m->EnableMenuItem(IDC_DISCONNECT, mfe);
            m->EnableMenuItem(IDC_CONNECT, mfd);
        }
        else
        {
            m->EnableMenuItem(IDC_DISCONNECT, mfd);

            if (SettingBox.TryConnect==true)
                m->EnableMenuItem(IDC_CONNECT, mfd);
            else
                m->EnableMenuItem(IDC_CONNECT, mfe);

        }
    }

}

BOOL CECHO_TCP_IPDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CECHO_TCP_IPDlg::OnAbout()
{
    AboutBox about;
    about.DoModal();
//	ChatWindowAddStringUID("x",SYSTEM_UID);
}

void CECHO_TCP_IPDlg::ScrollRich()
{
    if (!SettingBox.m_no_autoscroll)
    {
        int cnt=m_richedit.GetLineCount();
        for(int i=0; i<cnt; i++)
            m_richedit.LineScroll(1);
    }
}




BOOL CECHO_TCP_IPDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    WORD wNotifyCode = HIWORD(wParam);
    WORD wID = LOWORD(wParam);
    DWORD code = lParam;

    if (wID==ID_POPUP_SHOW)
    {
        m_hide=false;
        ShowWindow(SW_NORMAL);
        BringWindowToTop();
        return true;
    }

    if (wID==ID_POPUP_EXIT)
    {

        ShowWindow(SW_NORMAL);
        OnFileExit();
        return true;
    }

    if (wID == ID_SHELL_MESSAGE)
    {
        if (code==WM_LBUTTONDOWN)
        {
            if (m_hide==true)
            {
                m_hide=false;

                ShowWindow(SW_NORMAL);
                BringWindowToTop();
            }
            else
            {
                m_hide=true;
                ShowWindow(SW_HIDE);
            }

            return true;
        }

        if (code==WM_RBUTTONDOWN)
        {
            CMenu menu;
            menu.CreatePopupMenu();
            menu.AppendMenu(0,ID_POPUP_SHOW,"Показать");
            menu.AppendMenu(MF_SEPARATOR);
            menu.AppendMenu(0,ID_POPUP_EXIT,"Выйти");
            POINT p;
            GetCursorPos(&p);
            menu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_CENTERALIGN,p.x,p.y,this);
        }
        return 0;
    }



    return CDialog::OnCommand(wParam, lParam);
}

void CECHO_TCP_IPDlg::SavePlacement()
{
    SettingBox.WriteSettings();

    CRegKeyAdv reg;

    CRect r;
    GetWindowRect(&r);
    reg.SetValue( r.left, "PosX");
    reg.SetValue( r.top, "PosY");
    reg.SetValue( r.Width(), "WinCX");
    reg.SetValue( r.Height(), "WinCY");

    reg.Close();

}

void CECHO_TCP_IPDlg::AddIcon()
{
    NOTIFYICONDATA ni= {0};
    ni.cbSize=sizeof(ni);
    ni.hWnd=this->m_hWnd;
    ni.uID=ID_SHELL_MESSAGE;
    ni.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
    ni.uCallbackMessage=WM_COMMAND;
    strcpy(ni.szTip,"TCP/IP чат");

    ni.hIcon=hIcon;
    Shell_NotifyIcon(NIM_ADD, &ni);//) MessageBox("ошибка Shell_NotifyIcon ");

}

void CECHO_TCP_IPDlg::DeleteIcon()
{
    NOTIFYICONDATA ni= {0};
    ni.cbSize=sizeof(ni);
    ni.hWnd=this->m_hWnd;
    ni.uID=ID_SHELL_MESSAGE;
    ni.uFlags=NIF_MESSAGE;
    ni.uCallbackMessage=WM_COMMAND;

    Shell_NotifyIcon(NIM_DELETE, &ni);//) MessageBox("ошибка Shell_NotifyIcon ");
}

void CECHO_TCP_IPDlg::OnDestroy()
{
    DeleteIcon();
    SavePlacement();
    WriteLogFile();

    CDialog::OnDestroy();
}

void CECHO_TCP_IPDlg::OnFileSend()
{
    MessageBox("Извините, данная функция находится в стадии разработки ...");

}

CECHO_TCP_IPDlg::~CECHO_TCP_IPDlg()
{
    delete ServerSocket;

}

void CECHO_TCP_IPDlg::OnClearLog()
{
    m_richedit.SetWindowText("");

}

void CECHO_TCP_IPDlg::OnCrash()
{
    if (UserInfo.size()>0)
    {
        char data[4]= {ESC_USER_EXIT,0,1,2};
        UserInfo[0].socket_connect->SendData(&data,4);

        ChatWindowAddStringUID("Послали серверу запрещенную комбинацию"+ENDL,SYSTEM_UID);
    }

}

void CECHO_TCP_IPDlg::OnDisconnect()
{
    if (UserInfo.size())
    {
        if (MessageBox("Вы действительно желаете отключиться от чат-сервера?", "Подтверждение",MB_ICONWARNING | MB_OKCANCEL) == IDOK)
        {
            UserInfo[0].socket_connect->Close();
            //delete UserInfo[0].socket_connect;

            UserInfo.clear();

            RefreshUserList();

            Sound(SOUND_DISCONNECT,SOUND_STOP);
            ChatWindowAddStringUID("Связь с сервером прекращена"+ENDL,SYSTEM_UID);
            SettingBox.TryConnect=false;
            SynchroMenuButtons();
            UnID=1+rand();
        }
    }
    else
	{
        MessageBox("Нет соединения. Отключаться не от кого :)");
	}

}

void CECHO_TCP_IPDlg::OnClickRichedit(NMHDR* pNMHDR, LRESULT* pResult)
{

//	POINT p={0,0};;
//	m_richedit.SetCaretPos(p);

    *pResult = 0;
}


void CECHO_TCP_IPDlg::ChatWindowAddStringUID(CString recvstr, DWORD uid,bool WithNewLine)
{

    bool sound=false;

    for(int j=0; j<recvstr.GetLength(); j++)
    {
        if (BYTE(recvstr[j])==RING_SYMBOL)
        {
            Sound(SOUND_RING1);
            sound=true;
            break;
        }
        if (BYTE(recvstr[j])==RING_SYMBOL2)
        {
            Sound(SOUND_RING2);
            sound=true;
            break;
        }

    }

    //ещем подстроку смеха
	{
		const lol_sz=3;
		char * lol[lol_sz]= {"LOL","Lol","lol"};

		for(int i=0; i<lol_sz; i++)
		{
			if (recvstr.Find(lol[i]) != -1)
			{
				Sound(SOUND_LOL1);
				sound=true;
				break;
			}
		}

	}

	{
		const lol_sz=3;
		char * lol[lol_sz]= {"ЛОЛ","лол","Лол"};

		for(int i=0; i<lol_sz; i++)
		{
			if (recvstr.Find(lol[i]) != -1)
			{
				Sound(SOUND_LOL2);
				sound=true;
				break;
			}
		}

	}


	{
		const count=3;
		char * txt[count]= {"мля","Мля","МЛЯ"};

		for(int i=0; i<count; i++)
		{
			if (recvstr.Find(txt[i]) != -1)
			{
				Sound(SOUND_MLA);
				sound=true;
				break;
			}
		}
	}

	{
		const count=1;
		char * txt[count]= {"блабла"};

		for(int i=0; i<count; i++)
		{
			if (recvstr.Find(txt[i]) != -1)
			{
				Sound(SOUND_BLA);
				sound=true;
				break;
			}
		}
	}


	{
		const count=1;
		char * txt[count]= {":)"};

		for(int i=0; i<count; i++)
		{
			if (recvstr.Find(txt[i]) != -1)
			{
				Sound(SOUND_SOUND1);
				sound=true;
				break;
			}
		}
	}

	{
		const count=1;
		char * txt[count]= {":("};

		for(int i=0; i<count; i++)
		{
			if (recvstr.Find(txt[i]) != -1)
			{
				Sound(SOUND_SOUND2);
				sound=true;
				break;
			}
		}
	}

	{
		const count=1;
		char * txt[count]= {":|"};

		for(int i=0; i<count; i++)
		{
			if (recvstr.Find(txt[i]) != -1)
			{
				Sound(SOUND_SOUND3);
				sound=true;
				break;
			}
		}
	}

	{
		const count=1;
		char * txt[count]= {"ВИЗ"};

		for(int i=0; i<count; i++)
		{
			if (recvstr.Find(txt[i]) != -1)
			{
				Sound(SOUND_SOUND4);
				sound=true;
				break;
			}
		}
	}

	{
		const count=1;
		char * txt[count]= {"АХАСИ"};

		for(int i=0; i<count; i++)
		{
			if (recvstr.Find(txt[i]) != -1)
			{
				Sound(SOUND_SOUND5);
				sound=true;
				break;
			}
		}
	}

	{
		const count=1;
		char * txt[count]= {":-)"};

		for(int i=0; i<count; i++)
		{
			if (recvstr.Find(txt[i]) != -1)
			{
				Sound(SOUND_SOUND6);
				sound=true;
				break;
			}
		}
	}

    if (!sound) Sound(SOUND_MESSAGE1);

    int size=m_richedit.GetWindowTextLength();
    
	if (size>64000 && SettingBox.m_auto_clear)
    {
        SetWindowText("");
    }

    //на всякий случай
    if (recvstr.GetLength() == 0) return;


    CHARFORMAT cf=SystemSettings.cf;

    int textlen=m_richedit.GetTextLength();

    if (uid == SYSTEM_UID)
    {
        cf = SystemSettings.cf;
    }
    else if (uid == HISTORY_UID)
    {
        cf= HistorySettings.cf;
    }
    else if (uid == UnID)
    {
        cf = LocalSettings.cf;
    }
    else
    {

        //ищем соответствие uid c профилем пользователя
        for(int i=0; i<UserInfo.size(); i++)
        {
            if (UserInfo[i].RemoteSettings.uid == uid)
            {
                cf=UserInfo[i].RemoteSettings.cf;
                break;
            }
        }
    }

    CRichEditCtrl& rich=m_richedit;

    int l=rich.GetTextLength();

	//rich.HideCaret();
    rich.SetSel(l,l);

    Cursor.Erase();

    char LastSymbol=0;

    GetData;

    if (textlen>=2)
	{
        LastSymbol=m_richtext.Right(2)[0];
	}

    char FirstSymbol=recvstr[0];

    //если требутся данную строку поместить с новой строки
    if (WithNewLine)
    {
        CString z;

        WizSelectionCharFormat(rich, SystemSettings.cf);

        if (LastSymbol != 13 && LastSymbol!=10 && FirstSymbol!=13)
            z=ENDL;


        if (FirstSymbol!=13) z+=GetLocalTimeString();


        rich.ReplaceSel(z);
//		rich.ShowCaret();

    }

    if (recvstr[0]!='\b')
    {
		 int l=rich.GetTextLength();

		//rich.HideCaret();
		rich.SetSel(l,l);
        WizSelectionCharFormat(rich, cf);

        rich.ReplaceSel(recvstr);
        
		dialog->ScrollRich();
        
		Cursor.Draw();
    }

    else
    {


        CString str;
        rich.GetWindowText(str);
        CString k=str.Right(2);

        //не даем затирать предыдущую строку
        if (k[0]==13 || k[0]==10)
        {
            Cursor.Draw();
            return;
        }


        rich.SendMessage(WM_KEYDOWN, VK_BACK);
        Cursor.Draw();

    }

//	Cursor.Show();
//	int	l=rich.GetTextLength();
//	CPoint p=rich.GetCharPos(l);
//	p.x+=3;
//	p.y+=2;
//	rich.SetCaretPos(p);

}

void CECHO_TCP_IPDlg::OnRclickRichedit(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnMenuSettingBox();

    *pResult = 0;
}

void CECHO_TCP_IPDlg::OnDblclkRichedit(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here

    *pResult = 0;
}

void CECHO_TCP_IPDlg::OnRdblclkRichedit(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here

    *pResult = 0;
}


///CListBox
//
void CECHO_TCP_IPDlg::OnSelchangeUserList()
{
    int n=m_user_list.GetCurSel();
    CString str;
    m_user_list.GetText(n, str);

    //m_user_list.

    CString z;

    m_input.GetCaretPos();

    m_input.GetWindowText(z);
    
	if (AllSpace(z)==true) 
	{
		z=str+", ";
	}
    else 
	{
		z+=str+", " ;
	}
    
	m_input.SetWindowText(z);

    CHARFORMAT cf;
    cf=LocalSettings.cf;
    
	cf.yHeight=INPUT_HEI;
    
	m_input.SetBackgroundColor(false,BackGroundColor);
    
	m_input.SetSel(0,-1);
    
	WizSelectionCharFormat(m_input, cf);
    
	m_input.SetFocus();

    int k=m_input.GetTextLength();
    m_input.SetSel(k,k);

}

//отрисовка списка пользователей
void CECHO_TCP_IPDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT  di)
{
    if (di->CtlType==ODT_LISTBOX)
    {
        //DRAWITEMSTRUCT

        if (m_user_list.GetCount()==0) return;

        CUserSettings * user=(CUserSettings*) di->itemData;

        COLORREF bgcolor=RGB(0,0,0);

        CHARFORMAT ch=user->cf;
        LOGFONT lf= {0};

        lf.lfCharSet=ch.bCharSet;
        lf.lfHeight=LIST_ITEM_HEIGHT;
        lf.lfWidth=10;
        lf.lfPitchAndFamily=ch.bPitchAndFamily;
        lf.lfCharSet=DEFAULT_CHARSET;
        strcpy(lf.lfFaceName,ch. szFaceName);

        HFONT hfont=CreateFontIndirect(&lf);


        HFONT *oldfont=(HFONT*)SelectObject(di->hDC,hfont);

        HBRUSH hbr=CreateSolidBrush(bgcolor);
        HBRUSH hbr2=CreateSolidBrush(RGB(255,0,0));

        SetTextColor(di->hDC, user->cf.crTextColor);
        SetBkColor(di->hDC, bgcolor);

        RECT listrect;
        m_user_list.GetClientRect(&listrect);

        di->rcItem.left+=1;
        di->rcItem.right-=1;


        //di->rcItem.top+=1;
        di->rcItem.bottom-=1;

        //if (di->itemAction & ODA_SELECT)
        FillRect(di->hDC, &di->rcItem,hbr);
        //else
        //FillRect(di->hDC, &di->rcItem,hbr2);

        DrawText(di->hDC,user->NickName,strlen(user->NickName),&di->rcItem,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

        //CString s;
        //s.Format("%i", di->itemID);
        //DrawText(di->hDC,s,s.GetLength(),&di->rcItem,0);


        //нижнюю часть рисуем черным цветом

        int count=m_user_list.GetCount();

        /*if (di->itemID == count-1)
        {

        	int hei=abs(di->rcItem.top-di->rcItem.bottom);
        	listrect.top+=hei;
        	FillRect(di->hDC, &listrect,hbr2);
        }*/


        SelectObject(di->hDC,oldfont);

        DeleteObject(hbr);
        DeleteObject(hbr2);
        DeleteObject(hfont);
        return;
    }

//	CWnd


    CDialog::OnDrawItem(nIDCtl, di);
}

static DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
    CFile* pFile = (CFile*) dwCookie;

    pFile->Read(pbBuff, cb);
    *pcb = cb;

    return 0;
}

void CECHO_TCP_IPDlg::OnMenuLoadLog()
{
    CFileDialog d(TRUE, NULL,NULL,
                  0, //flags
                  "Logs file(*.rtf)|*.rtf"
                 );
    char buf[2048];
    GetCurrentDirectory(2047,buf);
//	CString path=buf;
//	path+="\\";
//	path+="Logs";
//	d.m_ofn.lpstrInitialDir=path;
    if (d.DoModal() == IDCANCEL) return;

    CString rtf=d.GetFileName();

    CFile rh(rtf, CFile::modeRead);
    EDITSTREAM es;
    es.dwCookie=(DWORD) &rh;
    es.pfnCallback=MyStreamInCallback;
    m_richedit.StreamIn(SF_RTF,es);

    rh.Close();

}


//////////////////////////////////////////////////////////////////////////



void CAboutDlg::OnMenuLoadLog()
{

}

void CECHO_TCP_IPDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT mis)
{

    if (mis->CtlType == ODT_LISTBOX)
    {
        //static k
        //MEASUREITEMSTRUCT item;

        //CUserSettings *user=(CUserSettings *)mis->itemData;


        mis->itemWidth=30;
        mis->itemHeight=LIST_ITEM_HEIGHT;//user->cf.yHeight/HEIGHT_MULTIPER;
        return;
    }

    CDialog::OnMeasureItem(nIDCtl, mis);
}

HBRUSH CECHO_TCP_IPDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


    if (nCtlColor== CTLCOLOR_LISTBOX)
    {
        int a=3;
        a++;

        static first=1;
        static HBRUSH hbr=0;
        if (first)
        {
            first=0;
            hbr=CreateSolidBrush(RGB(60,60,90));
            //нужно удалить после!!!
        }
        return hbr;
    }
    // TODO: Return a different brush if the default is not desired
    return hbr;
}

void CECHO_TCP_IPDlg::OnCrashServer()
{

    if (!ServerSocket)
    {
        if (UserInfo.size() != 0)
        {
            char buf[1]= {ESC_USER_EXIT};
            UserInfo[0].socket_connect->SendData(buf,1);

            DWORD id=345;
            UserInfo[0].socket_connect->SendData(&id,4);
            /*
            UserInfo[0].socket_connect->SendData(&id,4);
            UserInfo[0].socket_connect->SendData(&id,4);
            */
        }
    }
}

BOOL CAboutDlg::DestroyWindow()
{
    // TODO: Add your specialized code here and/or call the base class

    return CDialog::DestroyWindow();
}

BOOL CECHO_TCP_IPDlg::DestroyWindow()
{


    //MessageBox("Destroy");




    return CDialog::DestroyWindow();
}

LRESULT CECHO_TCP_IPDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

    if (message==WM_QUERYENDSESSION)
    {
        WriteLogFile();
        //MessageBox("END SESSION");
        return 1;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}
