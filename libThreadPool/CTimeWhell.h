#pragma once
#include <map>
#include <set>
#include <cmath>
#include <assert.h>
#include "CThreadPool.h"
#include "CTimer.h"

#define OneWhell 8
#define UpperWhellNum 2 
#define WhellNum 3 
using task = function<void()>;
struct STimeWhell
{
    int m_index;
    int m_step;
    set<int/*task_id*/> m_task_lists[OneWhell];
};

class CTimeWhell
{
private:    
    int64_t m_init_time;
    int64_t m_max_during_time;
    int m_current_index[WhellNum] = {0};
    
    STimeWhell m_whell[WhellNum];
    CTimer* m_timer;

    map<int/*task_id*/, int[WhellNum] /*index[WhellNum]*/> m_task_record;          //任务信息记录
    map<int/*task_id*/, task> m_task_map;                                 //任务函数
    
    map<int/*task_id*/, pair<int/*delay_time*/, task>> m_wait_add_tasks;  //待添加任务列表
public:
    CTimeWhell(/* args */);
    ~CTimeWhell();
    void Init();
    void addTask(int64_t time, int task_id, task one);
    void addTask(int during, int task_id, task one);

    int64_t getCurrentTime();
    int64_t getEndTime();
    void getIndexs(int during, int* indexs);
    int getDuring(int* indexs);
    void check();
    void routine();
};


