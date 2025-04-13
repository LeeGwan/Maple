#pragma once
#include "../../../pch.h"
#include "../Engine/AnimationType.h"

// ------------------------------------------------------------
// Abstract Base Class: 모든 게임 오브젝트가 상속받는 클래스
// ------------------------------------------------------------
class ObjectClass
{
public:
    virtual ~ObjectClass() = default;

    // 필수 구현 메서드
    virtual void Update() = 0;

    // 기본 정보 getter
    virtual ObjectClass* GetObj();
    virtual int Get_Lv();
    virtual bool Get_Is_Lv_Up();
    virtual std::string Get_Name();
    virtual float Get_Hp();
    virtual float Get_Mp();
    virtual float Get_Xp();
    virtual Vec2 Get_Pos();
    virtual Vec2 Get_Size();
    virtual AnimationType Get_Animaion_State();
    virtual bool Get_immortal();
    virtual double Get_direction();
    virtual double Get_Can_Delete_Time();

    // 상태 정보
    virtual bool Is_Moving();
    virtual bool Is_Jump();
    virtual bool Is_MovingAndJmp();
    virtual float Get_Speed();
    virtual float Get_JumpPower();
    virtual float Get_AttackSpeed();
    virtual float Get_BasicDmg();
    virtual float Get_Radius();
    virtual float Get_See_Radius();
    virtual int Get_Str();
    virtual int Get_Dex();

    // Setter
    virtual void Set_Hp(float _Hp);
    virtual void Set_Mp(float _Mp);
    virtual void Set_Xp(float _Xp);
    virtual void Set_Pos(Vec2& Vec);
    virtual void Set_State(AnimationType state);
    virtual void Set_Speed(float _Speed);
    virtual void Set_JumpPower(float _JumpPower);
    virtual void Set_direction(double _direction);
    virtual void Set_AttackSpeed(float _AttackSpeed);
    virtual void Set_Str(int _Str);
    virtual void Set_Dex(int _Dex);
    virtual void Set_Lv(int _Lv);
    virtual void Set_Is_Lv_Up(bool _Lv);
    virtual void Set_Can_Delete_Time(double DeltaTime);
    virtual bool Set_immortal(bool State);
    virtual void Change_Jump(bool st);
    virtual void Change_MovingAndJmp(bool st);

    // 동작 관련
    virtual void Start_Monster_Action(ObjectClass* My, double DeltaTime);
    virtual void Start_Skill_Calc(ObjectClass* My, double DeltaTime);
};

// ------------------------------------------------------------
// RoutineObject: 다형성으로 실제 오브젝트를 보관하는 컨테이너 클래스
// ex) PlayerObject, GreenMushroom 등
// ------------------------------------------------------------
class RoutineObject : public ObjectClass
{
private:
    std::unique_ptr<ObjectClass> ObjectClasses;

public:
    RoutineObject(const std::string& _Name);
    virtual ~RoutineObject() override = default;

    // Override
    ObjectClass* GetObj() override;
    void Update() override;
};
