#include "CTimeWhell.h"
#include "CThreadPool.h"
#include <functional>
#include <mutex>
#define WHELL_NUM 3
#define ONE_WHELL_STEP 60
#define ONE_WHELL_STEP_HOUR 24
class CWhellMgr
{
private:
    /* data */
public:
    map<int/*task_id*/, tuple<vector<int>/*idx*/,Task>> m_task_map;
    CTimeWhell* m_time_whell[WHELL_NUM];
    CTimer* m_timer;
    mutex m_mutex;
    CThreadPool* m_thread_pool;
    CWhellMgr(/* args */);
    ~CWhellMgr();
    void onTimeWhellStart();
    void onAddTask(int time, int task_id, Task task);
    void onDoTask(set<int> task_ids);
    void onStepDoTask(CTimeWhell* timeWhell);
};