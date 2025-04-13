#include "ObjectManager.h"
#include <algorithm>
#include "../../../Global.h"
ObjectManager::ObjectManager()
{
    InitializeCriticalSection(&cs);
}
ObjectManager::~ObjectManager()
{
    DeleteCriticalSection(&cs);
}
void ObjectManager::AddObjectManager(const string& _Name)
{
    EnterCriticalSection(&cs);
    ObjectManagers.push_back(make_unique<RoutineObject>(_Name));
    LeaveCriticalSection(&cs);
}

ObjectClass* ObjectManager::Get_Find_obj(const string& Find_Name)
{
    if(ObjectManagers.empty())
        return nullptr;

    EnterCriticalSection(&cs);
    for (auto& objList : ObjectManagers)
    {
        if (!objList)continue;


        if (objList->GetObj()->Get_Name().find(Find_Name) != std::string::npos)
        {
            LeaveCriticalSection(&cs);
            return objList->GetObj();
        }
        //각각 어떤 이미지 냐 if 문
    }
    LeaveCriticalSection(&cs);
    return nullptr;
}
void ObjectManager::Create_Object()
{
    ObjCount = std::count_if(ObjectManagers.begin(), ObjectManagers.end(), [](std::unique_ptr<RoutineObject>& Obj) {return Obj != nullptr;});

    if (ObjCount > MaxCount)
    {

        return;
    }

  
    for (int i = ObjCount;i < 6 ; i++)
    {
      
        int RandMonster = rand() % 2;
        if (RandMonster == 0)
        {
            G_ctx->G_ObjectManager->AddObjectManager("GreenMushroom");
            continue;
        }
        else
        {
            G_ctx->G_ObjectManager->AddObjectManager("OrangeMushroom");
        }
      
    }

}
vector<ObjectClass*>  ObjectManager::Get_Find_Monster()
{
    EnterCriticalSection(&cs);
    vector<ObjectClass*> Get_Monster;

    for (const auto& objList : ObjectManagers)
    {
        if (!objList)continue;

        if (objList->GetObj()->Get_Name().find("Monster") != std::string::npos)
        {
        
            Get_Monster.push_back(objList->GetObj());
        }
        //각각 어떤 이미지 냐 if 문
    }
   
    LeaveCriticalSection(&cs);
    return Get_Monster;
}
vector<ObjectClass*>  ObjectManager::Get_Find_Object()
{
    EnterCriticalSection(&cs);
    vector<ObjectClass*> Get_Obj_List;
    for (const auto& objList : ObjectManagers)
    {
        if (!objList)continue;

        if (objList->GetObj()->Get_Name().find("Player") != std::string::npos)
        {

            Get_Obj_List.push_back(objList->GetObj());
        }
        else if (objList->Get_Name().find("Monster") != std::string::npos)
        {
            Get_Obj_List.push_back(objList->GetObj());
        }
        //각각 어떤 이미지 냐 if 문
    }

    LeaveCriticalSection(&cs);
    return Get_Obj_List;
}
void ObjectManager::Check_Manager(double DeltaTime)
{
    if (ObjectManagers.empty())
        return;

    EnterCriticalSection(&cs);
    DeleteTime += DeltaTime;
    
    if (DeleteTime > 10.0f)
    {

        Remove_NullObj();
        DeleteTime = 0.0;
    }

    for (auto& objList : ObjectManagers)
    {
        if (!objList)continue;

        ObjectClass* Obj = objList->GetObj();
        if (Obj->Get_Name().find("Maps") != std::string::npos|| Obj->Get_Name().find("Player") != std::string::npos)
        {
       
            continue;
        }
        else if (Obj->Get_Hp() == 0.0f && Obj->Get_Animaion_State() != AnimationType::Die)
        {
            Obj->Set_State(AnimationType::Die);
        }
        else if (Obj->Get_Hp() == 0.0f && Obj->Get_Animaion_State() == AnimationType::Die)
        {
            Obj->Set_Can_Delete_Time(DeltaTime);

            if (Obj->Get_Can_Delete_Time() > 1.2)
            {
                
                objList.reset();
            }

        }
    }
    LeaveCriticalSection(&cs);
}

void ObjectManager::Remove_NullObj()
{
    ObjCount = std::count_if(ObjectManagers.begin(), ObjectManagers.end(), [](std::unique_ptr<RoutineObject>& Obj) {return Obj != nullptr;});
    if (ObjCount != MaxCount)
    {
        ObjectManagers.erase(
            std::remove_if(ObjectManagers.begin(), ObjectManagers.end(),
                [](const std::unique_ptr<RoutineObject>& ptr) {
                    return ptr == nullptr;
                }
            ),
            ObjectManagers.end()
        );
    }
}



void ObjectManager::Update()
{
    if (ObjectManagers.empty())
        return ;
    __try
    {
        for (auto& objList : ObjectManagers)
        {

            if (!objList || !IsValidPtr(*(uintptr_t*)objList.get()))continue;

            objList->Update();
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) 
    {
        return;
    }
}
