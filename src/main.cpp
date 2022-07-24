#include "CEventLoop.h"
// #include "CTimeWhell.h"
#include "CWhellMgr.h"
#include "CThreadPool.h"
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
    
    CThreadPool* pool = new CThreadPool();
    CWhellMgr* mgr = new CWhellMgr;
    // mgr->onTimeWhellStart();
    pool->start();
    int *tmp = NULL;
    // pool->addTask([](){printf("----task\n");},(void*)tmp);
    // pool->addTask([](){printf("----task\n");},(void*)tmp);
    // pool->addTask([](){printf("----task\n");},(void*)tmp);
    // pool->addTask([](){printf("----task\n");},(void*)tmp);
    // pool->addTask([](){printf("----task\n");},(void*)tmp);
    for (int i = 0; i < 200000; i++)
    {
        pool->addTask(bind(&CWhellMgr::onAddTask, mgr, (int)(15 + i/10000), i, [i](){printf("-----pool-----task------%d\n",i);}));
    }
    
    pool->addTask(bind(&CWhellMgr::onAddTask, mgr, 19, 1000002, [](){sleep(1);printf("-----pool-----task------sleep\n");}));
    pool->addTask(bind(&CWhellMgr::onAddTask, mgr, 19, 1000003, [](){sleep(1);printf("-----pool-----task------sleep\n");}));
    pool->addTask(bind(&CWhellMgr::onAddTask, mgr, 19, 1000004, [](){printf("-----pool-----task------\n");}));
    pool->addTask(bind(&CWhellMgr::onAddTask, mgr, 19, 1000005, [](){printf("-----pool-----task------\n");}));
    // mgr->onAddTask(15, 1, [](){printf("task\n");});
    // mgr->onAddTask(15, 2, [](){printf("task\n");});
    mgr->onTimeWhellStart();
    // CTimer* timer = CTimer::GetInstance();
    // timer->start([](){printf("test\n");});
    
    getchar();
    printf("%d", mgr->m_time_whell[0]->m_whell_task_list[25].size());
    // sleep(77);
    // event_loop->routine();
    return 0;
}