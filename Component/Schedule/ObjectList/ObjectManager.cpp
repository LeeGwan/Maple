#include "ObjectManager.h"
#include <algorithm>
#include "../../../Global.h"

// ------------------------------------------------------------
// 생성자/소멸자: 멀티스레드 보호를 위한 임계영역 초기화
// ------------------------------------------------------------
ObjectManager::ObjectManager()
{
    InitializeCriticalSection(&cs);
}

ObjectManager::~ObjectManager()
{
    DeleteCriticalSection(&cs);
}

// ------------------------------------------------------------
// 오브젝트 추가 (RoutineObject로 감싸서 내부 저장)
// ------------------------------------------------------------
void ObjectManager::AddObjectManager(const std::string& _Name)
{
    EnterCriticalSection(&cs);
    ObjectManagers.push_back(std::make_unique<RoutineObject>(_Name));
    LeaveCriticalSection(&cs);
}

// ------------------------------------------------------------
// 특정 이름을 포함한 객체 검색
// ------------------------------------------------------------
ObjectClass* ObjectManager::Get_Find_obj(const std::string& Find_Name)
{
    if (ObjectManagers.empty()) return nullptr;

    EnterCriticalSection(&cs);
    for (auto& objList : ObjectManagers)
    {
        if (!objList) continue;

        if (objList->GetObj()->Get_Name().find(Find_Name) != std::string::npos)
        {
            LeaveCriticalSection(&cs);
            return objList->GetObj();
        }
    }
    LeaveCriticalSection(&cs);
    return nullptr;
}

// ------------------------------------------------------------
// 몬스터 개체 자동 생성 로직 (최대 수 제한)
// ------------------------------------------------------------
void ObjectManager::Create_Object()
{
    ObjCount = std::count_if(ObjectManagers.begin(), ObjectManagers.end(),
        [](const std::unique_ptr<RoutineObject>& Obj) { return Obj != nullptr; });

    if (ObjCount > MaxCount)
        return;

    for (int i = ObjCount; i < 6; i++)
    {
        int RandMonster = rand() % 2;
        if (RandMonster == 0)
            G_ctx->G_ObjectManager->AddObjectManager("GreenMushroom");
        else
            G_ctx->G_ObjectManager->AddObjectManager("OrangeMushroom");
    }
}

// ------------------------------------------------------------
// "Monster" 이름을 포함하는 객체만 필터링
// ------------------------------------------------------------
std::vector<ObjectClass*> ObjectManager::Get_Find_Monster()
{
    EnterCriticalSection(&cs);
    std::vector<ObjectClass*> Get_Monster;

    for (const auto& objList : ObjectManagers)
    {
        if (!objList) continue;

        if (objList->GetObj()->Get_Name().find("Monster") != std::string::npos)
        {
            Get_Monster.push_back(objList->GetObj());
        }
    }

    LeaveCriticalSection(&cs);
    return Get_Monster;
}

// ------------------------------------------------------------
// Player + Monster 객체 필터링
// ------------------------------------------------------------
std::vector<ObjectClass*> ObjectManager::Get_Find_Object()
{
    EnterCriticalSection(&cs);
    std::vector<ObjectClass*> Get_Obj_List;

    for (const auto& objList : ObjectManagers)
    {
        if (!objList) continue;

        const std::string& name = objList->GetObj()->Get_Name();

        if (name.find("Player") != std::string::npos ||
            name.find("Monster") != std::string::npos)
        {
            Get_Obj_List.push_back(objList->GetObj());
        }
    }

    LeaveCriticalSection(&cs);
    return Get_Obj_List;
}

// ------------------------------------------------------------
// 오브젝트 상태 확인 및 사망 처리, 삭제 시간 갱신
// ------------------------------------------------------------
void ObjectManager::Check_Manager(double DeltaTime)
{
    if (ObjectManagers.empty()) return;

    EnterCriticalSection(&cs);
    DeleteTime += DeltaTime;

    if (DeleteTime > 10.0f)
    {
        Remove_NullObj();  // null 제거
        DeleteTime = 0.0;
    }

    for (auto& objList : ObjectManagers)
    {
        if (!objList) continue;

        ObjectClass* Obj = objList->GetObj();
        const std::string& name = Obj->Get_Name();

        if (name.find("Maps") != std::string::npos || name.find("Player") != std::string::npos)
            continue;

        if (Obj->Get_Hp() == 0.0f && Obj->Get_Animaion_State() != AnimationType::Die)
        {
            Obj->Set_State(AnimationType::Die);
        }
        else if (Obj->Get_Hp() == 0.0f && Obj->Get_Animaion_State() == AnimationType::Die)
        {
            Obj->Set_Can_Delete_Time(DeltaTime);
            if (Obj->Get_Can_Delete_Time() > 1.2)
            {
                objList.reset();  // 삭제 조건 충족 시 제거 예약
            }
        }
    }

    LeaveCriticalSection(&cs);
}

// ------------------------------------------------------------
// null 객체 제거 (삭제 예약된 오브젝트 정리)
// ------------------------------------------------------------
void ObjectManager::Remove_NullObj()
{
    ObjCount = std::count_if(ObjectManagers.begin(), ObjectManagers.end(),
        [](const std::unique_ptr<RoutineObject>& Obj) { return Obj != nullptr; });

    if (ObjCount != MaxCount)
    {
        ObjectManagers.erase(
            std::remove_if(ObjectManagers.begin(), ObjectManagers.end(),
                [](const std::unique_ptr<RoutineObject>& ptr) { return ptr == nullptr; }),
            ObjectManagers.end()
        );
    }
}

// ------------------------------------------------------------
// 전체 오브젝트에 대해 Update 호출
// ------------------------------------------------------------
void ObjectManager::Update()
{
    if (ObjectManagers.empty()) return;

    __try
    {
        for (auto& objList : ObjectManagers)
        {
            if (!objList || !IsValidPtr(*(uintptr_t*)objList.get())) continue;

            objList->Update();
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return;  // 예외 발생 시 무시
    }
}
