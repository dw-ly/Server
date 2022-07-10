#pragma once
#include <map>
#include <set>
#include <cmath>
#include <assert.h>
#include "CThreadPool.h"
#include "CTimer.h"

#define OneWhell 8
#define UpperWhellNum 2 
using task = function<void()>;
class CTimeWhell
{
private:
    set<int/*task_id*/> time_whell_now[OneWhell];
    set<int/*task_id*/> time_whell_upper[UpperWhellNum][OneWhell];
    map<int/*task_id*/,pair<int/*level*/, int* /*index*/>> record;        //任务信息记录
    map<int/*task_id*/, task> task_map;                                 //任务函数
    map<int/*task_id*/, pair<int/*delay_time*/, task>> wait_add_tasks;  //待添加任务列表
    int current_index[UpperWhellNum + 1] = {0};
    int64_t init_time;
    CTimer* m_timer;
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


