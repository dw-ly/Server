// #include "CDispatcher.h"
// #include "CEventLoop.h"
// CDispatcher::CDispatcher(CEventLoop *event_loop) : m_event_loop(event_loop)
// {

// }
// CEpollDispatcher::CEpollDispatcher(CEventLoop *event_loop) : CDispatcher(event_loop)
// {
//     m_event_loop = event_loop;
// }

// CEpollDispatcher::~CEpollDispatcher()
// {
//     m_event_loop = nullptr;
// }

// int CEpollDispatcher::Init(SEventLoop* event_loop)
// {
//     event_loop->event_dispatcher_data = new SEpollDispatcherData();
//     SEpollDispatcherData *dispatcherData = (SEpollDispatcherData*)event_loop->event_dispatcher_data;
//     dispatcherData->event_count = 0;
//     dispatcherData->event_fd = 0;
//     dispatcherData->nfds = 0;
//     dispatcherData->event_fd = epoll_create1(0);
//     dispatcherData->events = 0;
//     return 0;
// }

// int CEpollDispatcher::Add(SEventLoop* event_loop, SChannel* channel)
// {
//     SEpollDispatcherData *dispatcherData = (SEpollDispatcherData*)event_loop->event_dispatcher_data;
//     int fd = channel->fd;
//     int events = 0;
//     if (channel->events & EVENT_READ)
//         events = events | EPOLLIN;
//     if(channel->events & EVENT_WRITE)
//         events = events | EPOLLOUT;
//     epoll_event event;
//     event.data.fd = fd;
//     event.events = events;
//     if (epoll_ctl(dispatcherData->event_fd, EPOLL_CTL_ADD, fd, &event) == -1)
//     {
//         printf("epoll_ctl add fd fail!");
//     }
//     return 0;    
// }

// int CEpollDispatcher::Del(SEventLoop* event_loop, SChannel* channel)
// {
//     SEpollDispatcherData *dispatcherData = (SEpollDispatcherData*)event_loop->event_dispatcher_data;
//     int fd = channel->fd;
//     int events = 0;
//     if (channel->events & EVENT_READ)
//         events = events | EPOLLIN;
//     if(channel->events & EVENT_WRITE)
//         events = events | EPOLLOUT;
//     epoll_event event;
//     event.data.fd = fd;
//     event.events = events;
//     if (epoll_ctl(dispatcherData->event_fd, EPOLL_CTL_DEL, fd, &event) == -1)
//     {
//         printf("epoll_ctl del fd fail!");
//     }
//     return 0;    
// }

// int CEpollDispatcher::Update(SEventLoop* event_loop, SChannel* channel)
// {
//     SEpollDispatcherData *dispatcherData = (SEpollDispatcherData*)event_loop->event_dispatcher_data;
//     int fd = channel->fd;
//     int events = 0;
//     if (channel->events & EVENT_READ)
//         events = events | EPOLLIN;
//     if(channel->events & EVENT_WRITE)
//         events = events | EPOLLOUT;
//     epoll_event event;
//     event.data.fd = fd;
//     event.events = events;
//     if (epoll_ctl(dispatcherData->event_fd, EPOLL_CTL_MOD, fd, &event) == -1)
//     {
//         printf("epoll_ctl update fd fail!");
//     }
//     return 0;    
// }

// int CEpollDispatcher::Dispatch(SEventLoop* event_loop, timeval* time_val)
// {
//     SEpollDispatcherData *dispatcherData = (SEpollDispatcherData*)event_loop->event_dispatcher_data;
//     int fd_nums = epoll_wait(dispatcherData->event_fd, dispatcherData->events, MAXEVENTS, -1);
//     for (int i = 0; i < fd_nums; i++)
//     {
//         if ((dispatcherData->events[i].events & EPOLLERR) || (dispatcherData->events[i].events & EPOLLHUP))
//         {
//             close(dispatcherData->events[i].data.fd);
//             continue;
//         }
//         if(dispatcherData->events[i].events & EPOLLIN)
//             m_event_loop->getChannelHandle()->eventActivation(dispatcherData->events[i].data.fd, EVENT_READ);
//         if(dispatcherData->events[i].events & EPOLLOUT)
//             m_event_loop->getChannelHandle()->eventActivation(dispatcherData->events[i].data.fd, EVENT_WRITE);
//     }
//     return 0;
// }

// int CEpollDispatcher::Clear(SEventLoop* event_loop)
// {
//     SEpollDispatcherData *dispatcherData = (SEpollDispatcherData*)event_loop->event_dispatcher_data;
//     close(dispatcherData->event_fd);
//     delete dispatcherData->events;
//     delete dispatcherData;
//     event_loop->event_dispatcher_data = nullptr;
//     return 0;
// }