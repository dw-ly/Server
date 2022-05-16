#include "CEventLoop.h"

CEventLoop::CEventLoop(/* args */)
{
    m_event_loop = new SEventLoop;
    init();
}

CEventLoop::~CEventLoop()
{
    if (m_event_loop->dispatcher != nullptr)
    {
        delete m_event_loop->dispatcher;
    }
    // if (m_event_loop->event_dispatcher_data != nullptr)
    // {
    //     delete m_event_loop->event_dispatcher_data;
    // }
    delete m_event_loop;
}

void CEventLoop::init(char *thread_name)
{
    
    m_event_loop->thread_name = thread_name;
    m_event_loop->owner_thread_id = pthread_self();
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_event_loop->socketPair) < 0) {

    }
    m_channel_handle->init(m_event_loop->socketPair[1], EVENT_READ, nullptr, nullptr, nullptr);
    timeval *time;
    ChannelMap channels;
    m_channel_handle->GetChannelMap(channels);
    m_event_loop->dispatcher = new CEpollDispatcher(this);
    // m_event_loop->dispatcher = new SEventDispatcher();
    // m_event_loop->dispatcher->event_init = m_event_funcs->Init;
    // m_event_loop->dispatcher->event_add = m_event_funcs->Add;
    // m_event_loop->dispatcher->event_del = m_event_funcs->Del;
    // m_event_loop->dispatcher->event_update = m_event_funcs->Update;
    // m_event_loop->dispatcher->event_dispatch = m_event_funcs->Dispatch;
    // m_event_loop->dispatcher->event_clear = m_event_funcs->Clear;

}

CChannelHandle *CEventLoop::getChannelHandle()
{
    return m_channel_handle;
}
void CEventLoop::routine()
{
    assert(m_event_loop != nullptr);
    timeval * time;
    time->tv_sec = 1;
    while (!m_event_loop->quit)
    {
        m_event_loop->dispatcher->Dispatch(m_event_loop, time);
    }
    
}