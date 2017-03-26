#include "stdafx.h"
#include "PauseScreen.h"

CPauseScreen::CPauseScreen(IEnumScreenStack* stack):
    CMenuScreen(stack, "Pause", StringList{"Continue","Options","Help","Menu"})
{}

void CPauseScreen::Update(float dt)
{
    CMenuScreen::Update(dt);
}

void CPauseScreen::Enter()
{
    switch (m_currentButton)
    {
    case 0:
        m_stack->PopScreen();
        break;
    case 1:
        m_stack->PushScreen(EScreen::OPTIONS);
        break;
    case 2:
        m_stack->PushScreen(EScreen::HELP);
        break;
    case 3:
        m_stack->PushScreen(EScreen::END);
        break;
    }
}

CPauseScreen::~CPauseScreen() = default;
