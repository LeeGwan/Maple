#include <io.h>
#include <Shlwapi.h>
#include <WinInet.h>
#include <TlHelp32.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "wininet.lib")
#pragma warning(disable:4102)
#pragma warning(disable:4244)
#pragma warning(disable:4733)
#pragma warning(disable:4819)
#pragma warning(disable:4996)

COPYDATASTRUCT _xGlobalVar_CopyData;
LPCSTR _xGlobalStr_Contents[] = {
	"Login Both",
	"계정 또는 비밀번호 오류",
	"Register Success",
	"계정이 등록되었습니다.",
	"계정 등록 실패",
	"Login Success",
};
char _xGlobalStr_SiteUrl[0x00000260] = { 0x00, };
char* _xGlobalStr_lpBuffer = 0x00000000;
DWORD _xGlobalVar_dwNumberOfBytesToRead = 0x00000000;
void _xThread_MD5Hash(char* _xThreadParameters_pbData, DWORD _xThreadParameters_dwDataLen, char* _xThreadParameters_Save)
{
	HCRYPTPROV _xThreadVar_phProv;
	HCRYPTHASH _xThreadVar_phHash;
	BYTE _xThreadVar_pbData[0x00000010];
	DWORD _xThreadVar_pdwDataLen = 0x00000010;
	const char* _xThreadVar_Hex = "0123456789ABCDEF";

	if (!CryptAcquireContextA(&_xThreadVar_phProv, 0x00000000, MS_DEF_PROV_A, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return;
	if (!CryptCreateHash(_xThreadVar_phProv, CALG_MD5, 0x00000000, 0x00000000, &_xThreadVar_phHash))
		return;
	if (!CryptHashData(_xThreadVar_phHash, (BYTE*)_xThreadParameters_pbData, _xThreadParameters_dwDataLen, 0x00000000))
		return;
	if (!CryptGetHashParam(_xThreadVar_phHash, HP_HASHVAL, _xThreadVar_pbData, &_xThreadVar_pdwDataLen, 0x00000000))
		return;

	for (DWORD _xThreadVar_Index = 0x00000000; _xThreadVar_Index < _xThreadVar_pdwDataLen; _xThreadVar_Index++)
	{
		_xThreadParameters_Save[_xThreadVar_Index * 0x02] = _xThreadVar_Hex[_xThreadVar_pbData[_xThreadVar_Index] >> 0x04];
		_xThreadParameters_Save[(_xThreadVar_Index * 0x02) + 0x01] = _xThreadVar_Hex[_xThreadVar_pbData[_xThreadVar_Index] & 0x0F];
	}

	CryptReleaseContext(_xThreadVar_phProv, 0x00000000);
	CryptDestroyHash(_xThreadVar_phHash);
}
void _xThread_UrlRead(LPCSTR _xThreadParameters_SiteUrl, char* _xThreadParameters_lpBuffer)
{
	DWORD _xThreadVar_lpdwNumberOfBytesRead;

	HINTERNET _xThreadVar_hInternet = InternetOpenA(0x00000000, INTERNET_OPEN_TYPE_DIRECT, 0x00000000, 0x00000000, 0x00000000);
	if (_xThreadVar_hInternet)
	{
		HINTERNET _xThreadVar_hFile = InternetOpenUrlA(_xThreadVar_hInternet, _xThreadParameters_SiteUrl, 0x00000000, 0x00000000, INTERNET_FLAG_RELOAD | INTERNET_FLAG_TRANSFER_BINARY, 0x00000000);
		if (_xThreadVar_hFile)
			InternetReadFile(_xThreadVar_hFile, _xThreadParameters_lpBuffer, 0x00001000, &_xThreadVar_lpdwNumberOfBytesRead);

		_xThreadParameters_lpBuffer[_xThreadVar_lpdwNumberOfBytesRead] = 0x00000000;

		InternetCloseHandle(_xThreadVar_hFile);
	}
}

void _xThread_UrlPageRead(LPCSTR _xThreadParameters_SiteUrl)
{
	HINTERNET _xThreadVar_hInternet = InternetOpenA(0x00000000, INTERNET_OPEN_TYPE_DIRECT, 0x00000000, 0x00000000, 0x00000000);
	if (_xThreadVar_hInternet)
	{
		DWORD _xThreadVar_lpdwBufferLength = sizeof(DWORD);
		DWORD _xThreadVar_dwNumberOfBytesToRead = 0x00000000;
		DWORD _xThreadVar_lpdwNumberOfBytesRead = 0x00000000;

		HINTERNET _xThreadVar_hFile = InternetOpenUrlA(_xThreadVar_hInternet, _xThreadParameters_SiteUrl, 0x00000000, 0x00000000, INTERNET_FLAG_RELOAD | INTERNET_FLAG_TRANSFER_BINARY, 0x00000000);
		if (_xThreadVar_hFile)
		{
			HttpQueryInfoA(_xThreadVar_hFile, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_CONTENT_LENGTH, &_xThreadVar_dwNumberOfBytesToRead, &_xThreadVar_lpdwBufferLength, 0x00000000);
			_xGlobalStr_lpBuffer = (char*)VirtualAllocEx(GetCurrentProcess(), 0x00000000, _xThreadVar_dwNumberOfBytesToRead, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			InternetReadFile(_xThreadVar_hFile, _xGlobalStr_lpBuffer, _xThreadVar_dwNumberOfBytesToRead, &_xThreadVar_lpdwNumberOfBytesRead );
			_xGlobalVar_dwNumberOfBytesToRead = _xThreadVar_dwNumberOfBytesToRead;
		
		}

		InternetCloseHandle(_xThreadVar_hInternet);
		InternetCloseHandle(_xThreadVar_hFile);
	}
}