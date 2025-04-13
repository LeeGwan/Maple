#pragma once
#include "../../../../pch.h"

#define INFINITESOUND 9999.0
struct SoundStruct
{
	FMOD::System* system = nullptr;
	FMOD::Sound* sound = nullptr;
	FMOD::Channel* channel = nullptr;
	double Deltatime;
	
	SoundStruct operator=(const SoundStruct& other) {
		return { this->system = other.system, this->sound = other.sound ,this->channel = other.channel ,this->Deltatime =other.Deltatime };
	}
};



class SoundObject
{
private:
	std::list<SoundStruct> SoundList;
public:
	void PlayBackgroundMusic(const std::string& Path);
	void PlayEffectSound(const std::string& soundFilePath, double Time, float SoundVolume);

	void Push_SoundObject(SoundStruct& Sound);
	void Sound_Update(double Deltatime);
	void DeleteSound(double Deltatime);
};

