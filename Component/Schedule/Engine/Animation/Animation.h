#pragma once
#include "../../../../pch.h"
#include "../AnimationType.h"

class Animation
{
private:
	int Current_Frame;
	int Limit_Frame;
	float ElapsedTime;
	AnimationType Before_State;
	bool TEST = true;

public:
	Animation();
	void Update_Animaion(AnimationType State,float deltaTime, int MaxFrames, float DistanceOrSpeed);

	void InitAnimations_Move(Vec2& Size, const std::string& Path,
		std::map<AnimationType, std::map<int, ID3D11ShaderResourceView*>>& RenderResource, vector<int> ImageCount);
	void LoadAnimationFrames(Vec2& Size, AnimationType type,
		const std::string& basePath, int frameCount,
		std::map<int, ID3D11ShaderResourceView*>& RenderResource);
	void InitAnimations_Skill(Vec2& Size, const std::string& Path, std::map<int, ID3D11ShaderResourceView*>& RenderResource, int FrameCount);
	bool Update_Animaion_For_Skill(float deltaTime, int MaxFrames);
	ID3D11ShaderResourceView* Start_Animation_Update_For_Skiil(std::map<int, ID3D11ShaderResourceView*>& RenderResource, bool* EndFrame,bool Temp=false);

	ID3D11ShaderResourceView* Start_Animation_Update(std::map<AnimationType, std::map<int, ID3D11ShaderResourceView*>>& RenderResource,  AnimationType State, float DistanceOrSpeed);
};

