#include "stdafx.h"
#include "Timer.h"

CTimer::CTimer(float time) :
    m_time(time)
{
}

void CTimer::UpdateTimer(float dt)
{
    m_time -= dt;

    if (m_time <= 0.0f)
    {
        CallBack();
    }
}

bool CTimer::Done() const
{
    return m_time <= 0.0f;
}
