#include "SDefine.h"

class CEpollFunc
{
private:
public:
    CEpollFunc() = default;
    ~CEpollFunc() = default;
    static int Init(SEventLoop* event_loop);
    static int Add(SEventLoop* event_loop, SChannel* channel);
    static int Del(SEventLoop* event_loop, SChannel* channel);
    static int Update(SEventLoop* event_loop, SChannel* channel);
    static int Dispatch(SEventLoop* event_loop, timeval* time_val);
    static int Clear(SEventLoop* event_loop);
};
