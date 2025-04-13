#include "Input.h"
#include"../AnimationType.h"
double Input::Input_KeyBoard(ObjectClass* Obj, double DeltaTime)
{
    static double Check_Move_TIME = 0.0;   // 이동 누적 시간
    static int lastPressedKey = 0;         // 마지막으로 눌린 키 저장

    // 키 입력 상태 확인
    bool leftPressed    = GetAsyncKeyState(VK_LEFT);
    bool rightPressed   = GetAsyncKeyState(VK_RIGHT);
    bool jumpPressed    = GetAsyncKeyState(VK_MENU);
    bool AttackPressed  = GetAsyncKeyState(VK_LCONTROL);

    // 양쪽 방향키 동시에 눌렸을 경우 - 제자리 상태로 전환
    if (leftPressed && rightPressed)
    {
        double Direction = (lastPressedKey == VK_LEFT) ? -1.0 : 1.0;
        Obj->Set_direction(Direction);
        Obj->Set_State(AnimationType::Stand);
        Check_Move_TIME = 0.0;

        // 공중 상태라면 초기화
        if (Obj->Is_MovingAndJmp() || Obj->Is_Jump())
        {
            Obj->Change_Jump(false);
            Obj->Change_MovingAndJmp(false);
            Obj->Set_Pos({ Obj->Get_Pos().x, 565.0f });
        }
        return Check_Move_TIME;
    }

    // 방향키 입력 시 키 큐에 저장
    if (leftPressed  && (Key.empty() || Key.back() != VK_LEFT)) {
        Key.push(VK_LEFT);
        lastPressedKey = VK_LEFT;
    }
    if (rightPressed && (Key.empty() || Key.back() != VK_RIGHT)) {
        Key.push(VK_RIGHT);
        lastPressedKey = VK_RIGHT;
    }

    // 키 큐 기반 상태 처리
    if (!Key.empty())
    {
        int lastKey = Key.back();
        double Direction = (lastKey == VK_LEFT) ? -1.0 : 1.0;
        Obj->Set_direction(Direction);

        bool isJumping = Obj->Is_Jump();

        if (lastKey == VK_LEFT || lastKey == VK_RIGHT)
        {
            if (jumpPressed && !isJumping)
            {
                // 이동 중 점프
                Obj->Set_State(AnimationType::Jump);
                Obj->Change_MovingAndJmp(true);
            }
            else
            {
                // 기본 이동
                Obj->Set_State(AnimationType::Move);

                // 점프 중이 아닐 때만 이동 시간 누적
                if (!isJumping)
                {
                    Check_Move_TIME = std::min(1.0, Check_Move_TIME + DeltaTime);
                }
            }
        }
    }
    else
    {
        // 방향키 없음: 스탠딩 처리
        double Direction = (lastPressedKey == VK_LEFT) ? -1.0 : 1.0;
        Obj->Set_direction(Direction);

        if (!Obj->Is_Jump() && Obj->Get_Animaion_State() != AnimationType::Attack)
        {
            Obj->Set_State(AnimationType::Stand);
            Check_Move_TIME = 0.0;
        }
    }

    // 방향키가 떼졌을 경우 큐에서 제거
    if (!leftPressed || !rightPressed)
    {
        std::queue<int> tempQueue;
        while (!Key.empty())
        {
            int key = Key.front(); Key.pop();
            if ((key != VK_LEFT || leftPressed) && (key != VK_RIGHT || rightPressed))
            {
                tempQueue.push(key);
            }
        }
        Key = tempQueue;
    }

    // 정지 상태에서 점프만 눌렀을 경우 (순수 점프)
    if (jumpPressed && !Obj->Is_Jump())
    {
        if (Obj->Get_Animaion_State() == AnimationType::Stand)
        {
            Obj->Set_State(AnimationType::Jump);
            Check_Move_TIME = 0.0;
        }
    }

    // 공격 입력 처리
    if (AttackPressed || Obj->Get_Animaion_State() == AnimationType::Attack)
    {
        Obj->Set_State(AnimationType::Attack);  // 일반 공격 상태 전환
        Check_Move_TIME = 0.0;
        return Check_Move_TIME;
    }

    return Check_Move_TIME;
}
