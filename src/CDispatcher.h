#include "SDefine.h"
class CDispatcher
{
private:
    /* data */
public:
    CDispatcher(/* args */);
    ~CDispatcher();
    virtual int Init(SEventLoop* event_loop) = 0;
    virtual int Add(SEventLoop* event_loop, SChannel* channel) = 0;
    virtual int Del(SEventLoop* event_loop, SChannel* channel) = 0;
    virtual int Update(SEventLoop* event_loop, SChannel* channel) = 0;
    virtual int Dispatch(SEventLoop* event_loop, timeval* time_val) = 0;
    virtual int Clear(SEventLoop* event_loop) = 0;
};