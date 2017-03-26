#pragma once

class CTimer
{
public:
    CTimer(float time);
    
    void UpdateTimer(float dt);
    bool Done() const;

private:
    float m_time;

private:
    virtual void CallBack() = 0;    
};

template<typename Func>
class CCallbackTimer : public CTimer
{

public:
    CCallbackTimer(float time, Func func);

private:
    Func m_func;

private:
    virtual void CallBack() override;

};

template<typename Func>
CCallbackTimer<Func>::CCallbackTimer(float time, Func func) :
    CTimer(time),
    m_func(func)
{
}

template<typename Func>
void CCallbackTimer<Func>::CallBack()
{
    m_func();
}