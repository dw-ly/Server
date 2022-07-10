#pragma once
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <assert.h>
#include "CThreadPool.h"
#include "CTimer.h"

class CTimeWhell
{
private:
    /* data */
public:
    int m_step_num; //总步数
    int m_step;     //当前步数
    int m_plies;    //时间轮层数
    string m_name;
    vector<set<int>> m_whell_task_list;
    // set<pair<int,int>> m_task_time;
    function<void()> m_step_call_back;      //当前时间轮走完一圈时触发的回调函数
    function<void(CTimeWhell*)> m_task_call_back;   //当前时间轮每走一步触发的回调函数
    CTimeWhell(int step_num, string name, int plies);
    ~CTimeWhell();
    void getNowTaskIds(set<int>& task_ids);
    void onStep();
    void onAddTask(int task_id, int step);
    void onAddTask(set<int> task_ids, int step);

    void setCallback(function<void()> callback);
    void setTaskCallback(function<void(CTimeWhell*)> callback);
    // void onCircle();
}; 