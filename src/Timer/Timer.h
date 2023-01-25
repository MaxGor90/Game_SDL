#ifndef TIMER_H
#define TIMER_H
#include "Global.h"
#include <memory>


class Timer
{
    Timer() {}
    ~Timer() {}

    float m_DeltaTime;
    float m_LastTime {0.0f};

public:
    
    inline static Timer& getInstance()
    {
        static Timer TimerInstance;
        return TimerInstance;
    } 

    void Tick();

    void Delay();

    float GetDeltaTime() { return m_DeltaTime; }
    float GetLastTime() { return m_LastTime; }
};





#endif /* TIMER_H */
