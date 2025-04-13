#include "Global.h"

Global* G_ctx = new Global;
Timer Global::RenderTimer;
DamageObject Global::DmgObject;
SoundObject Global::G_SoundList;
HttpSocket Global::LoginCheck;
Global::Global()
{
	CanUseCalc = false;
	G_ObjectManager = nullptr;;
}

void Global::Timer_Update()
{
	RenderTimer.Update();
}

double Global::GetDeltaTime() const
{
	return RenderTimer.GetDeltaTime();
}

void Global::WaitForInterval(double seconds)
{
	RenderTimer.WaitForInterval(seconds);
}


