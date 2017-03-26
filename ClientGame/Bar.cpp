#include "stdafx.h"
#include "Bar.h"

CBar::CBar(Vector2 size)
{
    m_current.SetSize(size);
    m_background.SetSize(size);
}

void CBar::Show()
{
    theWorld.Add(&m_background, static_cast<int>(ELayer::GAME_UI));
    theWorld.Add(&m_current, static_cast<int>(ELayer::GAME_UI));
}

void CBar::Hide()
{
    theWorld.Remove(&m_current);
    theWorld.Remove(&m_background);
}

void CBar::SetValue(float ratio)
{
    m_current.SetSize(m_background.GetSize().X * ratio, m_current.GetSize().Y);
    float offset = (m_background.GetSize().X  - m_current.GetSize().X) * 0.5f;
    m_current.SetPosition( m_current.GetPosition().X - offset, m_current.GetPosition().Y );
}

void CBar::SetPosition(Vector2 position)
{
    m_current.SetPosition(position);
    m_background.SetPosition(position);
}

Vector2 CBar::GetPosition() const
{
    return m_background.GetPosition();
}
        
void CBar::SetColor(Color color)
{
    m_current.SetColor(color);
}

void CBar::SetBackgroundColor(Color color)
{
    m_background.SetColor(color);
}

Vector2 CBar::GetSize() const
{
    return m_background.GetSize();
}
