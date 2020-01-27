// ECHO_TCP_IPDlg.h : header file
//

#if !defined(AFX_ECHO_TCP_IPDLG_H__9DA49768_2930_4C95_9273_E89E22646480__INCLUDED_)
#define AFX_ECHO_TCP_IPDLG_H__9DA49768_2930_4C95_9273_E89E22646480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "settingbox.h"
#include "usersettings.h"

#include "xclistbox.h"

#define GetData UpdateData(true)

/////////////////////////////////////////////////////////////////////////////
// CECHO_TCP_IPDlg dialog

//const ESCAPE_CODE=4;	//код принятия строки, далее следует байт длины строки
const OLD_ESC_CODE			=	2;	//старый код, соединения должно быть разорвано
const ESCAPE_CODE			=	3;	//код, после принятии которого нужно считать структуру информации о пользователе
const ESC_SEND_UID_STRING	=	4;	//посылка строки текста с идентификатором пользователя, который посылает эту строку
const ESC_USER_EXIT			=	5;

#define SYSTEM_UID 0xFFFFFFFF
#define HISTORY_UID 0xFFFFFFFE
#define SERVER_UID 0x00000000


class CECHO_TCP_IPDlg : public CDialog
{
// Construction
public:
    void Sound(int type,int stop);
    void RefreshUserList();
    void SynchroMenuButtons();
    void SetTitle();
    void ChatWindowAddStringUID(CString recvstr, DWORD uid, bool WithNewLine=true);
    void InfoSend(int source);
    int OnConnect();
    void OnRemoteClose(int err, DWORD sock);
    void OnClientAccept(int err);
    void OnToServerConnect(int err,DWORD sock);
//	void OnClientConnect(int err);
    void EnableClientServerButtons(bool enable);
    void OnRemoteInfoReceive(int sender, CUserSettings IncomingRemote);
    void InfoExchange();
    void OnReceive(int err, DWORD socket);
    void OnReceive(const CString& k, DWORD sock);
//	void OnRichTest(char, COLORREF=0, bool Client_Server=false);
    void OnRunServer();
//	void OnRichTest(const CString &s, COLORREF c=0, bool Client_Server=false);
    void WriteLogFile();
    CECHO_TCP_IPDlg(CWnd* pParent = NULL);	// standard constructor

    CSettingBox SettingBox;
// Dialog Data
    //{{AFX_DATA(CECHO_TCP_IPDlg)
    enum { IDD = IDD_ECHO_TCP_IP_DIALOG };
    xClistBox	m_user_list;
    CRichEditCtrl	m_input;
    CButton	m_run_server;
    CButton	m_connect;
    CEdit	m_input_box;
    CRichEditCtrl	m_richedit;
    CButton	m_send;
    CString	m_richtext;
    CString	m_inputtext;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CECHO_TCP_IPDlg)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL DestroyWindow();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

// Implementation
//protected:
public:
    ~CECHO_TCP_IPDlg();
    bool m_hide;
    void DeleteIcon();
    void AddIcon();
    void SavePlacement();
    HICON hIcon;
    void ScrollRich();
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CECHO_TCP_IPDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    afx_msg void OnSend();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
    afx_msg void OnClose();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMenuConnect();
    afx_msg void OnMenuRunServer();
    afx_msg void OnMenuSettingBox();
    afx_msg void OnFileExit();
    afx_msg void OnSetfocusInput(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickInput(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnKillfocusInputBox();
    afx_msg void OnSetfocusInputBox();
    afx_msg void OnReturnInput(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMenuStopServer();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnAbout();
    afx_msg void OnDestroy();
    afx_msg void OnFileSend();
    afx_msg void OnClearLog();
    afx_msg void OnCrash();
    afx_msg void OnDisconnect();
    afx_msg void OnClickRichedit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRclickRichedit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkRichedit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRdblclkRichedit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeUserList();
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMenuLoadLog();
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnCrashServer();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECHO_TCP_IPDLG_H__9DA49768_2930_4C95_9273_E89E22646480__INCLUDED_)
