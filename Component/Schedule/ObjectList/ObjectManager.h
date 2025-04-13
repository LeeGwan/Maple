#pragma once
#include "../../../pch.h"
#include "ObjectList.h"

// ------------------------------------------------------------
// ObjectManager: 게임 오브젝트(플레이어, 몬스터 등) 전체를 관리하는 클래스
// ------------------------------------------------------------
class ObjectManager
{
private:
    // 객체 관련 상태
    int ObjCount = 0;               // 현재 존재하는 유효 오브젝트 수
    int MaxCount = 10;              // 생성 가능한 최대 오브젝트 수
    double DeleteTime = 0.0;        // 삭제 주기 체크용 타이머

    // 멀티스레드 보호용 임계영역
    CRITICAL_SECTION cs;

    // 모든 게임 오브젝트를 보관하는 컨테이너
    std::vector<std::unique_ptr<RoutineObject>> ObjectManagers;

public:
    ObjectManager();
    ~ObjectManager();

    // 오브젝트 추가 (이름 기준으로 RoutineObject 생성)
    void AddObjectManager(const std::string& Find_Name);

    // 특정 이름이 포함된 객체 반환 (예: "Player")
    ObjectClass* Get_Find_obj(const std::string& Find_Name);

    // 자동 오브젝트 생성 (몬스터 등)
    void Create_Object();

    // 몬스터 객체 목록 반환
    std::vector<ObjectClass*> Get_Find_Monster();

    // 플레이어 + 몬스터 객체 목록 반환
    std::vector<ObjectClass*> Get_Find_Object();

    // 죽은 몬스터 제거 및 상태 확인
    void Check_Manager(double DeltaTime);

    // nullptr 오브젝트 제거
    void Remove_NullObj();

    // 전체 오브젝트에 대해 Update 호출
    void Update();
};
