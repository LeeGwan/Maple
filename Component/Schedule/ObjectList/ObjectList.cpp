#include "ObjectList.h"
#include "PlayerList/PlayerObject.h"
#include "MapList/MapObject.h"
#include "MonsterList/GreenMushroom/GreenMushroom.h"
#include "MonsterList/OrangeMushroom/OrangeMushroom.h"
#include <cstdlib>
#include <ctime>

// ------------------------------------------------------------
// RoutineObject: 오브젝트 생성자 (이름 기준으로 타입 분기)
// ------------------------------------------------------------
RoutineObject::RoutineObject(const std::string& _Name)
{
    if (_Name.find("Player") != std::string::npos)
    {
        ObjectClasses = std::make_unique<PlayerObject>(_Name);
    }
    else if (_Name.find("Maps") != std::string::npos)
    {
        ObjectClasses = std::make_unique<MapObject>(_Name);
    }
    else if (_Name.find("Mushroom") != std::string::npos)
    {
        static int count = 0;
        if (count == 0)
        {
            srand(static_cast<unsigned int>(time(nullptr)));
            count++;
        }

        int Random_X = rand() % (1270 - 0 + 1);  // 0~1270
        if (_Name.find("GreenMushroom") != std::string::npos)
        {
            ObjectClasses = std::make_unique<GreenMushroom>(_Name, Random_X);
        }
        else
        {
            ObjectClasses = std::make_unique<OrangeMushroom>(_Name, Random_X);
        }
    }
}

ObjectClass* RoutineObject::GetObj()
{
    return ObjectClasses ? ObjectClasses.get() : nullptr;
}

void RoutineObject::Update()
{
    if (ObjectClasses)
        ObjectClasses->Update();
}

// ------------------------------------------------------------
// ObjectClass: 기본 인터페이스 구현부 (더미값 반환)
// ------------------------------------------------------------

bool ObjectClass::Get_immortal()                             { return false; }
AnimationType ObjectClass::Get_Animaion_State()              { return AnimationType(); }
Vec2 ObjectClass::Get_Size()                                 { return Vec2(); }
double ObjectClass::Get_direction()                          { return 0.0; }

void ObjectClass::Set_Hp(float)                              {}
void ObjectClass::Set_Mp(float)                              {}
void ObjectClass::Set_Xp(float)                              {}
void ObjectClass::Set_Pos(Vec2&)                             {}
void ObjectClass::Set_State(AnimationType)                   {}
void ObjectClass::Set_Speed(float)                           {}
void ObjectClass::Set_JumpPower(float)                       {}
void ObjectClass::Set_direction(double)                      {}
void ObjectClass::Set_AttackSpeed(float)                     {}
void ObjectClass::Change_Jump(bool)                          {}
void ObjectClass::Change_MovingAndJmp(bool)                  {}
void ObjectClass::Set_Str(int)                               {}
void ObjectClass::Set_Dex(int)                               {}
bool ObjectClass::Set_immortal(bool)                         { return false; }
void ObjectClass::Set_Lv(int)                                {}
void ObjectClass::Set_Is_Lv_Up(bool)                         {}
void ObjectClass::Set_Can_Delete_Time(double)                {}
void ObjectClass::Start_Monster_Action(ObjectClass*, double) {}
void ObjectClass::Start_Skill_Calc(ObjectClass*, double)     {}

ObjectClass* ObjectClass::GetObj()                           { return nullptr; }

int ObjectClass::Get_Lv()                                    { return 0; }
bool ObjectClass::Get_Is_Lv_Up()                             { return false; }
std::string ObjectClass::Get_Name()                          { return std::string(); }
float ObjectClass::Get_Hp()                                  { return 0.0f; }
float ObjectClass::Get_Mp()                                  { return 0.0f; }
float ObjectClass::Get_Xp()                                  { return 0.0f; }
Vec2 ObjectClass::Get_Pos()                                  { return Vec2(); }
bool ObjectClass::Is_Moving()                                { return false; }
bool ObjectClass::Is_Jump()                                  { return false; }
bool ObjectClass::Is_MovingAndJmp()                          { return false; }
float ObjectClass::Get_Speed()                               { return 0.0f; }
float ObjectClass::Get_JumpPower()                           { return 0.0f; }
float ObjectClass::Get_AttackSpeed()                         { return 0.0f; }
float ObjectClass::Get_BasicDmg()                            { return 0.0f; }
float ObjectClass::Get_Radius()                              { return 0.0f; }
int ObjectClass::Get_Str()                                   { return 0; }
int ObjectClass::Get_Dex()                                   { return 0; }
double ObjectClass::Get_Can_Delete_Time()                    { return 0.0; }
float ObjectClass::Get_See_Radius()                          { return 0.0f; }
