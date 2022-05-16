#include "CEpollDispatcher.h"
CEpollDispatcher::CEpollDispatcher(/* args */)
{
}

CEpollDispatcher::~CEpollDispatcher()
{
}

int CEpollDispatcher::Init(SEventLoop* event_loop)
{
    SEpollDispatcherData* dispatcherData = new SEpollDispatcherData;
    dispatcherData->event_count = 0;
    dispatcherData->event_fd = 0;
    dispatcherData->nfds = 0;
    dispatcherData->event_fd = epoll_create1(0);
    dispatcherData->events = 0;
    event_loop->event_dispatcher_data = dispatcherData;

}

int CEpollDispatcher::Add(SEventLoop* event_loop, SChannel* channel)
{

}

int CEpollDispatcher::Del(SEventLoop* event_loop, SChannel* channel)
{

}

int CEpollDispatcher::Update(SEventLoop* event_loop, SChannel* channel)
{

}

int CEpollDispatcher::Dispatch(SEventLoop* event_loop, timeval* time_val)
{

}

int CEpollDispatcher::Clear(SEventLoop* event_loop)
{
    SEpollDispatcherData *dispatcherData = (SEpollDispatcherData*)event_loop->event_dispatcher_data;
    close(dispatcherData->event_fd);
}