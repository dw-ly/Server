#include "CTimeWhell.h"

CTimeWhell::CTimeWhell(int step_num, string name, int plies)
{
    m_plies = plies;
    m_name = name;
    m_step_num = step_num;
    m_step = 0;
    set<int> task_list;
    for (int i = 0; i < m_step_num; i++)
    {
        m_whell_task_list.emplace_back(task_list);
    }
}

CTimeWhell::~CTimeWhell()
{
}

/**
 * @brief 获取当前step所有任务id
 * 
 * @param taks_ids 
 */
void CTimeWhell::getNowTaskIds(set<int>& taks_ids)
{
    taks_ids = m_whell_task_list[m_step];
}

/**
 * @brief 处理时间轮跳一步，处理注入的回调函数
 * 
 */
void CTimeWhell::onStep()
{
    if (m_step < m_step_num - 1)
    {
        m_step++;
        printf("onStep %s %d/%d\n",m_name.c_str(), m_step, m_step_num);
    }
    else
    {
        m_step = 0;
        m_step_call_back();
    }
    m_task_call_back(this); 
}

void CTimeWhell::onAddTask(set<int> task_ids, int step)
{
    for (auto task_id : task_ids)
    {
        onAddTask(task_id, step);
    }
}

/**
 * @brief 记录任务id到该时间轮
 * 
 * @param task_id 
 * @param step 
 */
void CTimeWhell::onAddTask(int task_id, int step)
{
    int idx = 0;
    if (m_step + step > m_step_num)
    {
        idx = m_step + step - m_step_num;
    }
    else
    {
        idx = m_step + step;
    }
    
     m_whell_task_list[idx].emplace(task_id);
}

/**
 * @brief 设置时间轮回调函数
 * 
 * @param callback 
 */
void CTimeWhell::setCallback(function<void()> callback)
{
    m_step_call_back = callback;
}

void CTimeWhell::setTaskCallback(function<void(CTimeWhell*)> callback)
{
    m_task_call_back = callback;
}
