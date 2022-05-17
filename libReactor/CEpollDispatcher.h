#pragma once
#include "CDispatcher.h"
#define MAXEVENTS 128
class CEventLoop;
class CEpollDispatcher : public CDispatcher
{
private:
    CEventLoop *m_event_loop;
public:
    CEpollDispatcher(CEventLoop *event_loop);
    ~CEpollDispatcher();
    int Init(SEventLoop* event_loop) override;
    int Add(SEventLoop* event_loop, SChannel* channel) override;
    int Del(SEventLoop* event_loop, SChannel* channel) override;
    int Update(SEventLoop* event_loop, SChannel* channel) override;
    int Dispatch(SEventLoop* event_loop, timeval* time_val) override;
    int Clear(SEventLoop* event_loop) override;
};


