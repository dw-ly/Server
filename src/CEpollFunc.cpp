#include "CEpollFunc.h"

int CEpollFunc::Init(SEventLoop* event_loop)
{
    SEpollDispatcherData* dispatcherData = new SEpollDispatcherData;
    dispatcherData->event_count = 0;
    dispatcherData->event_fd = 0;
    dispatcherData->nfds = 0;
    dispatcherData->event_fd = epoll_create1(0);
    dispatcherData->events = 0;
    event_loop->event_dispatcher_data = dispatcherData;

}

int CEpollFunc::Add(SEventLoop* event_loop, SChannel* channel)
{

}

int CEpollFunc::Del(SEventLoop* event_loop, SChannel* channel)
{

}

int CEpollFunc::Update(SEventLoop* event_loop, SChannel* channel)
{

}

int CEpollFunc::Dispatch(SEventLoop* event_loop, timeval* time_val)
{

}

int CEpollFunc::Clear(SEventLoop* event_loop)
{
    SEpollDispatcherData *dispatcherData = (SEpollDispatcherData*)event_loop->event_dispatcher_data;
    close(dispatcherData->event_fd);
}