#include "HttpSocket.h"
#include "xbase.h"
#include <iostream>

// -----------------------------------------
// 회원가입 요청 처리
// -----------------------------------------
bool HttpSocket::Register(char* _xGlobalStr_Account, char* _xGlobalStr_Password)
{
    char _xThreadStr_SiteUrl[4096] = { 0 };  // 응답 저장용 버퍼
    char _xThreadStr_pw[4096] = { 0 };       // 해싱된 비밀번호

    // 비밀번호 복사 및 MD5 해싱
    strcpy(_xThreadStr_pw, _xGlobalStr_Password);
    int passwordLen = strlen(_xThreadStr_pw);
    _xThread_MD5Hash(_xGlobalStr_Password, passwordLen, _xThreadStr_pw);

    // 회원가입 요청 URL 생성
    sprintf(_xGlobalStr_SiteUrl,
        "http://testservers.dothome.co.kr/web/games/kdf/register1.php?account=%s&password=%s",
        _xGlobalStr_Account, _xThreadStr_pw);

    // 서버로 요청 전송 및 결과 수신
    _xThread_UrlRead(_xGlobalStr_SiteUrl, _xThreadStr_SiteUrl);

    // 응답 값 비교로 성공 여부 판단
    if (strcmp((const char*)_xThreadStr_SiteUrl, _xGlobalStr_Contents[2]) == 0)
    {
        MessageBoxA(NULL, _xGlobalStr_Contents[2], "", MB_OK | MB_ICONINFORMATION);
        return true;
    }
    else
    {
        MessageBoxA(NULL, _xGlobalStr_Contents[4], "", MB_OK | MB_ICONERROR);
        return false;
    }
}

// -----------------------------------------
// 로그인 요청 처리
// -----------------------------------------
bool HttpSocket::Login(char* _xGlobalStr_Account, char* _xGlobalStr_Password)
{
    char _xThreadStr_SiteUrl[4096] = { 0 };
    char _xThreadStr_pw[4096] = { 0 };

    // 비밀번호 해시화
    int passwordLen = strlen(_xGlobalStr_Password);
    _xThread_MD5Hash(_xGlobalStr_Password, passwordLen, _xThreadStr_pw);

    // 로그인 요청 URL 생성
    sprintf(_xGlobalStr_SiteUrl,
        "http://testservers.dothome.co.kr/web/games/kdf/login1.php?account=%s&password=%s",
        _xGlobalStr_Account, _xThreadStr_pw);

    // 서버 응답 수신
    _xThread_UrlRead(_xGlobalStr_SiteUrl, _xThreadStr_SiteUrl);

    // 응답이 로그인 성공 문자열인지 확인
    if (strcmp((const char*)_xThreadStr_SiteUrl, _xGlobalStr_Contents[5]) == 0)
    {
        // 로그인 성공 → 계정 정보 저장
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

// -----------------------------------------
// 현재 계정의 레벨 조회
// -----------------------------------------
int HttpSocket::Get_Level()
{
    char _xThreadStr_SiteUrl[4096] = { 0 };
    char _xThreadStr_pw[4096] = { 0 };

    int passwordLen = strlen(G_ThreadStr_pw);
    _xThread_MD5Hash(G_ThreadStr_pw, passwordLen, _xThreadStr_pw);

    // 요청 URL 생성
    sprintf(_xGlobalStr_SiteUrl,
        "http://testservers.dothome.co.kr/web/games/kdf/get_level.php?account=%s&password=%s",
        G_ThreadStr_ID, _xThreadStr_pw);

    _xThread_UrlRead(_xGlobalStr_SiteUrl, _xThreadStr_SiteUrl);

    // 응답의 첫 문자를 숫자로 변환해 레벨로 반환
    return static_cast<int>(_xThreadStr_SiteUrl[0]) - '0';
}

// -----------------------------------------
// 현재 계정의 레벨 서버에 저장
// -----------------------------------------
void HttpSocket::Set_Level(int Lv)
{
    char _xThreadStr_SiteUrl[4096] = { 0 };
    char _xThreadStr_pw[4096] = { 0 };

    int passwordLen = strlen(G_ThreadStr_pw);
    _xThread_MD5Hash(G_ThreadStr_pw, passwordLen, _xThreadStr_pw);

    // 요청 URL 생성
    sprintf(_xGlobalStr_SiteUrl,
        "http://testservers.dothome.co.kr/web/games/kdf/set_level.php?account=%s&password=%s&level=%d",
        G_ThreadStr_ID, _xThreadStr_pw, Lv);

    _xThread_UrlRead(_xGlobalStr_SiteUrl, _xThreadStr_SiteUrl);
}
