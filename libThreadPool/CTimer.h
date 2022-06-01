#include <thread>
#include <functional>
#include <atomic>
using callback = std::function<void()>;
class CTimer
{
private:
    static CTimer* m_instance;
    callback m_callback;
    std::thread m_thread;
    std::atomic_int m_status;
public:
    CTimer(/* args */);
    ~CTimer();
    static CTimer* GetInstance();
    void start(callback timerCallback);
    void stop();
    void work();
    void workPerSecond();
};
