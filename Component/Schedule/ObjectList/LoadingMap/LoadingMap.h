#pragma once
#include "../../../../pch.h"
#include "../../Engine/Animation/Animation.h"
#include "../../Engine/AnimationType.h"
class LoadingMap
{
private:

	std::string InitAnimation_Path;
	std::map<int, ID3D11ShaderResourceView* > InitAnimation1;
	Animation InitAnim;

	Vec2 Temp;

public:
	
	LoadingMap();
	~LoadingMap();
	void init();
	void Update(bool* CheckFinish);
};

