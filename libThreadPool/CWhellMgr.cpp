#include "CWhellMgr.h"

CWhellMgr::CWhellMgr(/* args */)
{
    for (int i = 0; i < WHELL_NUM; i++)
    {
        m_time_whell[i] = new CTimeWhell(ONE_WHELL_STEP, "", i);
    }
    m_time_whell[0]->m_name = "s";
    m_time_whell[1]->m_name = "min";
    m_time_whell[2]->m_name = "h";
}

CWhellMgr::~CWhellMgr()
{
    for (int i = 0; i < WHELL_NUM; i++)
    {
        delete m_time_whell[i];
    }
    
}

void CWhellMgr::onTimeWhellStart()
{
    set<pair<int, int>> tasks;
    //设置每级时间轮的callback
    for (int i = 0; i < WHELL_NUM - 1; i++)
    {
        m_time_whell[i]->setCallback(bind(&CTimeWhell::onStep, m_time_whell[i + 1]));
    }
    m_time_whell[WHELL_NUM - 1]->setCallback([](){});

    //设置每级时间轮的taskcallback
    for (int i = 0; i < WHELL_NUM; i++)
    {
        m_time_whell[i]->setTaskCallback(bind(&CWhellMgr::onStepDoTask, this, placeholders::_1));
    }

    /*
        开启时间轮,每级时间轮走完一圈时会调用上一级时间轮的onStep，这样就可以下级时间轮走完一圈时同步给上级时间轮走一步。
    */
    m_timer->GetInstance()->start(bind(&CTimeWhell::onStep, m_time_whell[0])); 
}
/**
 * @brief 添加任务
 * 
 * @param time 等待时间
 * @param task_id 任务id
 * @param task 任务方法
 */
void CWhellMgr::onAddTask(int time, int task_id, Task task)
{
    int tmp;
    vector<int> idx;
    for (int step = 0; step < WHELL_NUM; step++)
    {
        tmp = time / pow((double)ONE_WHELL_STEP, step);
        // idx[step] = tmp % ONE_WHELL_STEP;
        idx.emplace_back(tmp % ONE_WHELL_STEP);
    }
    printf("CWhellMgr::onAddTask idx[%d,%d,%d]\n",idx[0], idx[1], idx[2]);
    m_task_map.emplace(task_id, make_tuple(idx, task));
    for (int i = WHELL_NUM - 1; i >= 0; i--)
    {
        if (idx[i] != 0)
        {
            m_time_whell[i]->onAddTask(task_id, idx[i]);
            break;        
        }
    }
}

/**
 * @brief 执行任务
 * 
 * @param task_ids 
 */
void CWhellMgr::onDoTask(set<int> task_ids)
{
    for (auto task_id : task_ids)
    {
        if (m_task_map.find(task_id) != m_task_map.end())
        {
            get<1>(m_task_map[task_id])();
            printf("onDoTask:%d\n", task_id);
            m_task_map.erase(task_id);
        }
    }
}

/**
 * @brief 控制时间轮每跳一步的动作
 * 
 */
void CWhellMgr::onStepDoTask(CTimeWhell* timeWhell)
{
    set<int> task_ids;
    timeWhell->getNowTaskIds(task_ids);
    int plies = timeWhell->m_plies - 1;
    if (timeWhell->m_plies > 0)
    {
        for (auto task_id : task_ids)
        {
            if (m_task_map.find(task_id) != m_task_map.end())
            {
                int idx = get<0>(m_task_map[task_id])[plies];
                m_time_whell[plies]->onAddTask(task_ids, idx);
            }
        }
    }
    else if(timeWhell->m_plies == 0)
    {
        onDoTask(task_ids);        
    }
}