#include "SDefine.h"
#include "CEpollFunc.h"
#include "CChannelHandle.h"
class CEventLoop
{
private:
    SEventLoop *m_event_loop;
    CEpollFunc *m_event_funcs;
    CChannelHandle *m_channel_handle;
public:
    CEventLoop(/* args */);
    ~CEventLoop();
    void init(char *thread_name = "main thread");
    void routine();
};

