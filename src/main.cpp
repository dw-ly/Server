#include "CEventLoop.h"
// #include "CTimeWhell.h"
#include "CWhellMgr.h"
int main()
{
    // CEventLoop *event_loop;
    // CTimeWhell timeWhell;
    // timeWhell.Init();
    // timeWhell.addTask(1, 1, [](){printf("task\n");});
    // timeWhell.addTask(1, 2, [](){printf("task2\n");});
    // timeWhell.addTask(4, 3, [](){printf("task3\n");});
    // timeWhell.addTask(10, 4, [](){printf("task3\n");});
    // timeWhell.addTask(18, 5, [](){printf("task3\n");});
    // timeWhell.routine();
    
    
    CWhellMgr* mgr = new CWhellMgr;
    // mgr->onTimeWhellStart();
    mgr->onAddTask(15, 1, [](){printf("task\n");});
    mgr->onAddTask(15, 2, [](){printf("task\n");});
    mgr->onTimeWhellStart();
    // CTimer* timer = CTimer::GetInstance();
    // timer->start([](){printf("test\n");});
    sleep(77);
    // event_loop->routine();
    return 0;
}