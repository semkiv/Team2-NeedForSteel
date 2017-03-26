#include "stdafx.h"
#include "HelpScreen.h"
#include "Infrastructure/World.h"
#include "../ClientGame/Common.h"

CHelpScreen::CHelpScreen(IEnumScreenStack* stack):
    CMenuScreen(stack, "", StringList{}),
    m_controls(std::make_shared<Actor>())
{
    m_controls->SetSprite("Resources/Images/controls.png");
    m_controls->SetSize(MathUtil::GetWorldDimensions());
}

void CHelpScreen::Start()
{
    theWorld.Add(m_controls.get(), static_cast<int>(ELayer::UI));
}

void CHelpScreen::Stop()
{
    theWorld.Remove(m_controls.get());
}

void CHelpScreen::Update(float dt)
{
    CMenuScreen::Update(dt);
}

void CHelpScreen::MoveUp()
{}

void CHelpScreen::MoveDown()
{}

void CHelpScreen::Enter()
{
    Back();
}
