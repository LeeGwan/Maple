#include "Calculation.h"
#include "../../../../Global.h"
#include "../Input/Input.h"
#define NOMINMAX
#include <iostream>
#include <algorithm>


Calculation::~Calculation() {}



void Calculation::Calculation_Init() 
{
	Ec = 0.0;
	AttackTime = 0.0;
	Velocity_y = 0.0;
}

void Calculation::Player_For_Calc(const string& _Name)
{
	Input KeyBoard;
	const double FixedTime = 0.16 / 10.0; // 프레임
	double Accumulator = 0.0;
	double CheckAccumulator = 0.0;//키보드용
	while (1) 
	{
		if (!G_ctx->CanUseCalc) 
		{
			Sleep(10);
			continue;
		}
		break;
	}
	
	while (true) 
	{
	
		
		ObjectClass* Obj = G_ctx->G_ObjectManager->Get_Find_obj(_Name);
		G_ctx->Timer_Update();
		Accumulator += G_ctx->GetDeltaTime();

		// 타이머를 사용해서 0.1 주기마다 계산
		while (Accumulator >= FixedTime) 
		{
			
				Obj->Start_Skill_Calc(Obj, FixedTime);
			
			G_ctx->DmgObject.Set_NowDmg_Calc(FixedTime);
			G_ctx->DmgObject.Check_Delete_NowDmg(FixedTime);//데미지오브젝트
			Handle_Alert(Obj, FixedTime);
			
			
			
	
			Calc_For_Dmg(Obj, FixedTime);
			CheckAccumulator = KeyBoard.Input_KeyBoard(Obj, FixedTime);


			Calc_For_Vector(Obj, FixedTime, CheckAccumulator);
			Accumulator -= FixedTime;
		}

		// 타임 딜레이
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}
}

void Calculation::Monster_For_Calc(const string& _Name)
{
	size_t sizes=0;
	Timer Monster_Timer;
	const double FixedTime = 0.16 / 10.0; // 프레임
	double Accumulator = 0.0;
	double CheckAccumulator = 0.0;
	while (1) {
		if (!G_ctx->CanUseCalc) {
			Sleep(10);
			continue;
		}
		break;
	}
	vector<ObjectClass*> M_obj;
	while (1)
	{
		
		
		M_obj = G_ctx->G_ObjectManager->Get_Find_Monster();
		Monster_Timer.Update();
		Accumulator += Monster_Timer.GetDeltaTime();
		
		if (M_obj.empty())continue;

	
		while (Accumulator >= FixedTime)
		{
			for (auto& It : M_obj)
			{
		
				if (!It|| !IsValidPtr(It))continue;
				if (It->Get_Animaion_State() == AnimationType::Die)continue;
				
				It->Start_Monster_Action(It, FixedTime);
			
			}
			Accumulator -= FixedTime;
		}
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}
}

void Calculation::Start_Calc(const string& _Name) 
{

	if (_Name == "Player")
	{
		Player_For_Calc(_Name);
	}
	else
	{
		Monster_For_Calc(_Name);
	}


}



void Calculation::Calc_For_Vector(ObjectClass* Obj, double DeltaTime, double Move_Time) {
	ImVec2 Map_Size = G_ctx->MakeWindows->Get_MapSize();
	Vec2 Obj_Pos = Obj->Get_Pos();
	Vec2 Obj_Size = Obj->Get_Size();


	Vec2 Calc_Pos = Calc_Event(Obj, DeltaTime, Move_Time);
	if (Calc_Pos.x == -999.0f) return;

	//clamp 를 사용하여 맵 밖으로 못나가게함
	
	Calc_Pos.x = clamp(Calc_Pos.x, -62.0f, Map_Size.x);
	Obj->Set_Pos(Calc_Pos);
}

Vec2 Calculation::Calc_Event(ObjectClass* Obj, double DeltaTime, double Move_Time) {
	
	float AttackSpeed = Obj->Get_AttackSpeed();
	double D_AttackSpeed = static_cast<double>(AttackSpeed);
	bool IsMoving = Obj->Is_Moving();
	float Speed = Obj->Get_Speed();
	AnimationType Current_State = Obj->Get_Animaion_State();
	Vec2 Calc(Obj->Get_Pos().x, Obj->Get_Pos().y);
	const double Gravi = 1000.0;

	// double 을 사용하는 이유는 조금더 정확하게 수소점을 계산이 가능
	double Velocity_x = 0.0, Velocity_y = 0.0;
	float F_Velocity_x = 0.0f, F_Velocity_y = 0.0f;

	// 각각의 상태를 분리하여 계산
	if (Current_State == AnimationType::Move)
	{
		Handle_Movement(Obj, Current_State, Speed, Move_Time, DeltaTime, Calc, Ec, Gravi);
		AttackTime = 0.0;
	}
	else if (Current_State == AnimationType::Jump)
	{
		Apply_JumpPhysics(Obj, DeltaTime, Calc, Ec, Gravi);
		AttackTime = 0.0;
	}


	else {
		return { -999.0f, -999.0f }; // 예외처리
	}

	return Calc;
}

