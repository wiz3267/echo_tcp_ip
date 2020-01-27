#ifndef H_USER_SETTINGS_H
#define H_USER_SETTINGS_H

extern DWORD UnID;

//настройки пользователя
struct CUserSettings
{
    int size;	//размер структуры(другими словами, версия)
    CHARFORMAT cf;		//шрифт
    enum {NMAX=16};
    char NickName[NMAX];//ник
    DWORD uid;	//уникальный идентификатор
    CUserSettings() : size(sizeof(CUserSettings)), uid(UnID)
    {
        strcpy(NickName, "noname user");
        ZeroMemory(&cf,sizeof(cf));
        cf.cbSize=sizeof(cf);
    }
};

#endif
