#ifndef TIMER_H
#define TIMER_H
#include "Global.h"
#include <memory>


class Timer
{

    static std::shared_ptr<Timer> s_TimerInstance;

    float m_DeltaTime;
    float m_LastTime {0.0f};

public:
    Timer() {}
    
    inline static std::shared_ptr<Timer> getInstance()
    {
        return s_TimerInstance = (s_TimerInstance == nullptr)? std::make_shared<Timer>() : s_TimerInstance;
    } 

    ~Timer()
    {}

    void Tick();

    void Delay();

    float GetDeltaTime() { return m_DeltaTime; }
    float GetLastTime() { return m_LastTime; }
};





#endif /* TIMER_H */
