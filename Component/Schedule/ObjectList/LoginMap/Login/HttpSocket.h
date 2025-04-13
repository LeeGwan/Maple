#pragma once
class HttpSocket
{
private:
	char G_ThreadStr_ID[0x00001000];
	char G_ThreadStr_pw[0x00001000];
public:
	bool Register(char* _xGlobalStr_Account, char* _xGlobalStr_Password);
	bool Login(char* _xGlobalStr_Account, char* _xGlobalStr_Password);
	int Get_Level();
	void Set_Level(int Lv);
};

