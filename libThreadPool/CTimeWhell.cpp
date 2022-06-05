#include "CTimeWhell.h"

CTimeWhell::CTimeWhell(/* args */)
{
}

CTimeWhell::~CTimeWhell()
{
}

void CTimeWhell::Init()
{    
    m_init_time = getCurrentTime();
    m_max_during_time = pow((double)OneWhell, WhellNum);
    for (auto task : m_wait_add_tasks)
    {
        addTask(task.second.first, task.first, task.second.second);
    }
    for (auto iter = m_wait_add_tasks.begin(); iter != m_wait_add_tasks.end(); iter = m_wait_add_tasks.erase(iter)){}
    printf("CTimeWhell::Init\n");
    m_timer = CTimer::GetInstance();
    // wait_add_tasks.clear();
}
void CTimeWhell::check()
{
    auto getIndex = [this](int index, int idx)->int{
        int tmp = m_current_index[idx] + index;
        return tmp >= OneWhell? tmp - OneWhell: tmp;
    };
    // while (1)
    {   
        // // 修正游标位置
        // for (auto &index : current_index)
        // {
        //     if (index == OneWhell)
        //     {
        //         index = 0;
        //     }
        // }

        // sleep(1);
        //秒级时间轮游标是否指到最后一格
        set<int> &tasks = m_whell[0].m_task_lists[m_current_index[0]];
        //执行当前任务
        for (auto task : tasks)
        {
            m_task_map[task]();
            m_task_map.erase(task);
            printf("do task %d\n",task);
        }
        for (auto iter = tasks.begin(); iter != tasks.end(); iter = tasks.erase(iter)){}
        assert(tasks.empty());

        m_current_index[0]++;
        int flag = 0;
        if(m_current_index[0] == OneWhell)
        {
            flag = 1;
            m_current_index[0] = 0;
        }

        for (int idx = 1; idx < WhellNum; idx++)
        {
            int &index = m_current_index[idx];
            //修正游标位置
            int tmp = 0;
            if (index == OneWhell)
            {
                index = 0;
                tmp = 1;
            }

            //上级时间轮游标指到最后一格
            if (flag == 1)
            {
                printf("CTimeWhell::check index=%d idx=%d\n",index, idx);
                if(idx == 0)
                {

                }
                else if (idx <= WhellNum - 1)
                {
                    int _index;
                    set<int> &tasks = m_whell[idx].m_task_lists[index + 1];
                    // printf("CTimeWhell::check index=%d tasks.size=%ld\n", index, tasks.size());
                    for(auto task : tasks)
                    {
                        // printf("CTimeWhell::check task=%d _index=%d\n", task, _index);
                        _index = m_task_record[task][idx - 1];
                        // printf("CTimeWhell::check task=%d indexs=%d\n", task, _index);
                        m_whell[idx - 1].m_task_lists[_index].emplace(task);
                        // printf("CTimeWhell::check task=%d _index=%d\n", task, _index);
                    }
                    
                    for (auto iter = tasks.begin(); iter != tasks.end(); iter = tasks.erase(iter)){}
                    assert(tasks.empty());
                }
                // else if(idx == WhellNum - 1)
                // {

                // }
                index++;
            }
            flag = tmp;
        }
    }
    printf("CTimeWhell::check m_current_index[%d,%d,%d]\n",m_current_index[0], m_current_index[1], m_current_index[2]);
}

void CTimeWhell::addTask(int64_t time, int task_id, task one)
{
    int64_t during = time - m_init_time;
    addTask(during, task_id, one);
}
void CTimeWhell::addTask(int during, int task_id, task one)
{   
    int flag = 0;
    if(during > m_max_during_time)
    {
        m_wait_add_tasks.emplace(task_id, make_pair(during - getEndTime() + getCurrentTime(),one));
    }
    else
    {
        int indexs[WhellNum] = {0};
        getIndexs(during, indexs);

        for (int i = 0; i < WhellNum; i++)
        {
            m_whell[i].m_index = indexs[i];
            if(indexs[i] != 0) flag = i;
        }
        m_whell[flag].m_task_lists[indexs[flag]].emplace(task_id);
        m_task_map.emplace(task_id, one);
        for (int i = 0; i < WhellNum; i++)
        {
            m_task_record[task_id][i] = indexs[i];
        }        
    }
}

int64_t CTimeWhell::getCurrentTime()
{
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

int64_t CTimeWhell::getEndTime()
{
    int64_t during = 0;
    for (int upperWhellNum = 1; upperWhellNum <= UpperWhellNum; upperWhellNum++)
    {
        during += pow((double)OneWhell, upperWhellNum + 1);
    }
    return m_init_time + during + OneWhell;
}

void CTimeWhell::routine()
{
    m_timer->start(bind(&CTimeWhell::check, this));
    printf("CTimeWhell::routine\n");
}

void CTimeWhell::getIndexs(int during, int* index)
{
    // int index[UpperWhellNum + 1];
    int tmp;
    for (int upperWhellNum = 0; upperWhellNum < UpperWhellNum + 1; upperWhellNum++)
    {
        tmp = during / pow((double)OneWhell, upperWhellNum);
        index[upperWhellNum] = tmp % OneWhell;
    }
    printf("CTimeWhell::getIndexs index[%d,%d,%d]\n",index[0], index[1], index[2]);
    // return index;
}

int CTimeWhell::getDuring(int* indexs)
{
    int during = 0;
    for (int upperWhellNum = 0; upperWhellNum < UpperWhellNum + 1; upperWhellNum++)
    {
        during += indexs[upperWhellNum] * pow((double)OneWhell, upperWhellNum);
    }
    return during;
}