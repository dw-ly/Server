#include "CThreadPool.h"
void CThreadPool::initThread(int size)
{
    for (int i = 0; i < size; i++)
    {
    	// printf("add rountine\n");
        m_threads.emplace_back(thread(&CThreadPool::rountine, this, i));
    }
}

void CThreadPool::start()
{
    initThread(max);
}
void CThreadPool::stop()
{
    // unique_lock<std::mutex> lk(m_lock);
    // printf("this is stop\n");
    cv.notify_all();
    for (int i = 0; i < max; i++)
    {
        printf("stop thread %d\n", i);
        if (m_threads[i].joinable())// 等待任务结束， 前提：线程一定会执行完
        {
            printf("join thread %d\n", i);
            m_threads[i].join();
        }
    }
    done = false;//单独调用stop时关闭析构函数中stop
    
    // printf("after join\n");
}

CThreadPool::CThreadPool(/* args */)
{
    max = MAX_THREAD_NUM;
    done = true;
    freeNum = MAX_THREAD_NUM;
    printf("max thread num = %d\n", max);
    start();
}

CThreadPool::~CThreadPool()
{
    if (done)
    {
        stop();
    }
    // for (auto &t : threads)
    // {
    //     t.join();
    // }
}

void CThreadPool::addTask(Task task/*, void* args*/)
{
    unique_lock<std::mutex> lk(m_lock);
    // printf("this is add_task:%d\n", num);
    m_tasks.emplace(task);
    // printf("tasks num:%d\n", tasks.size());
    // if (freeNum > 0)
    // {
        cv.notify_one();
    // }
}
void CThreadPool::rountine(int tid)
{
    // printf("start new_routine\n");
    string s = "mthread";
    // s += (++i+'0');//char
    s += to_string(tid++);//int
    printf("%s\n",s.c_str());
    prctl(PR_SET_NAME, s.c_str());
    int time_out = 0;
    while (1)
    {
        // printf("--------wait for task-------\n");
        unique_lock<std::mutex> lk(m_lock);
        if(m_tasks.empty())
        {
            time_out ++;
            cv.wait_for(lk, chrono::seconds(5));//设置超时
            sleep(0);
            if (time_out >= 100)
            {
                printf("time_out");
                return;
            }
            
        }
        else
        {
            time_out = 0;
            if (freeNum > 0)
            {
                auto task = move(m_tasks.front());
                m_tasks.pop();
                //printf("start task\n");
                freeNum--;
                task();
                freeNum++;
            }
            else
            {
                sleep(0);
                // cv.wait(lk);
            }
        }
        
    }
    // if (freeNum < MAX_THREAD_NUM)
    // {
    //     done = false;
    // }
    // else
    // {
    //     done = true;
    // }
    
}


