#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <unistd.h>
#include <thread>
#include <vector>
#include <ctime>
#include <sys/prctl.h>
#include <atomic>
using namespace std;
#define MAX_THREAD_NUM 4;//thread::hardware_concurrency();
using Task = function<void()>; 
class CThreadPool
{
public:
    int max;
    bool done;
    atomic_int freeNum;
    queue<Task> m_tasks;
    vector<thread> m_threads;
    std::mutex m_lock;
    std::condition_variable cv;
public:
    CThreadPool(); 
    ~CThreadPool();
    void initThread(int size);
    void start();
    void stop();
    void addTask(Task task/*, void* argstest*/);
    void rountine(int tid);
};


