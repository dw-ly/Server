#include "CEventLoop.h"

CEventLoop::CEventLoop(/* args */)
{
    m_event_loop = new SEventLoop;
    this->init("main thread");
}

CEventLoop::~CEventLoop()
{
    if (m_event_loop->dispatcher != nullptr)
    {
        m_event_loop->dispatcher->Clear(m_event_loop);
        delete m_event_loop->dispatcher;
    }
    // if (m_event_loop->event_dispatcher_data != nullptr)
    // {
    //     delete m_event_loop->event_dispatcher_data;
    // }
    delete m_event_loop;
}

void CEventLoop::init(const char *thread_name)
{
    
    m_event_loop->thread_name = const_cast<char*>(thread_name);
    m_event_loop->owner_thread_id = pthread_self();
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_event_loop->socketPair) < 0) {

    }
    m_channel_handle->init(m_event_loop->socketPair[1], EVENT_READ, nullptr, nullptr, nullptr);
    timeval *time;
    ChannelMap channels;
    m_channel_handle->GetChannelMap(channels);
    m_event_loop->dispatcher = new CEpollDispatcher(this);
    m_event_loop->dispatcher->Init(m_event_loop);
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

int CEventLoop::addEventLoopHandle(int fd, SChannel *channel)
{
    ChannelMap map;
    m_channel_handle->GetChannelMap(map);
    if(fd < 0)
        return 0;
    
    if(map.find(fd) == map.end())
    {
        map[fd] = channel;
        m_event_loop->dispatcher->Add(m_event_loop, channel);
    }    
    return 0;
}

int CEventLoop::updateEventLoopHandle(int fd, SChannel *channel)
{
    ChannelMap map;
    m_channel_handle->GetChannelMap(map);
    if(fd < 0)
        return 0;
    
    if(map.find(fd) == map.end()) //fd>map中key最大值时
        return -1;

    m_event_loop->dispatcher->Update(m_event_loop, map[fd]);
    return 0;
}

int CEventLoop::removeEventLoopHandle(int fd, SChannel *channel)
{
    ChannelMap map;
    m_channel_handle->GetChannelMap(map);
    if(fd < 0)
        return 0;

    if(map.find(fd) == map.end()) //fd>map中key最大值时
        return -1;
    int ret = 0;
    if(m_event_loop->dispatcher->Del(m_event_loop, map[fd]) == -1) //TODO 修改返回值规则
        ret = -1;
    else
        ret = 1;
    map[fd] = nullptr;
    
    return ret;    
}

int CEventLoop::wakeUpEventLoop()
{
    char tmp = 'a';
    ssize_t n = write(m_event_loop->socketPair[0], &tmp, sizeof(tmp));
    if (n != sizeof(tmp))
    {
        printf("fail");
    }
    return 0;
}

int CEventLoop::wakeUpHandle()
{
    char tmp;
    ssize_t n = read(m_event_loop->socketPair[1], &tmp, sizeof(tmp));
    if (n != sizeof(tmp))
    {
        printf("fail");
    }   
    return 0;
}