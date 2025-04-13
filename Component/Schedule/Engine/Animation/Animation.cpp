#include "Animation.h"
#include "../../../../Global.h"
#include "../../Rendering/RenderingObject.h"
#include <algorithm>
#undef min //<--넣어야 함
#undef max //<--넣어야 함
Animation::Animation()
{
    ElapsedTime = 0.0;
    Current_Frame = 0;
    Limit_Frame = 0;

}
void Animation::Update_Animaion(AnimationType State,float deltaTime, int MaxFrames, float DistanceOrSpeed)
{
    if (MaxFrames <= 0)
    {
        ElapsedTime = 0.0;
        return;
    }
    float animationSpeed = State == AnimationType::Attack   ? clamp(DistanceOrSpeed, 0.2f, 2.0f) : clamp(DistanceOrSpeed, 2.0f, 5.0f);//최대 최소 값 만 나오게 하는 게 clamp 다
    // distance에 따라 애니메이션 속도 조절

    float frameDuration = 0.16f* animationSpeed; /// animationSpeed;  // 프레임당 시간 (작을수록 빠름)

    ElapsedTime += deltaTime;

    if (ElapsedTime >= frameDuration)
    {
        Current_Frame = (Current_Frame + 1) % MaxFrames;
        ElapsedTime -= frameDuration; // 남은 시간 유지 (정확한 시간 누적)
    }
}

void Animation::InitAnimations_Move(Vec2& Size, const std::string& Path,
    std::map<AnimationType, std::map<int, ID3D11ShaderResourceView*>>& RenderResource,vector<int> ImageCount)
{
    const std::map<AnimationType, std::pair<std::string, int>> animationData = {
        {AnimationType::Stand,  {Path + "Stand\\", ImageCount[0]}},
        {AnimationType::Alert,  {Path + "Alert\\", ImageCount[1] }},
        {AnimationType::Move,   {Path + "Move\\", ImageCount[2]}},
        {AnimationType::Jump,  {Path + "Jump\\", ImageCount[3]}},
        {AnimationType::Attack,  {Path + "Attack\\", ImageCount[4]}},

        {AnimationType::Die,  {Path + "Die\\", ImageCount[5]}},
           {AnimationType::Hit,  {Path + "Hit\\", ImageCount[6]}},


    };

    for (const auto& anim : animationData) {
        if (anim.second.second == 0)continue;

        LoadAnimationFrames(Size, anim.first, anim.second.first, anim.second.second, RenderResource[anim.first]);
    }
}
void Animation::LoadAnimationFrames(Vec2& Size, AnimationType type,
    const std::string& basePath, int frameCount,
    std::map<int, ID3D11ShaderResourceView*>& RenderResource)
{
    const std::string pngExt = ".png";

    for (int i = 0; i < frameCount; i++)
    {
       
        std::string filePath = basePath + std::to_string(i) + pngExt;
        RenderResource[i] = nullptr;  // 초기화
        G_ctx->MakeRender->LoadTextureFromFileDX11(filePath.c_str(), &RenderResource[i], Size);
    }
}
void Animation::InitAnimations_Skill(Vec2& Size, const std::string& Path,
    std::map<int, ID3D11ShaderResourceView*>& RenderResource, int FrameCount)
{
    const std::string pngExt = ".png";
    for (int i = 0; i < FrameCount; i++)
    {
        std::string FullPath = Path + to_string(i) + pngExt;
        
        G_ctx->MakeRender->LoadTextureFromFileDX11(FullPath.c_str(), &RenderResource[i], Size);
    }


}
bool Animation::Update_Animaion_For_Skill(float deltaTime, int MaxFrames)
{
    if (MaxFrames <= 0)
    {
        ElapsedTime = 0.0f;
        return true;
    }
    float frameDuration;
  
    float baseFrameDuration = 0.16f * 0.6f; 
    if (MaxFrames <= 7)
    {
        frameDuration = (baseFrameDuration * MaxFrames) * 1.1f;
    }
    else
    {
        frameDuration = baseFrameDuration;
    }
    
 

    ElapsedTime += deltaTime;
    if (ElapsedTime >= frameDuration)
    {
        ElapsedTime -= frameDuration;
        Current_Frame++;

        if (Current_Frame >= MaxFrames)
        {
            return false; // 애니메이션 끝
        }
    }

    return true; // 아직 진행 중
}


ID3D11ShaderResourceView* Animation::Start_Animation_Update_For_Skiil(
   std::map<int, ID3D11ShaderResourceView*>& RenderResource, bool* EndFrame,bool Temp
)
{
    
    float delta = 0.016f;

    bool endFlag=Update_Animaion_For_Skill(delta, RenderResource.size());

    if (!endFlag)
    {
        ElapsedTime = 0.0;
        Current_Frame = 0;
        Limit_Frame = 0;
        *EndFrame = endFlag;
        TEST = true;
        return nullptr;
    }
        // 보간 비율 계산 (ElapsedTime / frameDuration)
        float animationSpeed = 1.0f  * 0.1f;
        float frameDuration = 0.2f / animationSpeed;
        float blendRatio = ElapsedTime / frameDuration; // 0 ~ 1
        if (TEST &&Temp&& Current_Frame==27)
        {
            std::string Path = SkillMusicPath + std::string("Q_1.mp3");
            G_ctx->G_SoundList.PlayEffectSound(Path, 3.0, 0.7f);
            TEST = false;
        }
        if (RenderResource.count(Current_Frame))
            return RenderResource[Current_Frame];
      
    

    return nullptr;
}

ID3D11ShaderResourceView* Animation::Start_Animation_Update(
    std::map<AnimationType, std::map<int, ID3D11ShaderResourceView*>>& RenderResource,
    AnimationType State,
    float DistanceOrSpeed)
{
  
  
    if (Before_State != State) {
        Current_Frame = 0;
        ElapsedTime = 0.0f; // 상태 바뀌면 시간도 초기화
    }

    auto it = RenderResource.find(State);
    if (it != RenderResource.end()) {
        Limit_Frame = (State == AnimationType::Jump)
            ? static_cast<int>(it->second.size())   -1
            : static_cast<int>(it->second.size()) ;

        float delta = 0.016f; // 고정된 프레임당 시간 (예: 60FPS 기준)
        Update_Animaion(State,delta, Limit_Frame, DistanceOrSpeed);

        // 보간 비율 계산 (ElapsedTime / frameDuration)
        float animationSpeed = 1.0f + DistanceOrSpeed * 0.1f;
        float frameDuration = 0.2f / animationSpeed;
        float blendRatio = ElapsedTime / frameDuration; // 0 ~ 1
      
        

        for (const auto& FrameRect : it->second) {
            if (FrameRect.first == Current_Frame) {
                Before_State = State;
                return FrameRect.second;
            }
        }
    }

    return nullptr;
}


