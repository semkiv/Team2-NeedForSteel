#include "stdafx.h"
#include "VictoryBox.h"

CVictoryBox::CVictoryBox(Vector2 position) :
    m_isVisible(false),
    m_winner(std::make_shared<Actor>())
{
    m_winner->SetSize(MathUtil::GetWorldDimensions());
    m_winner->SetPosition(position);
}

void CVictoryBox::Show(size_t winner)
{
    m_isVisible = true;
    m_winner->SetSprite(winner == 1 ? "Resources/Images/victory_screen.png" : "Resources/Images/victory_screen2.png");
    theWorld.Add(m_winner.get(), static_cast<int>(ELayer::UI));
}

void CVictoryBox::Hide()
{
    m_isVisible = false;
    theWorld.Remove(m_winner.get());
}

bool CVictoryBox::IsVisible()
{
    return m_isVisible;
}
