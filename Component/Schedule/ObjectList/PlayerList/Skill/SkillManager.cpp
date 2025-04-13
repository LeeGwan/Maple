#include "SkillManager.h"

// ---------------------------------------------
// 생성자: 스킬 초기화 및 키 바인딩
// ---------------------------------------------
SkillManager::SkillManager()
{
    InitializeCriticalSection(&cs);  // 멀티스레드 보호용

    // 키 설정 (Q, W, E, R, T)
    Skill_Q.Set_Key(0x51);  // Q
    Skill_W.Set_Key(0x57);  // W
    Skill_E.Set_Key(0x45);  // E
    Skill_R.Set_Key(0x52);  // R
    Skill_T.Set_Key(0x54);  // T

    // 스킬 리스트에 추가
    SkillManagers.push_back(&Skill_Q);
    SkillManagers.push_back(&Skill_W);
    SkillManagers.push_back(&Skill_E);
    SkillManagers.push_back(&Skill_R);
    SkillManagers.push_back(&Skill_T);
}

// ---------------------------------------------
// 소멸자: 동기화 객체 해제
// ---------------------------------------------
SkillManager::~SkillManager()
{
    DeleteCriticalSection(&cs);
}

// ---------------------------------------------
// 렌더링용 스킬 업데이트 호출
// (스킬 애니메이션 출력)
// ---------------------------------------------
void SkillManager::Update_For_Renderring(Vec2 Pos)
{
    if (SkillManagers.empty()) return;

    __try
    {
        for (SkillObject* objList : SkillManagers)
        {
            if (!objList) continue;

            objList->Update_For_Renderring(Pos);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return;  // 예외 발생 시 무시하고 안전 탈출
    }
}

// ---------------------------------------------
// 연산용 스킬 업데이트 호출
// (입력 감지 + 쿨타임 체크 + 데미지 처리 등)
// ---------------------------------------------
void SkillManager::Update_For_Calc(ObjectClass* My, double DeltaTime)
{
    if (SkillManagers.empty()) return;

    __try
    {
        for (SkillObject* objList : SkillManagers)
        {
            if (!objList) continue;

            objList->Update_For_Calc(My, DeltaTime);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return;  // 예외 발생 시 루프 무시
    }
}
