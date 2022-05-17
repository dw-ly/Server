#pragma once
#include "SDefine.h"
#define MAXEVENTS 128
class CEventLoop;
// struct SEventLoop;
// struct SChannel;

class CDispatcher
{
private:
public:
    CEventLoop *m_event_loop;

    CDispatcher(CEventLoop *event_loop):m_event_loop(event_loop){};
    virtual ~CDispatcher() = default;
    virtual int Init(SEventLoop* event_loop) = 0;
    virtual int Add(SEventLoop* event_loop, SChannel* channel) = 0;
    virtual int Del(SEventLoop* event_loop, SChannel* channel) = 0;
    virtual int Update(SEventLoop* event_loop, SChannel* channel) = 0;
    virtual int Dispatch(SEventLoop* event_loop, timeval* time_val) = 0;
    virtual int Clear(SEventLoop* event_loop) = 0;
};

// class CEpollDispatcher : public CDispatcher
// {
// private:
//     // CEventLoop *m_event_loop;
// public:
//     CEpollDispatcher(CEventLoop *event_loop);
//     ~CEpollDispatcher();
//     int Init(SEventLoop* event_loop) override;
//     int Add(SEventLoop* event_loop, SChannel* channel) override;
//     int Del(SEventLoop* event_loop, SChannel* channel) override;
//     int Update(SEventLoop* event_loop, SChannel* channel) override;
//     int Dispatch(SEventLoop* event_loop, timeval* time_val) override;
//     int Clear(SEventLoop* event_loop) override;
// };