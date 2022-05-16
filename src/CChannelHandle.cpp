#include "CChannelHandle.h"
CChannelHandle::CChannelHandle(/* args */)
{
}

CChannelHandle::~CChannelHandle()
{
    for (auto channel : m_channel_map)
    {
        delete channel.second;
    }
    
}
void CChannelHandle::GetChannelMap(ChannelMap &channels)
{
    channels = m_channel_map;
}

void CChannelHandle::init(int fd, int events, eventCallBack writeCallBack, eventCallBack readCallBack, void *data)
{
    SChannel *channel = new SChannel(fd, events, writeCallBack, readCallBack, data);
    if (m_channel_map.find(fd) == m_channel_map.end())
    {
        m_channel_map.emplace(fd, channel);
    }
    else
    {
        m_channel_map[fd] = channel;
    }
    
}
