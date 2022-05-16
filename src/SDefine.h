#include "common.h"
// #include "CDispatcher.h"

#define eventInit function<int(SEventLoop*)>
#define eventAdd function<int(SEventLoop*, SChannel*)>
#define eventDel function<int(SEventLoop*, SChannel*)>
#define eventUpdate function<int(SEventLoop*, SChannel*)>
#define eventDispatch function<int(SEventLoop*, timeval*)>
#define eventClear function<int(SEventLoop*)>

#define eventCallBack function<int(void*)>

//参考
#define EVENT_TIMEOUT    0x01
/** Wait for a socket or FD to become readable */
#define EVENT_READ       0x02
/** Wait for a socket or FD to become writeable */
#define EVENT_WRITE      0x04
/** Wait for a POSIX signal to be raised*/
#define EVENT_SIGNAL     0x08

struct SEventDispatcher
{
    const char* name;
    SEventLoop *event_loop;

    eventInit event_init;
    eventAdd event_add;
    eventDel event_del;
    eventUpdate event_update;
    eventDispatch event_dispatch;
    eventClear event_clear;
    SEventDispatcher(){};
    SEventDispatcher(
        eventInit init, 
        eventAdd add,
        eventDel del,
        eventUpdate update,
        eventDispatch dispatch,
        eventClear clear
    ):event_init(init),event_add(add),event_del(del),event_update(update),event_dispatch(dispatch),event_clear(clear){};

};
/*
struct SEventDispatcher
{
    const char* name;
    SEventLoop *event_loop;

    eventInit event_init;
    eventAdd event_add;
    eventDel event_del;
    eventUpdate event_update;
    eventDispatch event_dispatch;
    eventClear event_clear;
    // function<int(SEventLoop*)> eventInit;
    // function<int(SEventLoop*, SChannel*)> eventAdd;
    // function<int(SEventLoop*, SChannel*)> eventDel;
    // function<int(SEventLoop*, SChannel*)> eventUpdate;
    // function<int(SEventLoop*, timeval*)> eventDispatch;
    // function<int(SEventLoop*)> eventClear;

    SEventDispatcher(
        eventInit init, 
        eventAdd add,
        eventDel del,
        eventUpdate update,
        eventDispatch dispatch,
        eventClear clear
    ):event_init(init),event_add(add),event_del(del),event_update(update),event_dispatch(dispatch),event_clear(clear){}
    // init eventInit(SEventLoop* event_loop);
    // add eventAdd(SEventLoop* event_loop, SChannel* channel);
    // del eventDel(SEventLoop* event_loop, SChannel* channel);
    // update eventUpdate(SEventLoop* event_loop, SChannel* channel);
    // dispatch eventDispatch(SEventLoop* event_loop, timeval* time_val);
    // clear eventClear(SEventLoop* event_loop);  
};
*/
class CDispatcher;
struct SEventLoop
{
    // SEventDispatcher *dispatcher;
    CDispatcher *dispatcher;
    pthread_t owner_thread_id; 
    pthread_mutex_t mutex; 
    pthread_cond_t cond; 
    int socketPair[2]; 
    char *thread_name;
    int quit;
    void *event_dispatcher_data;
    ~SEventLoop(){};
};

struct SChannel
{
    int fd; 
    int events; //表示event类型 
    eventCallBack m_readCallback; 
    eventCallBack m_writeCallback; 
    void *data;
    SChannel(int new_fd, int new_events, eventCallBack readCallback, eventCallBack writeCallback, void *new_data):
        fd(new_fd), events(new_events), m_readCallback(readCallback), m_writeCallback(writeCallback), data(new_data){};
};

struct SEpollDispatcherData
{
    int event_count;
    int nfds;
    int realloc_copy;
    int event_fd;
    epoll_event *events;
};

using ChannelMap = map<int/*fd*/, SChannel*>;
