#include "common.h"
#include "SDefine.h"

class CChannelHandle
{
private:
    ChannelMap m_channel_map;
public:
    CChannelHandle(/* args */);
    ~CChannelHandle();
    void GetChannelMap(ChannelMap &channels);
    void init(int fd, int events, eventCallBack writeCallBack, eventCallBack readCallBack, void *data);
};

