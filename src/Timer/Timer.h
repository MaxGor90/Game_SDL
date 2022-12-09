#ifndef TIMER_H
#define TIMER_H
#include "Global.h"


class Timer
{
    Timer() {}

    static Timer* s_TimerInstance;

    float m_DeltaTime;
    float m_LastTime {0.0f};

public:

    inline static Timer* getInstance()
    {
        return s_TimerInstance = (s_TimerInstance == nullptr)? new Timer() : s_TimerInstance;
    } 

    ~Timer()
    {
        delete s_TimerInstance;
    }

    void Tick();

    void Delay();

    float GetDeltaTime() { return m_DeltaTime; }
    float GetLastTime() { return m_LastTime; }
};





#endif /* TIMER_H */
