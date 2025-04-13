
#include "../EachImageRender.h"
#include <string>
//월요일에 할거 
class BackGroundRender:public EachImageRender
{
private:
	
	string Path;
	string MainBarName;
	string ExpBarName;
	ID3D11ShaderResourceView* BackGroundResource;
	ID3D11ShaderResourceView* MainBarResource;
	ID3D11ShaderResourceView* ExpBarResource;
	Vec2 Size;
	Vec2 MainBar_2D;
	Vec2 ExpBarBar_2D;
private:

public:
	
	~BackGroundRender() override;
	BackGroundRender();
	void Init(string Name, Vec2& Pos, vector<int> AnimationCout)override;

	void Update(Vec2& Pos, double Direction)override;
	Vec2 Get_Size()override;

};