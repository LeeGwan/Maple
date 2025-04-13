#include "SoundObject.h"
#include "../../Engine/AnimationType.h"
void SoundObject::PlayBackgroundMusic(const std::string& Path)
{
	if (!SoundList.empty())
	{
		for (auto it = SoundList.begin();it != SoundList.end();)
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
			
		}
	}
	FMOD::System* system = nullptr;
	FMOD::Sound* sound = nullptr;
	FMOD::Channel* channel = nullptr;
	FMOD::System_Create(&system);
	system->init(512, FMOD_INIT_NORMAL, nullptr);
	std::string BackGroundPath = BackgroundMusicPath + Path;
	FMOD_RESULT result = system->createSound(BackGroundPath.c_str(), FMOD_2D | FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK) {

		return;
	}
	result = system->playSound(sound, nullptr, false, &channel);
	if (result != FMOD_OK)
	{

		return;
	}
	channel->setVolume(0.5f);
	SoundStruct Sound = { system ,sound ,channel ,INFINITESOUND };
	Push_SoundObject(Sound);
}

void SoundObject::PlayEffectSound(const std::string& soundFilePath, double Time,float SoundVolume)
{
	FMOD::System* system = nullptr;
	FMOD::Sound* sound = nullptr;
	FMOD::Channel* channel = nullptr;
	FMOD::System_Create(&system);
	unsigned long long dspClock;
	system->init(512, FMOD_INIT_NORMAL, nullptr);

	FMOD_RESULT result = system->createSound(soundFilePath.c_str(), FMOD_2D, nullptr, &sound);  // 2D 위치 음향
	if (result != FMOD_OK) return;


		result = system->playSound(sound, nullptr, false, &channel);
	

		channel->setVolume(SoundVolume);  // 효과음 볼륨 조정

	SoundStruct Sound = { system, sound, channel, Time };  // time 뒤 삭제되는 효과음
	Push_SoundObject(Sound);  // 리스트에 추가
}

void SoundObject::Push_SoundObject(SoundStruct& Sound)
{
	SoundList.push_back(Sound);
}

void SoundObject::Sound_Update(double Deltatime)
{
	if (SoundList.empty())return;

	for (auto& it : SoundList)
	{
		if (!it.channel || !it.sound || !it.system)continue;
		it.system->update();
		
	}
}

void SoundObject::DeleteSound(double Deltatime)
{
	if (SoundList.empty())return;
	for (auto it = SoundList.begin();it != SoundList.end();)
	{
		if (it->Deltatime == INFINITESOUND)
		{
			++it;
			continue;
		}

		it->Deltatime -= Deltatime;
		if (it->Deltatime < 0)
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
