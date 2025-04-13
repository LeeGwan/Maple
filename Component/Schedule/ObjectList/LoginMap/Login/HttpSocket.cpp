#include "HttpSocket.h"
#include"xbase.h"
#include<iostream>
bool HttpSocket::Register(char* _xGlobalStr_Account, char* _xGlobalStr_Password)
{
    char _xThreadStr_SiteUrl[0x00001000] = { 0x00, };
    char _xThreadStr_pw[0x00001000] = { 0x00, };
    strcpy(_xThreadStr_pw, _xGlobalStr_Password);
    int _xThreadVar_PasswordLength = strlen(_xThreadStr_pw);
    _xThread_MD5Hash(_xGlobalStr_Password, _xThreadVar_PasswordLength, _xThreadStr_pw);
    sprintf(_xGlobalStr_SiteUrl, "http://testservers.dothome.co.kr/web/games/kdf/register1.php?account=%s&password=%s", _xGlobalStr_Account, _xThreadStr_pw);

    _xThread_UrlRead(_xGlobalStr_SiteUrl, _xThreadStr_SiteUrl);
    if (strcmp((const char*)_xThreadStr_SiteUrl, _xGlobalStr_Contents[0x00000002]) == 0x00000000)
    {
        MessageBoxA(NULL, _xGlobalStr_Contents[0x00000002], "", MB_OK | MB_ICONINFORMATION);
        return true;
    }
    else
    {
        MessageBoxA(NULL, _xGlobalStr_Contents[0x00000004], "", MB_OK | MB_ICONERROR);
        return false;
    }
}

bool HttpSocket::Login(char* _xGlobalStr_Account, char* _xGlobalStr_Password)
{
    register char _xThreadStr_SiteUrl[0x00001000] = { 0x00, };
    char _xThreadStr_pw[0x00001000] = { 0x00, };
    int _xThreadVar_PasswordLength = strlen(_xGlobalStr_Password);
    _xThread_MD5Hash(_xGlobalStr_Password, _xThreadVar_PasswordLength, _xThreadStr_pw);
    sprintf(_xGlobalStr_SiteUrl, "http://testservers.dothome.co.kr/web/games/kdf/login1.php?account=%s&password=%s", _xGlobalStr_Account, _xThreadStr_pw);//원래는 암호화 해야함
    _xThread_UrlRead(_xGlobalStr_SiteUrl, _xThreadStr_SiteUrl);
    if (!strcmp((const char*)_xThreadStr_SiteUrl, _xGlobalStr_Contents[5]))//로그인성공
    {
        strcpy(G_ThreadStr_ID, _xGlobalStr_Account);
        strcpy(G_ThreadStr_pw, _xGlobalStr_Password);
        return true;
    }
    else
    {
        MessageBoxA(NULL, (const char*)_xThreadStr_SiteUrl, "", MB_OK | MB_ICONERROR);
        return false;
    }
    
}

int HttpSocket::Get_Level()
{
    register char _xThreadStr_SiteUrl[0x00001000] = { 0x00, };
    char _xThreadStr_pw[0x00001000] = { 0x00, };
    int _xThreadVar_PasswordLength = strlen(G_ThreadStr_pw);
    _xThread_MD5Hash(G_ThreadStr_pw, _xThreadVar_PasswordLength, _xThreadStr_pw);
    sprintf(_xGlobalStr_SiteUrl, "http://testservers.dothome.co.kr/web/games/kdf/get_level.php?account=%s&password=%s", G_ThreadStr_ID, _xThreadStr_pw);//원래는 암호화 해야함
    _xThread_UrlRead(_xGlobalStr_SiteUrl, _xThreadStr_SiteUrl);
    return static_cast<int>(_xThreadStr_SiteUrl[0]) - 48;
   
}

void HttpSocket::Set_Level(int Lv)
{
    register char _xThreadStr_SiteUrl[0x00001000] = { 0x00, };
    char _xThreadStr_pw[0x00001000] = { 0x00, };
    int _xThreadVar_PasswordLength = strlen(G_ThreadStr_pw);
    _xThread_MD5Hash(G_ThreadStr_pw, _xThreadVar_PasswordLength, _xThreadStr_pw);
    sprintf(_xGlobalStr_SiteUrl, "http://testservers.dothome.co.kr/web/games/kdf/set_level.php?account=%s&password=%s&level=%d", G_ThreadStr_ID, _xThreadStr_pw, Lv);//원래는 암호화 해야함
    _xThread_UrlRead(_xGlobalStr_SiteUrl, _xThreadStr_SiteUrl);
   
}
