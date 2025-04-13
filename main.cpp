#include "Component/Schedule/Schedule.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    ScheduleManager ScheduleManagers(hInstance);
    ScheduleManagers.Update(hInstance);
   
    return 0;
}