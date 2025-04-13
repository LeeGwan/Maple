#pragma once

// -------------------------
// 애니메이션 상태 타입 정의
// -------------------------
enum class AnimationType {
    Stand,     // 대기 상태
    Alert,     // 경계 상태
    Move,      // 이동 상태
    Jump,      // 점프 상태
    Attack,    // 공격 상태
    Die,       // 사망 상태
    Hit        // 피격 상태
};

// -------------------------
// 리소스 경로 상수 정의
// (절대 경로 사용 중)
// -------------------------

// 맵 관련 이미지
#define LoadingMapPath        "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\LoadingMap\\"
#define LoginMapPath          "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\LoginMap\\"

// 캐릭터 및 몬스터 리소스
#define Player                "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\Player\\"
#define Monster               "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\Monster\\"

// UI 요소
#define BottomUI              "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\BottomUI\\"

// 데미지 관련 이펙트
#define Dmgs                  "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\Dmg\\"

// 레벨업 이펙트
#define LevelUp               "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\pic\\LvUp\\"

// 스킬 관련 이미지 및 사운드
#define SkillPath             "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\Skill\\"

// 배경 음악 경로
#define BackgroundMusicPath   "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\Music\\BackgroundMusic\\"

// 스킬 효과음
#define SkillMusicPath        "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\Music\\Skill\\"

// 공격 효과음
#define AttackMusicPath       "C:\\Users\\KGA\\source\\repos\\MappleStory\\MappleStory\\Resource\\Music\\Attack\\"
