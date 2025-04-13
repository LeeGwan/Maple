#include "SoundObject.h"
#include "../../Engine/AnimationType.h"

// ------------------------------------------------------------
// 배경음악 재생 (지속 사운드)
// ------------------------------------------------------------
void SoundObject::PlayBackgroundMusic(const std::string& Path)
{
    // 기존 INFINITESOUND 배경음 정리
    if (!SoundList.empty())
    {
        for (auto it = SoundList.begin(); it != SoundList.end(); )
        {
            if (it->Deltatime == INFINITESOUND)
            {
                if (it->sound)
                {
                    it->sound->release();
                    it->sound = nullptr;
                }
                if (it->system)
                {
                    it->system->close();
                    it->system->release();
                    it->system = nullptr;
                }
                it = SoundList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // FMOD 초기화 및 사운드 재생
    FMOD::System* system = nullptr;
    FMOD::Sound* sound = nullptr;
    FMOD::Channel* channel = nullptr;

    FMOD::System_Create(&system);
    system->init(512, FMOD_INIT_NORMAL, nullptr);

    std::string BackGroundPath = BackgroundMusicPath + Path;
    FMOD_RESULT result = system->createSound(BackGroundPath.c_str(), FMOD_2D | FMOD_DEFAULT, nullptr, &sound);
    if (result != FMOD_OK) return;

    result = system->playSound(sound, nullptr, false, &channel);
    if (result != FMOD_OK) return;

    channel->setVolume(0.5f); // 볼륨 설정

    SoundStruct Sound = { system, sound, channel, INFINITESOUND };
    Push_SoundObject(Sound);
}

// ------------------------------------------------------------
// 효과음 재생 (일정 시간 후 제거)
// ------------------------------------------------------------
void SoundObject::PlayEffectSound(const std::string& soundFilePath, double Time, float SoundVolume)
{
    FMOD::System* system = nullptr;
    FMOD::Sound* sound = nullptr;
    FMOD::Channel* channel = nullptr;

    FMOD::System_Create(&system);
    system->init(512, FMOD_INIT_NORMAL, nullptr);

    FMOD_RESULT result = system->createSound(soundFilePath.c_str(), FMOD_2D, nullptr, &sound);
    if (result != FMOD_OK) return;

    result = system->playSound(sound, nullptr, false, &channel);
    channel->setVolume(SoundVolume); // 볼륨 조정

    SoundStruct Sound = { system, sound, channel, Time }; // 삭제 타이머 포함
    Push_SoundObject(Sound);
}

// ------------------------------------------------------------
// 사운드 리스트에 사운드 등록
// ------------------------------------------------------------
void SoundObject::Push_SoundObject(SoundStruct& Sound)
{
    SoundList.push_back(Sound);
}

// ------------------------------------------------------------
// 매 프레임마다 FMOD 시스템 업데이트
// ------------------------------------------------------------
void SoundObject::Sound_Update(double Deltatime)
{
    if (SoundList.empty()) return;

    for (auto& it : SoundList)
    {
        if (!it.channel || !it.sound || !it.system) continue;
        it.system->update();
    }
}

// ------------------------------------------------------------
// 일정 시간이 지난 효과음 제거 (Deltatime 기반)
// ------------------------------------------------------------
void SoundObject::DeleteSound(double Deltatime)
{
    if (SoundList.empty()) return;

    for (auto it = SoundList.begin(); it != SoundList.end(); )
    {
        if (it->Deltatime == INFINITESOUND)
        {
            ++it;
            continue;
        }

        it->Deltatime -= Deltatime;
        if (it->Deltatime < 0)
        {
            // 사운드 해제
            if (it->sound)
            {
                it->sound->release();
                it->sound = nullptr;
            }
            if (it->system)
            {
                it->system->close();
                it->system->release();
                it->system = nullptr;
            }

            it = SoundList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
