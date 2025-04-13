#pragma once
#include "../RenderingObject.h"
#include "../EachImageRender.h"
#include <map>
class MonsterRender : public EachImageRender
{
private:

	map<AnimationType, std::map<int, ID3D11ShaderResourceView*>> Animations;//�ִϸ��̼� ��
	//�¿�����
	//�������
	//����
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
	AnimationType Get_Current_AnimationType_State();//Ű�� �޾Ƽ� ������Ʈ �ٲ��
	void Set_Current_AnimationType_State(AnimationType ChangeState);
	Vec2 Get_Size()override;

};

