#pragma once
#include "../RenderingObject.h"
#include "../EachImageRender.h"
#include <map>
class MonsterRender : public EachImageRender
{
private:

	map<AnimationType, std::map<int, ID3D11ShaderResourceView*>> Animations;//애니메이션 맵
	//좌움직임
	//우움직임
	//점프
	Vec2 BeforeVec;
	float Speed;
	Vec2 Size;
	AnimationType Current_AnimationType_State;
	Animation Anim;
	double DmgTime;
	const double FixedTime = 0.16 / 10.0;
private:

	string ResourcePath[2];


	ID3D11ShaderResourceView* Resource[2];

	
	Vec2 TMP;

public:

	~MonsterRender() override;
	MonsterRender();
	void Init(string Name, Vec2& Pos, vector<int> AnimationCout)override;
	void Update(Vec2& Pos,double Direction)override;

	void Draw_Hp_Bar(Vec2& Pos,float MaxState,float State);
	AnimationType Get_Current_AnimationType_State();//키값 받아서 스테이트 바뀌게
	void Set_Current_AnimationType_State(AnimationType ChangeState);
	Vec2 Get_Size()override;

};

