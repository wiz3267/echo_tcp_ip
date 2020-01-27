#ifndef __SOCKETS__H
#define __SOCKETS__H

#include <windows.h>
#include "stdafx.h"
#include "ECHO_TCP_IPdlg.h"
#include <Afxsock.h>

extern CECHO_TCP_IPDlg * dialog;

class CClientSocket : public CAsyncSocket
{
public:
    void SendData(const void * buf, int BufLen)
    {
        LPBYTE buffer=(LPBYTE) buf;
        for(int r=0; r<20; r++)
        {
            int k=Send(buffer,BufLen);
            if (k!=SOCKET_ERROR)
            {
                BufLen-=k;
                if (BufLen)
                {
                    Sleep(10);
                    buffer+=k;
                    dialog->ChatWindowAddStringUID("Warning: при отправке данных на сокет произошел разрыв потока\r\n",SYSTEM_UID);
                    continue;
                }
            }
            else
            {
                dialog->ChatWindowAddStringUID("Фатальная ошибка: невозможно отправить данные в сокет\r\n",SYSTEM_UID);
                Close();
                break;
            }
        }
    }

    void OnClose(int er)
    {
        dialog->OnRemoteClose(er, DWORD(this));
    }

    //ответ на запрос соедиения с сервером
    void OnConnect(int err)
    {
        dialog->OnToServerConnect(err, DWORD(this));
    }

    void OnReceive(int err)
    {
        dialog->OnReceive(err, DWORD(this));
    }
public:
    void SendString(const CString& s)
    {
        if (!this) return;
        //if (ConvertOemToAnsi) s.AnsiToOem();
        SendData(LPCSTR(s), s.GetLength());
    }

    //посылаем строку с информацией о пользователе
    //uid =
    //0 - сервер
    //
    void SendStringWithUID(const CString &s, DWORD uid)
    {
        LPBYTE puid = (LPBYTE) &uid;

        WORD size=s.GetLength();

        LPBYTE psize = (LPBYTE) &size;

        WORD total=1+2+4+size;

        BYTE * buffer=new BYTE[total];

        buffer[0]=ESC_SEND_UID_STRING;

        buffer[1]=psize[0];
        buffer[2]=psize[1];

        buffer[3]=puid[0];
        buffer[4]=puid[1];
        buffer[5]=puid[2];
        buffer[6]=puid[3];

        for(int i=0; i<size; i++)
        {
            buffer[7+i]=s[i];
        }

        //int err=
        SendData(buffer,total);

        /*
        if (err==)
        обработка ошибок
        */

        delete buffer;

    }

};

class CServerSocket : public CAsyncSocket
{
    void OnAccept(int errCode)
    {
        dialog->OnClientAccept(errCode);
    }
public:

};


#endif
