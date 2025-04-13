#pragma once
#include"../../ObjectList/ObjectList.h"

class Collision
{
private:

private:
	ObjectClass* Find_Near_Obj_For_Player(Vec2 Pos, float Radius);
	ObjectClass* Find_Near_Obj_For_Monster(Vec2 Pos, float Radius);
public:
	ObjectClass* Check_Collision(ObjectClass* MyObj, float Radius);
	std::vector<ObjectClass*> Check_Collision(Vec2 Pos, float Radius);

	
};

