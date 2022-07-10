#include "CTimeWhell.h"

CTimeWhell::CTimeWhell(/* args */)
{
}

CTimeWhell::~CTimeWhell()
{
}

void CTimeWhell::Init()
{    
    init_time = getCurrentTime();
    for (auto task : wait_add_tasks)
    {
        addTask(task.second.first, task.first, task.second.second);
    }
    for (auto iter = wait_add_tasks.begin(); iter != wait_add_tasks.end(); iter = wait_add_tasks.erase(iter)){}
    printf("CTimeWhell::Init\n");
    m_timer = CTimer::GetInstance();
    // wait_add_tasks.clear();
}
void CTimeWhell::check()
{
    auto getIndex = [this](int index, int idx)->int{
        return current_index[idx] + index >= OneWhell? current_index[idx] + index - OneWhell: current_index[idx] + index;
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
        //执行当前任务
        for (auto task : time_whell_now[current_index[0]])
        {
            task_map[task]();
            task_map.erase(task);
            // time_whell_now[current_index[0]].erase(current_index[0]);
            printf("do task %d\n",task);
        }
        for (auto iter = time_whell_now[current_index[0]].begin(); iter != time_whell_now[current_index[0]].end(); iter = time_whell_now[current_index[0]].erase(iter)){}
        assert(time_whell_now[current_index[0]].empty());
        // sleep(1);
        //秒级时间轮游标是否指到最后一格
        current_index[0]++;
        int flag = current_index[0] == OneWhell? 1: 0;

        for (int idx = 0; idx < UpperWhellNum + 1; idx++)
        {
            int &index = current_index[idx];
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
                printf("CTimeWhell::check idx=%d\n", idx);
                if(idx == 0)
                {
                    int _index;
                    for (auto tasks : time_whell_upper[idx][current_index[index] + 1])
                    {
                        // if (tasks != 0)
                        // {
                            _index = getIndex(record[tasks].second[idx], idx);
                            time_whell_now[_index].emplace(tasks);
                            time_whell_upper[idx][current_index[index] + 1].erase(tasks);
                            printf("CTimeWhell::check _index=%d\n", _index);
                        // }
                    }
                }
                else if(idx == UpperWhellNum)
                {

                }
                else if (idx >= UpperWhellNum + 1 || idx < 0)
                {

                }
                else if(idx < UpperWhellNum && idx > 0)
                {
                    int _index;
                    for (auto tasks : time_whell_upper[idx][current_index[index + 1] + 1])
                    {
                        if(tasks != 0)
                        {
                            _index = getIndex(record[tasks].second[idx], idx);
                            time_whell_upper[idx - 1][current_index[index]].emplace(tasks);
                            time_whell_upper[idx][current_index[index + 1] + 1].erase(tasks);
                        }
                    }
                }
                index++;
            }
            flag = tmp;
        }
    }
    printf("CTimeWhell::check current_index[%d,%d,%d]\n",current_index[0], current_index[1], current_index[2]);
}

void CTimeWhell::addTask(int64_t time, int task_id, task one)
{
    int64_t during = time - init_time;
    addTask(during, task_id, one);
}
void CTimeWhell::addTask(int during, int task_id, task one)
{   
    //获取下标
    auto getIndex = [this](int during, int idx)->int{
        // int index = during + current_index[idx] - OneWhell;
        // if (index > OneWhell)
        // {
        //     index -= OneWhell;
        // }
        // if (index < 0)
        // {
        //     index += OneWhell;
        // }
        int indexs[UpperWhellNum + 1];
        getIndexs(during, indexs);
        printf("CTimeWhell::addTask index=%d\n", *(indexs + idx));
        return *(indexs + idx);
    };
    int indexs[UpperWhellNum + 1];
    getIndexs(during, indexs);
    //在首层
    if (during < OneWhell)
    {
        int index = current_index[0] + indexs[0];
        time_whell_now[index].emplace(task_id);
        task_map.emplace(task_id, one);
        record[task_id] = make_pair(0, indexs);
        printf("CTimeWhell::addTask add to time_whell_now index=%d\n",index);
    }
    else
    {
        int add_flag = 0;
        for (int upperWhellNum = 1; upperWhellNum <= UpperWhellNum; upperWhellNum++)
        {
            if (during < pow((double)OneWhell,upperWhellNum + 1) && during > pow((double)OneWhell,upperWhellNum) && add_flag == 0)
            {
                time_whell_upper[upperWhellNum - 1][current_index[upperWhellNum] + indexs[upperWhellNum]].emplace(task_id);
                task_map.emplace(task_id, one);
                record[task_id] = make_pair(upperWhellNum, indexs);
                add_flag = 1;
                printf("CTimeWhell::addTask add to time_whell_upper index=%d\n",current_index[upperWhellNum] + indexs[upperWhellNum]);
                break;
            }
        }
        //超出时间轮最大时间，添加到待添加任务列表中
        if (add_flag == 0)
        {
            wait_add_tasks.emplace(task_id, make_pair(during - getEndTime() + getCurrentTime(),one));
            printf("CTimeWhell::addTask add to wait_add_tasks\n");
            // addTask(getEndTime(), task_id, one);
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
    return init_time + during + OneWhell;
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