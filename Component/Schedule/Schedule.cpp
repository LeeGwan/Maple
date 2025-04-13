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

	//������ ��� ������ ����
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
	* std::bind�� ��� ����
	�Լ��� �Ϻ� ���� ����: �Լ��� ������ �Ϻ� ���ڸ� �̸� ������Ű��, ���߿� ������ ���ڸ� �Ѱܼ� ȣ���� �� �ֽ��ϴ�.

	��� �Լ� ȣ���� �����ϰ� ó��: ��� �Լ��� ����� �� ��ü�� ��� �Լ��� �Բ� ���ε��Ͽ� �Լ� ��ü�� ���� �� �ֽ��ϴ�.

	�ݹ� �Լ� ó��: �Լ��� �Ϻ� ���ڸ� ���߿� ������ �� �ְ� ���༭, �ַ� �ݹ� �Լ����� �����ϰ� ���˴ϴ�.
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
	//����ó�� �־�����
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
//��������
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
