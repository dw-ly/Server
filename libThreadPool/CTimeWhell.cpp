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
            printf("do task\n");
        }
        for (auto iter = time_whell_now[current_index[0]].begin(); iter != time_whell_now[current_index[0]].end(); iter = time_whell_now[current_index[0]].erase(iter)){}
        assert(time_whell_now[current_index[0]].empty());
        // sleep(1);
        //秒级时间轮游标是否指到最后一格
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
                switch (idx)
                {
                case 0:
                    for (auto tasks : time_whell_upper[idx][current_index[index]])
                    {
                        time_whell_now[current_index[index]].emplace(tasks);
                        time_whell_upper[idx][current_index[index]].erase(tasks);
                    }
                    break;
                case UpperWhellNum:
                
                    break;
                default:
                    if (idx >= UpperWhellNum + 1 || idx < 0)
                    {

                    }
                    else
                    {
                        for (auto tasks : time_whell_upper[idx + 1][current_index[index + 1]])
                        {
                            time_whell_upper[idx][current_index[index]].emplace(tasks);
                            time_whell_upper[idx + 1][current_index[index + 1]].erase(tasks);
                        }
                    }
                    break;
                }
                index++;
            }
            flag = tmp;
        }
    }
    printf("CTimeWhell::check\n");
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
        int index = during + current_index[idx] - OneWhell;
        if (index > OneWhell)
        {
            index -= OneWhell;
        }
        if (index < 0)
        {
            index += OneWhell;
        }
        
        printf("CTimeWhell::addTask index=%d", index);
        return index;
    };
    //在首层
    if (during < OneWhell)
    {
        time_whell_now[getIndex(during, 0)].emplace(task_id);
        task_map.emplace(task_id, one);
        record[task_id] = make_pair(0, during);
        printf("CTimeWhell::addTask add to time_whell_now");
    }
    else
    {
        int add_flag = 0;
        for (int upperWhellNum = 1; upperWhellNum <= UpperWhellNum; upperWhellNum++)
        {
            if (during % (OneWhell * (upperWhellNum + 1)) == 0)
            {
                time_whell_upper[upperWhellNum - 1][getIndex(during, upperWhellNum)].emplace(task_id);
                task_map.emplace(task_id, one);
                record[task_id] = make_pair(upperWhellNum, during);
                int add_flag = 1;
                printf("CTimeWhell::addTask add to time_whell_upper");
                break;
            }
        }
        //超出时间轮最大时间，添加到待添加任务列表中
        if (add_flag == 0)
        {
            wait_add_tasks.emplace(task_id, make_pair(during - getEndTime() + getCurrentTime(),one));
            printf("CTimeWhell::addTask add to wait_add_tasks");
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