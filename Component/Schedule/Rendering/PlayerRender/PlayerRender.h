#include "../RenderingObject.h"
#include "../EachImageRender.h"
#include <map>
class PlayerRender : public EachImageRender
{
private:

	map<AnimationType, std::map<int, ID3D11ShaderResourceView*>> Animations;//애니메이션 맵
	//좌움직임
	//우움직임
	//점프
	Vec2 BeforeVec;

	Vec2 Size;
	AnimationType Current_AnimationType_State;
	Animation Anim;
	
private:
	string Path[4];
	string LevelNumPath[10];
	string HpNumPath[10];
	string ExpNumPath[10];
	string SlashPath;
	string NamePath;
	ID3D11ShaderResourceView* NameResource;
	ID3D11ShaderResourceView* BarResource[4];
	ID3D11ShaderResourceView* levelDigitsTex[10];
	ID3D11ShaderResourceView* HpDigitsTex[10];
	ID3D11ShaderResourceView* ExpDigitsTex[10];
	ID3D11ShaderResourceView* SlashResource;
	Vec2 TMP;
private:
	std::map<int, ID3D11ShaderResourceView* > LevelUpAnimation;
	Animation LevelUpAnim;
	Vec2 TMP1;
public:

	virtual ~PlayerRender() override = default;
	PlayerRender();
	void Draw_Lv(int Lv);
	void Draw_Hp_Bar(float MaxState, float State);
	void Draw_Mp_Bar(float MaxState, float State);
	void Draw_LevelUp(Vec2 Pos,bool* Is_Level_Up);
	void Draw_Xp_Bar(float MaxState, float State);
	void Init(string Name, Vec2& Pos, vector<int> AnimationCout)override;

	void Draw_Name(Vec2 Pos,const wstring& Name);
	
	void Update(Vec2& Pos, double Direction)override;

	AnimationType Get_Current_AnimationType_State();//키값 받아서 스테이트 바뀌게
	void Set_Current_AnimationType_State(AnimationType ChangeState);
	Vec2 Get_Size()override;


};


