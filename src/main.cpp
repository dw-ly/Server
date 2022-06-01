#include "CEventLoop.h"
#include "CTimeWhell.h"
int main()
{
    // CEventLoop *event_loop;
    CTimeWhell timeWhell;
    timeWhell.Init();
    timeWhell.addTask(1, 1, [](){printf("task\n");});
    timeWhell.routine();
    // CTimer* timer = CTimer::GetInstance();
    // timer->start([](){printf("test\n");});
    sleep(10);
    // event_loop->routine();
    return 0;
}