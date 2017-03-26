#include "stdafx.h"
#include "Screen.h"
#include "Input/InputManager.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/WindowControl.h"
#include "Infrastructure/World.h"
#include <memory>

CMenuScreen::CMenuScreen(IEnumScreenStack* stack, String const& header, StringList const& labels) :
	CScreen(stack),
    m_header(header),
    m_labels(labels)
{
    m_colors[0] = { 168};
    m_colors[1] = { 169 };
    m_colors[2] = { 58 };
}

CMenuScreen::~CMenuScreen() = default;

void CMenuScreen::Start()
{
    m_buttons.clear();
    m_currentButton = 0;
    m_box = theUI.ShowChoiceBox(m_buttons, m_header, m_labels, shared_from_this());
    ChangeControl(true);
}

void CMenuScreen::OnPush()
{}

void CMenuScreen::OnPop()
{}

void CMenuScreen::Stop()
{
    static_cast<Gwen::Controls::WindowControl*>(m_box)->CloseButtonPressed();
}

void CMenuScreen::MoveUp()
{
    ChangeControl(false);
    if (!m_currentButton)
    {
        m_currentButton = m_buttons.size() - 1;
    }
    else
    {
        --m_currentButton;
    }
    ChangeControl(true);
}

void CMenuScreen::MoveDown()
{
    ChangeControl(false);
    m_currentButton = (m_currentButton + 1) % m_buttons.size();
    ChangeControl(true);
}

void CMenuScreen::Enter()
{}

void CMenuScreen::MoveRight()
{}

void CMenuScreen::MoveLeft()
{}

void CMenuScreen::Back()
{
    m_stack->PopScreen();
}

void CMenuScreen::ChangeControl(bool ok)
{
    if (!ok)
    {
        static_cast<Gwen::Controls::Label*>(m_buttons[m_currentButton])->SetTextColor({ Gwen::Colors::White });
    }
    else
    {
        static_cast<Gwen::Controls::Label*>(m_buttons[m_currentButton])->SetTextColor({ m_colors[0],m_colors[1],m_colors[2] });
    }
}

void CMenuScreen::Update(float dt)
{
	if (theInput.IsButtonPressed("Up"))
	{
		MoveUp();
	}
	else if (theInput.IsButtonPressed("Down"))
	{
		MoveDown();
	}
	else if (theInput.IsButtonPressed("Enter"))
	{
		Enter();
	}
    else if (theInput.IsButtonPressed("Right"))
    {
        MoveRight();
    }
    else if (theInput.IsButtonPressed("Left"))
    {
        MoveLeft();
    }
    else if (theInput.IsButtonPressed("Back"))
    {
        Back();
    }
}

void CMenuScreen::operator()(size_t option)
{
}

void CMenuScreen::operator()()
{
}

CScreen::CScreen(IEnumScreenStack* stack):
	m_stack(stack)
{}

void CScreen::SetScreenStack(IEnumScreenStack* stack)
{
    m_stack = stack;
}

void CScreenStack::PushScreen(ScreenPtr screen)
{
	if (!m_screensStack.empty())
	{
		m_screensStack.back()->Stop();
	}
	screen->OnPush();
	m_screensStack.push_back(screen);
	m_screensStack.back()->Start();
}

void CScreenStack::PopScreen()
{
	if (!m_screensStack.empty())
	{
		auto back = m_screensStack.back();
		back->Stop();
		m_screensStack.pop_back();
		back->OnPop();

		if (!m_screensStack.empty())
		{
			m_screensStack.back()->Start();
		}
	}	
}

CScreenStack::ScreenPtr CScreenStack::GetTop()
{
	return m_screensStack.back();
}

bool CScreenStack::Empty() const
{
    return m_screensStack.empty();
}
