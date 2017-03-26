#include "stdafx.h"
#include "OptionsScreen.h"

COptionsScreen::COptionsScreen(IEnumScreenStack* stack) :
	CMenuScreen(stack, "Options", StringList{ "Control", "Audio"})
{}

void COptionsScreen::Update(float dt)
{
	CMenuScreen::Update(dt);
}

void COptionsScreen::Enter()
{
	switch (m_currentButton)
	{
	case 0:
		m_stack->PushScreen(EScreen::CONTROL);
		break;
	case 1:
		m_stack->PushScreen(EScreen::AUDIO);
		break;
	}
}
