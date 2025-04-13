#include "Input.h"
#include"../AnimationType.h"

double Input::Input_KeyBoard(ObjectClass* Obj, double DeltaTime)
{
	static double Check_Move_TIME = 0.0;
	

	
	static int lastPressedKey = 0;

	//static bool AttackWasPressed = false;
	bool leftPressed = GetAsyncKeyState(VK_LEFT);
	bool rightPressed = GetAsyncKeyState(VK_RIGHT);
	bool jumpPressed = GetAsyncKeyState(VK_MENU);
	bool AttackPressed = GetAsyncKeyState(VK_LCONTROL);

	// 양쪽 키 동시에 눌렸을 때 멈춤 처리
	if (leftPressed && rightPressed)
	{
		double Direction = lastPressedKey == VK_LEFT ? -1.0 : 1.0;
		Obj->Set_direction(Direction);
		Obj->Set_State(AnimationType::Stand);
		Check_Move_TIME = 0.0;
		if (Obj->Is_MovingAndJmp() || Obj->Is_Jump())
		{
			Vec2 ChangePos = Vec2(Obj->Get_Pos().x, 565.0f);
			Obj->Change_Jump(false);
			Obj->Change_MovingAndJmp(false);
			Obj->Set_Pos(ChangePos);
		}
		return Check_Move_TIME;
	}

	// 키 입력 큐에 추가
	if (leftPressed && (Key.empty() || Key.back() != VK_LEFT)) {
		Key.push(VK_LEFT);
		lastPressedKey = VK_LEFT;
	}
	if (rightPressed && (Key.empty() || Key.back() != VK_RIGHT)) {
		Key.push(VK_RIGHT);
		lastPressedKey = VK_RIGHT;
	}

	// 키에 따라 상태 설정
	if (!Key.empty()) {
		int lastKey = Key.back();
		bool isJumping = Obj->Is_Jump();
		double Direction = lastKey == VK_LEFT ? -1.0 : 1.0;
		Obj->Set_direction(Direction);

		// <- 방향 코드
		if (lastKey == VK_LEFT)
		{

			if (jumpPressed && !isJumping) {
				Obj->Set_State(AnimationType::Jump);
				Obj->Change_MovingAndJmp(true);
			}
			else {
				Obj->Set_State(AnimationType::Move);

				// 점프 중 아닐 때만 이동 시간 증가
				if (!isJumping)
					if (Check_Move_TIME >= 1.0)//점프하면서 이동속도가 증가하는 버그 처리하는 예외
					{
						Check_Move_TIME = 1.0;
					}
					else
					{
						Check_Move_TIME += DeltaTime;
					}
			}
		}

		// -> 방향 코드
		else if (lastKey == VK_RIGHT) {
			if (jumpPressed && !isJumping) {
				Obj->Set_State(AnimationType::Jump);
				Obj->Change_MovingAndJmp(true);
			}
			else {
				Obj->Set_State(AnimationType::Move);

				if (!isJumping)
					if (Check_Move_TIME >= 1.0)//점프하면서 이동속도가 증가하는 버그 처리하는 예외
					{
						Check_Move_TIME = 1.0;
					}
					else
					{
						Check_Move_TIME += DeltaTime;
					}
			}
		}
	}
	else {
		double Direction = lastPressedKey == VK_LEFT ? -1.0 : 1.0;
		Obj->Set_direction(Direction);
		// 아무 키도 안 눌렸을 때
		if (!Obj->Is_Jump()&&Obj->Get_Animaion_State() != AnimationType::Attack)
		{

			Obj->Set_State(AnimationType::Stand);

			Check_Move_TIME = 0.0;
		}
	}

	// 키가 떼어진 경우 큐에서 제거
	if (!leftPressed || !rightPressed) {
		std::queue<int> tempQueue;
		while (!Key.empty()) {
			int key = Key.front(); Key.pop();
			if ((key != VK_LEFT || leftPressed) && (key != VK_RIGHT || rightPressed)) {
				tempQueue.push(key);
			}
		}
		Key = tempQueue;
	}

	// 스탠딩 상태에서 점프만 눌렸을 때 (정지 점프)
	if (jumpPressed && !Obj->Is_Jump())
	{
		// double Direction = lastPressedKey == VK_LEFT ? -1 : 1;
		// Obj->Set_direction(Direction);
		AnimationType state = Obj->Get_Animaion_State();
		if (state == AnimationType::Stand) {
			Obj->Set_State(AnimationType::Jump);
			Check_Move_TIME = 0.0;
		}

	}
	if (AttackPressed || Obj->Get_Animaion_State() == AnimationType::Attack)
	{

		/*
		if (Obj->Is_Jump()) {
			Obj->Set_State(AnimationType::JumpAttack); // 점프 공격 상태
		}
		else {
			Obj->Set_State(AnimationType::Attack); // 일반 공격 상태
		}*/
		
		Obj->Set_State(AnimationType::Attack); // 일반 공격 상태
		
		Check_Move_TIME = 0.0;
		return Check_Move_TIME;
	}
	return Check_Move_TIME;
}

bool Input::Input_SKillKeyBoard(int key)
{
	bool IsPressed = GetAsyncKeyState(key);
	if(IsPressed)
	{
		return true;
	}
	return false;
}


