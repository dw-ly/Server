#include "CTimer.h"
CTimer* CTimer::m_instance = nullptr;
CTimer::CTimer(/* args */)
{
    
}

CTimer::~CTimer()
{
    if(m_instance->m_thread.joinable()) m_instance->m_thread.join();
    delete m_instance;
}

CTimer* CTimer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new CTimer;
    }
    return m_instance;
}

void CTimer::start(callback timerCallback)
{
    m_instance->m_callback = timerCallback;
    m_instance->m_status = 1;
    m_instance->m_thread = std::thread(&CTimer::workPerSecond, m_instance);
    // m_callback = timerCallback;
    // m_status = 1;
    // m_thread = std::thread(&CTimer::workPerSecond, this);
}

void CTimer::stop()
{
    m_instance->m_status = 0;
    // m_status = 0;
}

void CTimer::work()
{
    if (m_instance->m_status)
    {
        m_instance->m_callback();
    }
}

void CTimer::workPerSecond()
{
    while (m_instance->m_status)
    {
        m_instance->m_callback();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printf("CTimer::workPerSecond\n");
    }
    
}