#ifndef H_USER_SETTINGS_H
#define H_USER_SETTINGS_H

extern DWORD UnID;

//��������� ������������
struct CUserSettings
{
    int size;	//������ ���������(������� �������, ������)
    CHARFORMAT cf;		//�����
    enum {NMAX=16};
    char NickName[NMAX];//���
    DWORD uid;	//���������� �������������
    CUserSettings() : size(sizeof(CUserSettings)), uid(UnID)
    {
        strcpy(NickName, "noname user");
        ZeroMemory(&cf,sizeof(cf));
        cf.cbSize=sizeof(cf);
    }
};

#endif
