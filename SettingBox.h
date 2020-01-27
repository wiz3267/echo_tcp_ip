#if !defined(AFX_SETTINGBOX_H__A3D24E2A_B95D_4541_9843_F2A945837398__INCLUDED_)
#define AFX_SETTINGBOX_H__A3D24E2A_B95D_4541_9843_F2A945837398__INCLUDED_

#include "ECHO_TCP_IPDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingBox.h : header file
//

//#include "sockets.h"


#include "UserSettings.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingBox dialog

class CSettingBox : public CDialog
{
// Construction
public:
    bool TryConnect;
    void SetColorBox();
    void InitButtons(bool k,bool j);
    CUserSettings GetUserSettings();
    void CorrectColorFontName();

    CString m_cremote_ip;	//IP address remote control as CString
    DWORD m_remote_ip;		//Remote IP in pack-format
    COLORREF MyColor;
    //void InfoSend(CClientSocket * client);
    bool WriteSettings();
    bool LoadSettings();
    CSettingBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSettingBox)
    enum { IDD = IDD_SETTINGBOX };
    CSliderCtrl	m_slider_font_size;
    CEdit	m_nick_win;
    CButton	m_server_stop;
    CEdit	m_port_edit;
    CButton	m_connect;
    CButton	m_run_server;
    CRichEditCtrl	m_color_box;
    CString	m_greeting;
    CString	m_nick_name;
    BOOL	m_connect_on_start;
    BOOL	m_run_server_on_start;
    DWORD	m_port;
    CString	m_font_face;
    BOOL	m_load_on_system_start;
    BOOL	m_sound_enable;
    CString	m_ip_server;
    int		m_font_size;
    BOOL	m_no_autoscroll;
    BOOL	m_system_message;
    BOOL	m_auto_clear;
    BOOL	m_auto_chat_show;
    BOOL	m_enable_write_log_file;
    //}}AFX_DATA
    int DoModal(bool initButtons=true);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSettingBox)

public:

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSettingBox)
    virtual void OnOK();
    afx_msg void OnSelectColor();
    afx_msg void OnSelectFont();
    virtual BOOL OnInitDialog();
    afx_msg void OnConnect();
    afx_msg void OnServerRun();
    afx_msg void OnConnectOnStart();
    afx_msg void OnRunServerOnStart();
    afx_msg void OnServerStop();
    afx_msg void OnUpdateFontSize();
    afx_msg void OnCustomdrawSliderFontSize(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGBOX_H__A3D24E2A_B95D_4541_9843_F2A945837398__INCLUDED_)
