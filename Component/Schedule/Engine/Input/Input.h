#pragma once
#include "../../../../pch.h"
#include "../../ObjectList/ObjectList.h"
class Input
{
private:
	queue<int> Key;
public:
	double Input_KeyBoard(ObjectClass* Obj,double DeltaTime);
	bool Input_SKillKeyBoard(int key);
	
};

