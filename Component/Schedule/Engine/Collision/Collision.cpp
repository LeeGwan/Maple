#include "Collision.h"
#include "../../../../Global.h"
#include <algorithm>

// ------------------------------
// 범위 내 충돌 체크 (단일 대상)
// ------------------------------
ObjectClass* Collision::Check_Collision(ObjectClass* MyObj, float Radius)
{
    if (MyObj->Get_Name() == "Player")
    {
        // 플레이어인 경우 몬스터 중 가까운 대상 탐색
        ObjectClass* Find_Enemy = Find_Near_Obj_For_Player(MyObj->Get_Pos(), Radius);
        if (!Find_Enemy) return nullptr;

        return Find_Enemy;
    }
    else
    {
        // 몬스터인 경우 플레이어와의 충돌 확인
        ObjectClass* Find_Enemy = Find_Near_Obj_For_Monster(MyObj->Get_Pos(), Radius);
        if (!Find_Enemy) return nullptr;

        return Find_Enemy;
    }
}

// ------------------------------
// 범위 내 충돌 체크 (다중 대상)
// ------------------------------
std::vector<ObjectClass*> Collision::Check_Collision(Vec2 Pos, float Radius)
{
    vector<ObjectClass*> Target_obj = G_ctx->G_ObjectManager->Get_Find_Monster();
    vector<ObjectClass*> Return_Obj;

    // 체력이 0 이하인 몬스터 제거
    Target_obj.erase(remove_if(Target_obj.begin(), Target_obj.end(),
        [](ObjectClass* Target_obj)
        {
            return Target_obj->Get_Hp() <= 0.0f;
        }), Target_obj.end());

    // 거리 기준으로 충돌 감지된 객체만 저장
    for (auto& Target : Target_obj)
    {
        if (Pos.distance(Target->Get_Pos()) <= Radius)
        {
            Return_Obj.push_back(Target);
        }
    }

    return Return_Obj;
}

// ------------------------------
// 몬스터 기준: 플레이어와의 충돌 감지
// ------------------------------
ObjectClass* Collision::Find_Near_Obj_For_Monster(Vec2 Pos, float Radius)
{
    ObjectClass* Target_obj = G_ctx->G_ObjectManager->Get_Find_obj("Player");

    if (Target_obj && Pos.distance(Target_obj->Get_Pos()) <= Radius)
    {
        return Target_obj;
    }

    return nullptr;
}

// ------------------------------
// 플레이어 기준: 몬스터 중 가까운 대상 탐색
// ------------------------------
ObjectClass* Collision::Find_Near_Obj_For_Player(Vec2 Pos, float Radius)
{
    vector<ObjectClass*> Target_obj = G_ctx->G_ObjectManager->Get_Find_Monster();

    // 체력이 0 이하인 대상은 제거
    Target_obj.erase(remove_if(Target_obj.begin(), Target_obj.end(),
        [](ObjectClass* Target_obj)
        {
            return Target_obj->Get_Hp() <= 0.0f;
        }), Target_obj.end());

    // 범위 내 가장 가까운 몬스터 반환 (가장 먼저 탐색된 하나만)
    for (auto& Target : Target_obj)
    {
        if (Pos.distance(Target->Get_Pos()) <= Radius)
        {
            return Target;
        }
    }

    return nullptr;
}