void Calculation::Calc_For_Dmg(ObjectClass* MyObj, double DeltaTime)
{
	float AttackSpeed = MyObj->Get_AttackSpeed();
	
	
	double D_AttackSpeed = static_cast<double>(AttackSpeed);
	AnimationType Current_State = MyObj->Get_Animaion_State();
	if (Current_State == AnimationType::Attack)
	{
		
		AttackTime += DeltaTime;
		if (AttackTime > 0.41 * D_AttackSpeed )
		{
		
			
		
			Dmg_Result Result = DmgCLass.Damage_Update(MyObj);
			if (!Result.T_obj)
				return;

			
			Animaion_Dmg Set_Dmg_Infor ={ Result.Dmg, DeltaTime,Result.T_obj->Get_Pos()};
			G_ctx->DmgObject.Set_NowDmg(Set_Dmg_Infor);
			std::string AttackSound = AttackMusicPath + std::string("Attack.mp3");
			G_ctx->G_SoundList.PlayEffectSound(AttackSound, 1.0, 2.7f);
			
		
			if (Result.T_obj->Get_Hp() <= 0.0f)
			{
				MyObj->Set_Xp(Result.T_obj->Get_Xp());
				AttackTime = 0.0;
				
			}
			else
			{
				Result.T_obj->Set_State(AnimationType::Alert);
				AttackTime = 0.0;
				MyObj->Set_State(AnimationType::Stand);
			}
		
		}
	}





}

void Calculation::Handle_Movement(ObjectClass* Obj, AnimationType Current_State, float Speed, double Move_Time, double DeltaTime, Vec2& Calc, double& Ec, double Gravi) {
	double Velocity_x = 0.0;
	float F_Velocity_x = 0.0f;

	bool isJumping = Obj->Is_MovingAndJmp();
	double direction = Obj->Get_direction();


	if (isJumping) {
		// 공중일 땐 Move_Time을 고려한 속도 (멀리 점프)
		Velocity_x = direction * Speed * Move_Time;
	}
	else {
		// 지상에선 일정한 속도만
		Velocity_x = direction * Speed;
	}

	F_Velocity_x = static_cast<float>(Velocity_x * DeltaTime);
	Calc.x += F_Velocity_x;

	// 중력 처리
	if (isJumping) {
		Apply_JumpPhysics(Obj, DeltaTime, Calc, Ec, Gravi);
	}
	else if (Calc.y > 565.0f) {
		Calc.y = 0.0f;
	}
}

void Calculation::Apply_JumpPhysics(ObjectClass* Obj, double DeltaTime, Vec2& Calc, double& Ec, double Gravi) 
{
	if (!Obj->Is_Jump()) {
		Obj->Change_Jump(true);
		Velocity_y = static_cast<double>( - Obj->Get_JumpPower());
		std::string JumpMusicPath = BackgroundMusicPath + std::string("Jump.mp3");
		G_ctx->G_SoundList.PlayEffectSound(JumpMusicPath, 1.0,  0.7f);
	}
	Calc.y=565.0f + Velocity_y * Ec + (0.5) * Gravi * Ec * Ec;

	Ec += DeltaTime;

	if (Calc.y>565.0f) {
		Ec = 0.0;
		Obj->Change_MovingAndJmp(false);
		Obj->Change_Jump(false);
		//Obj->Set_State(Obj->Is_Moving() ? AnimationType::Move : AnimationType::Stand);
		Calc.y = 565.0f;
	}
}

void Calculation::Handle_Alert(ObjectClass* Obj, double DeltaTime)
{
	if (!Obj->Get_immortal())return;

	AlertTime += DeltaTime;
	if (AlertTime > 1.5)
	{

		Obj->Set_immortal(false);
		AlertTime = 0.0;
	}
	

}


