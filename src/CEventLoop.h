#include "SDefine.h"
#include "CChannelHandle.h"
#include "CEpollDispatcher.h"

class CEventLoop
{
private:
    SEventLoop *m_event_loop;
    CChannelHandle *m_channel_handle;
public:
    CEventLoop(/* args */);
    ~CEventLoop();
    void init(char *thread_name = "main thread");
    void routine();
};

