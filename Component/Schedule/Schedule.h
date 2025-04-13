#pragma once
#include"../../pch.h"
#include "Rendering/RenderingObject.h"

class Schedule//커맨드 같은거
{
public:
	virtual void Make() = 0;
	virtual void Run() = 0;

};
class RenderSchedule:public Schedule
{
	HINSTANCE _hInstance;
public:
	RenderSchedule(HINSTANCE In_hInstance);
	void Make()override;
	virtual void Run()override;
	
};
class EngineSchedule :public Schedule
{

public:
	EngineSchedule();
	void Make()override;
	void Run_For_Player();
	void Run_For_Monster();
	void Run_For_Sound();
	virtual void Run()override;

};
class ManagerSchedule :public Schedule
{
private:

	double Accumulator = 0.0;
	const double FixedTime = 0.01; // 프레임
	double CheckAccumulator = 0.0;

public:
	ManagerSchedule();
	void Make()override;
	void Check_Manager_Obj(double DeltaTime);
	
	virtual void Run()override;

};

class ScheduleManager// 리무트
{
private:
	vector<Schedule*>ScheduleList;
	HINSTANCE _hInstance;


public:
	~ScheduleManager();
	ScheduleManager(HINSTANCE In_hInstance);
	void Update(HINSTANCE In_hInstance);
	void push(Schedule* InPut_Schedule);
	void MakeALL();

	
};
