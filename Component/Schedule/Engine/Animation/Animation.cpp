#include "Animation.h"
#include "../../../../Global.h"
#include "../../Rendering/RenderingObject.h"
#include <algorithm>
#include "Animation.h"
#include "../../../../Global.h"
#include "../../Rendering/RenderingObject.h"
#include <algorithm>

#undef min // Windows 매크로로 정의된 min/max 제거
#undef max

// 생성자 - 초기값 설정
Animation::Animation()
{
    ElapsedTime = 0.0;
    Current_Frame = 0;
    Limit_Frame = 0;
}

// 애니메이션 프레임 갱신 (상태별)
void Animation::Update_Animaion(AnimationType State, float deltaTime, int MaxFrames, float DistanceOrSpeed)
{
    if (MaxFrames <= 0)
    {
        ElapsedTime = 0.0;
        return;
    }

    // 상태에 따라 애니메이션 속도 결정
    float animationSpeed = (State == AnimationType::Attack)
        ? clamp(DistanceOrSpeed, 0.2f, 2.0f)
        : clamp(DistanceOrSpeed, 2.0f, 5.0f);

    float frameDuration = 0.16f * animationSpeed; // 한 프레임의 지속 시간

    ElapsedTime += deltaTime;

    // 다음 프레임으로 넘어갈지 판단
    if (ElapsedTime >= frameDuration)
    {
        Current_Frame = (Current_Frame + 1) % MaxFrames;
        ElapsedTime -= frameDuration;
    }
}

// 일반 캐릭터용 애니메이션 초기화
void Animation::InitAnimations_Move(Vec2& Size, const std::string& Path,
    std::map<AnimationType, std::map<int, ID3D11ShaderResourceView*>>& RenderResource,
    vector<int> ImageCount)
{
    const std::map<AnimationType, std::pair<std::string, int>> animationData = {
        {AnimationType::Stand,   {Path + "Stand\\",  ImageCount[0]}},
        {AnimationType::Alert,   {Path + "Alert\\",  ImageCount[1]}},
        {AnimationType::Move,    {Path + "Move\\",   ImageCount[2]}},
        {AnimationType::Jump,    {Path + "Jump\\",   ImageCount[3]}},
        {AnimationType::Attack,  {Path + "Attack\\", ImageCount[4]}},
        {AnimationType::Die,     {Path + "Die\\",    ImageCount[5]}},
        {AnimationType::Hit,     {Path + "Hit\\",    ImageCount[6]}}
    };

    for (const auto& anim : animationData)
    {
        if (anim.second.second == 0) continue;

        LoadAnimationFrames(Size, anim.first, anim.second.first, anim.second.second, RenderResource[anim.first]);
    }
}

// 실제 프레임 이미지 로드
void Animation::LoadAnimationFrames(Vec2& Size, AnimationType type,
    const std::string& basePath, int frameCount,
    std::map<int, ID3D11ShaderResourceView*>& RenderResource)
{
    const std::string pngExt = ".png";

    for (int i = 0; i < frameCount; i++)
    {
        std::string filePath = basePath + std::to_string(i) + pngExt;
        RenderResource[i] = nullptr;

        G_ctx->MakeRender->LoadTextureFromFileDX11(filePath.c_str(), &RenderResource[i], Size);
    }
}

// 스킬용 애니메이션 초기화
void Animation::InitAnimations_Skill(Vec2& Size, const std::string& Path,
    std::map<int, ID3D11ShaderResourceView*>& RenderResource, int FrameCount)
{
    const std::string pngExt = ".png";

    for (int i = 0; i < FrameCount; i++)
    {
        std::string FullPath = Path + std::to_string(i) + pngExt;
        G_ctx->MakeRender->LoadTextureFromFileDX11(FullPath.c_str(), &RenderResource[i], Size);
    }
}

// 스킬용 애니메이션 프레임 업데이트
bool Animation::Update_Animaion_For_Skill(float deltaTime, int MaxFrames)
{
    if (MaxFrames <= 0)
    {
        ElapsedTime = 0.0f;
        return true;
    }

    float baseFrameDuration = 0.16f * 0.6f;
    float frameDuration = (MaxFrames <= 7) ? (baseFrameDuration * MaxFrames * 1.1f) : baseFrameDuration;

    ElapsedTime += deltaTime;

    if (ElapsedTime >= frameDuration)
    {
        ElapsedTime -= frameDuration;
        Current_Frame++;

        // 모든 프레임을 완료했는지 확인
        if (Current_Frame >= MaxFrames)
        {
            return false; // 종료됨
        }
    }

    return true; // 아직 진행 중
}

// 스킬용 애니메이션 프레임 업데이트 및 사운드 재생 포함
ID3D11ShaderResourceView* Animation::Start_Animation_Update_For_Skiil(
    std::map<int, ID3D11ShaderResourceView*>& RenderResource,
    bool* EndFrame,
    bool Temp)
{
    float delta = 0.016f;

    bool endFlag = Update_Animaion_For_Skill(delta, static_cast<int>(RenderResource.size()));

    if (!endFlag)
    {
        // 애니메이션 종료 후 상태 초기화
        ElapsedTime = 0.0;
        Current_Frame = 0;
        Limit_Frame = 0;
        *EndFrame = endFlag;
        TEST = true;
        return nullptr;
    }

    float animationSpeed = 0.1f;  // 고정 속도
    float frameDuration = 0.2f / animationSpeed;
    float blendRatio = ElapsedTime / frameDuration;

    // 특정 프레임에서 효과음 재생
    if (TEST && Temp && Current_Frame == 27)
    {
        std::string Path = SkillMusicPath + "Q_1.mp3";
        G_ctx->G_SoundList.PlayEffectSound(Path, 3.0, 0.7f);
        TEST = false;
    }

    // 현재 프레임 텍스처 반환
    if (RenderResource.count(Current_Frame))
        return RenderResource[Current_Frame];

    return nullptr;
}

// 일반 상태 기반 애니메이션 재생
ID3D11ShaderResourceView* Animation::Start_Animation_Update(
    std::map<AnimationType, std::map<int, ID3D11ShaderResourceView*>>& RenderResource,
    AnimationType State,
    float DistanceOrSpeed)
{
    if (Before_State != State)
    {
        Current_Frame = 0;
        ElapsedTime = 0.0f;  // 상태 변경 시 시간 초기화
    }

    auto it = RenderResource.find(State);
    if (it != RenderResource.end())
    {
        // 점프 상태일 경우 마지막 프레임은 포함하지 않음
        Limit_Frame = (State == AnimationType::Jump)
            ? static_cast<int>(it->second.size()) - 1
            : static_cast<int>(it->second.size());

        float delta = 0.016f; // 60FPS 기준
        Update_Animaion(State, delta, Limit_Frame, DistanceOrSpeed);

        float animationSpeed = 1.0f + DistanceOrSpeed * 0.1f;
        float frameDuration = 0.2f / animationSpeed;
        float blendRatio = ElapsedTime / frameDuration;

        // 현재 프레임에 해당하는 텍스처 반환
        for (const auto& FrameRect : it->second)
        {
            if (FrameRect.first == Current_Frame)
            {
                Before_State = State;
                return FrameRect.second;
            }
        }
    }

    return nullptr;
}
