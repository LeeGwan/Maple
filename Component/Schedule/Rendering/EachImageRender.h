#pragma once
#include "../../../pch.h"
#include "../../../Global.h"
#include "../Engine/AnimationType.h"
#include "../Engine/Animation/Animation.h"
#include "../Rendering/RenderingObject.h"
#include "Color/Color.h"
class EachImageRender
{

public:
	virtual ~EachImageRender() = default; 
	virtual void Update(Vec2& Pos,  double Direction)=0;
	virtual void Init(string Name, Vec2& Pos, vector<int> AnimationCout) = 0;
	
	virtual Vec2 Get_Size()=0;
};


