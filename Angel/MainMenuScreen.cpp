#include "stdafx.h"
#include "MainMenuScreen.h"

CMainMenuScreen::CMainMenuScreen(IEnumScreenStack* stack) :
 	CMenuScreen(stack, "Main Menu", StringList{ "Play" ,"Options" ,"Help","Exit" })
{}

void CMainMenuScreen::Update(float dt)
{
	CMenuScreen::Update(dt);
}

void CMainMenuScreen::Enter()
{
	switch (m_currentButton)
	{
	case 0:
		m_stack->PushScreen(EScreen::LEVEL);
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

void CMainMenuScreen::Back()
{}
