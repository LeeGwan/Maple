#pragma once
#include "pch.h"
#include "Component/Window/window.h"
#include "Component/Schedule/Rendering/RenderingObject.h"
#include "Component/Schedule/ObjectList/ObjectManager.h"
#include"Component/Schedule/ObjectList/DamageObject/DamageObject.h"
#include"Component/Schedule/ObjectList/SoundList/SoundObject.h"
#include "Component/Schedule/ObjectList/LoginMap/Login/HttpSocket.h"
#define IsValidAddr(addr) (static_cast<uintptr_t>(addr) > 0x100 && static_cast<uintptr_t>(addr) < 0x00007fffffffffff  && !IsBadReadPtr((void*)addr, sizeof(uintptr_t)))
#define IsValidPtr(ptr) ([&]() -> bool { uintptr_t _val = *(uintptr_t*)(ptr); return IsValidAddr(_val); }())
class Global
{
public:
	shared_ptr<window>MakeWindows;
	shared_ptr<RenderObject>MakeRender;
	shared_ptr<ObjectManager>G_ObjectManager;
	bool CanUseCalc;
	Global();
	static DamageObject DmgObject;
	static SoundObject G_SoundList;
	static Timer RenderTimer;
	static HttpSocket LoginCheck;;
	void Timer_Update();
	double GetDeltaTime() const;
	void WaitForInterval(double seconds);

};
extern Global* G_ctx;
/*
extern window* MakeWindows;
extern RenderObject* MakeRender;
extern ObjectManager* G_ObjectManager;*/
