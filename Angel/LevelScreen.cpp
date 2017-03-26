#include "stdafx.h"
#include "LevelScreen.h"
#include "Infrastructure/Preferences.h"
#include "Gwen/Controls/WindowControl.h"

CLevelScreen::CLevelScreen(IEnumScreenStack* stack):
    CMenuScreen(stack, "Level", StringList{""})
{
    m_currentButton = 0;
}

void CLevelScreen::Start()
{
    m_buttons.clear();
    m_box = theUI.ShowLevelChoiceBox(m_buttons, m_header, shared_from_this());
   
    ChangeControl(true);
}

void CLevelScreen::Stop()
{
    static_cast<Gwen::Controls::WindowControl*>(m_box)->CloseButtonPressed();
}

void CLevelScreen::Update(float dt)
{
    CMenuScreen::Update(dt);
}

void CLevelScreen::MoveUp()
{}

void CLevelScreen::MoveDown()
{}

void CLevelScreen::Enter()
{
    thePrefs.SetInt("Settings", "Level", m_currentButton + 1);
    thePrefs.SavePreferences();
    m_stack->PopScreen();
    m_stack->PushScreen(EScreen::GAME_PLAY);
}

void CLevelScreen::MoveRight()
{
    ChangeControl(false);
    m_currentButton = (m_currentButton + 1) % m_buttons.size();
    ChangeControl(true);
}

void CLevelScreen::MoveLeft()
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
