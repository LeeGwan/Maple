#pragma once
#include "../../../../pch.h"

#include "../../Engine/AnimationType.h"
#include "../../Rendering/DrawManager.h"
#include "../LoadingMap/LoadingMap.h"
class LoginMap
{
private:
	std::string LoginUIPath;
	std::string LoginBackPath;
	ID3D11ShaderResourceView* LoginBackground;
	ID3D11ShaderResourceView* LoginUi;
	Vec2 Temp;
private:
	LoadingMap LoadingMaps;
private:
	char _xGlobalStr_Account[0x00000022] = { 0x00, };
	char _xGlobalStr_Password[0x00000022] = { 0x00, };
private:
	bool CheckSound;
	bool IsFinish;
public:
	LoginMap();
	~LoginMap();
	void init();
	void Update(bool* Islogin);
};

