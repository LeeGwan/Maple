#include "Schedule.h"
#include "../Window/window.h"
#include"../../Global.h"
#include "Engine/Calculation/Calculation.h"
ScheduleManager::~ScheduleManager()
{
	delete G_ctx;
}

ScheduleManager::ScheduleManager(HINSTANCE In_hInstance)
{
	_hInstance = In_hInstance;
	G_ctx->MakeWindows = make_shared<window>();

	
}

void ScheduleManager::Update(HINSTANCE In_hInstance)
{

	//렌더링 기반 쓰레드 생성
	ManagerSchedule Thread_Manager_Schedule;

	push(&Thread_Manager_Schedule);
	EngineSchedule Thread_Engine_Schedule;

	push(&Thread_Engine_Schedule);
	RenderSchedule Thread_Render_Schedule(In_hInstance);
	
	push(&Thread_Render_Schedule);

	MakeALL();

}
void ScheduleManager::push(Schedule* InPut_Schedule)
{
	ScheduleList.push_back(InPut_Schedule);
}

void ScheduleManager::MakeALL()
{
	for (auto It_Schedule : ScheduleList)
	{
		It_Schedule->Make();
	}
}



RenderSchedule::RenderSchedule(HINSTANCE In_hInstance)
{
	_hInstance = In_hInstance;
}

void RenderSchedule::Make()
{
	thread RenderThread(bind(&RenderSchedule::Run,this));
	/*
	* std::bind의 사용 목적
	함수의 일부 인자 고정: 함수에 전달할 일부 인자를 미리 고정시키고, 나중에 나머지 인자를 넘겨서 호출할 수 있습니다.

	멤버 함수 호출을 간단하게 처리: 멤버 함수를 사용할 때 객체와 멤버 함수를 함께 바인딩하여 함수 객체로 만들 수 있습니다.

	콜백 함수 처리: 함수의 일부 인자를 나중에 전달할 수 있게 해줘서, 주로 콜백 함수에서 유용하게 사용됩니다.
	*/

	RenderThread.join();

	return ;
}

void RenderSchedule::Run()
{
	

	G_ctx->MakeWindows->Update(_hInstance);
	return;
}

EngineSchedule::EngineSchedule()
{

}

void EngineSchedule::Make()
{
	thread Engine_Thread(bind(&EngineSchedule::Run, this));

	Engine_Thread.detach();

}

void EngineSchedule::Run_For_Player()
{
	//예외처리 넣어주자
	Calculation Player_Calc;
	Player_Calc.Calculation_Init();
	Player_Calc.Start_Calc("Player");
	
}

void EngineSchedule::Run_For_Monster()
{
	Calculation Monster_Calc;
	Monster_Calc.Calculation_Init();
	Monster_Calc.Start_Calc("Monster");
}

void EngineSchedule::Run_For_Sound()
{
	while (1)
	{
		double DeltaTime = G_ctx->GetDeltaTime();
		G_ctx->G_SoundList.Sound_Update(DeltaTime);
		G_ctx->G_SoundList.DeleteSound(DeltaTime);
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}

}

void EngineSchedule::Run()
{

	thread Engine_For_Player_Thread(bind(&EngineSchedule::Run_For_Player, this));

	thread Engine_For_Monster_Thread(bind(&EngineSchedule::Run_For_Monster, this));

	thread Engine_For_Sound_Thread(bind(&EngineSchedule::Run_For_Sound, this));
	Engine_For_Player_Thread.detach();
	Engine_For_Monster_Thread.detach();
	Engine_For_Sound_Thread.detach();
//엔진동작
}

ManagerSchedule::ManagerSchedule()
{
}

void ManagerSchedule::Make()
{
	thread Manager_Thread(bind(&ManagerSchedule::Run, this));

	Manager_Thread.detach();
}

void ManagerSchedule::Check_Manager_Obj(double DeltaTime)
{
	
	G_ctx->G_ObjectManager->Create_Object();
	G_ctx->G_ObjectManager->Check_Manager(DeltaTime);
}

void ManagerSchedule::Run()
{
	Timer Manager_Timer;
	while (1)
	{
		if (!G_ctx->CanUseCalc)
		{
			Sleep(10);
			continue;
		}
		break;
	}
	while (1)
	{
		Manager_Timer.Update();
		Accumulator += Manager_Timer.GetDeltaTime();
		while (Accumulator >= FixedTime)
		{
			Check_Manager_Obj(FixedTime);
			Accumulator -= FixedTime;
		}
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}
}
