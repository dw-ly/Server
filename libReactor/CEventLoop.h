#include "SDefine.h"
#include "CChannelHandle.h"
#include "CDispatcher.h"
#include "CEpollDispatcher.h"

class CEventLoop
{
private:
    SEventLoop *m_event_loop;
    CChannelHandle *m_channel_handle;
public:
    CEventLoop(/* args */);
    ~CEventLoop();
    void init(const char *thread_name = "main thread");
    void routine();
    CChannelHandle *getChannelHandle();
    int addEventLoopHandle(int fd, SChannel *channel);
    int updateEventLoopHandle(int fd, SChannel *channel);
    int removeEventLoopHandle(int fd, SChannel *channel);

    int wakeUpEventLoop();
    int wakeUpHandle();
};

