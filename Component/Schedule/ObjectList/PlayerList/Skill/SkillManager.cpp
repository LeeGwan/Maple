#include "SkillManager.h"


SkillManager::SkillManager()
{
   InitializeCriticalSection(&cs);
   Skill_Q.Set_Key(0x51);
 
   Skill_W.Set_Key(0x57);
   Skill_E.Set_Key(0x45);
   Skill_R.Set_Key(0x52);
   Skill_T.Set_Key(0x54);
   SkillManagers.push_back(&Skill_Q);
   SkillManagers.push_back(&Skill_W);
   SkillManagers.push_back(&Skill_E);
   SkillManagers.push_back(&Skill_R);
   SkillManagers.push_back(&Skill_T);
 
}

SkillManager::~SkillManager()
{
    DeleteCriticalSection(&cs);
}



void SkillManager::Update_For_Renderring(Vec2 Pos)
{
    if (SkillManagers.empty())
        return;
    __try
    {
        for (SkillObject* objList : SkillManagers)
        {

            if (!objList)continue;

            objList->Update_For_Renderring(Pos);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return;
    }
}

void SkillManager::Update_For_Calc(ObjectClass* My, double DeltaTime)
{
    if (SkillManagers.empty())
        return;
    __try
    {
        for (SkillObject* objList : SkillManagers)
        {

            if (!objList)continue;

            objList->Update_For_Calc(My, DeltaTime);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return;
    }
}

