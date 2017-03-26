#include "stdafx.h"
#include "ControlScreen.h"
#include "../Angel/Infrastructure/World.h"
#include "Infrastructure/Preferences.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/CheckBox.h"
#include "Gwen/Controls/Label.h"
#include "Input/InputManager.h"

CControlScreen::CControlScreen(IEnumScreenStack* stack) :
	CMenuScreen(stack, "Control", StringList{""})
{
    m_controls = { "Keyboard","Gamepad1","Gamepad2" };
    m_currentButton1 = 0;
    m_currentButton2 = 0;
}

void CControlScreen::Start()
{
    m_buttons.clear();
    m_currentButton = 0;
    for (size_t i = 0;i < m_controls.size();++i)
    {
        if (m_controls[i] == thePrefs.GetString("Settings", "Player1Control"))
        {
            m_currentButton1 = i;
            break;
        }
    }
    
   for (size_t i = 0;i < m_controls.size();++i)
    {
        if (m_controls[i] == thePrefs.GetString("Settings", "Player2Control"))
        {
            m_currentButton2 = i;
            break;
        }
    }
    m_box = theUI.ShowControlChoiceBox(m_buttons, m_header, shared_from_this());
    ChangeControl(true);
}

void CControlScreen::Stop()
{
    thePrefs.SetInt("Settings", "KeyboardInverted", static_cast<Gwen::Controls::CheckBoxWithLabel*>(m_buttons[0])->Checkbox()->IsChecked());
    thePrefs.SetInt("Settings", "GamepadInverted", static_cast<Gwen::Controls::CheckBoxWithLabel*>(m_buttons[1])->Checkbox()->IsChecked());
    thePrefs.SetString("Settings", "Player1Control", static_cast<Gwen::Controls::Label*>(m_buttons[2])->GetText().c_str());
    thePrefs.SetString("Settings", "Player2Control", static_cast<Gwen::Controls::Label*>(m_buttons[3])->GetText().c_str());
    thePrefs.SavePreferences();

    theInput.BindPlayersInput();
    
    CMenuScreen::Stop();
}

void CControlScreen::Update(float dt)
{
	CMenuScreen::Update(dt);
}

void CControlScreen::MoveRight()
{
    switch (m_currentButton)
    {
    case 2:
        m_currentButton1 = (m_currentButton1 + 1) % m_controls.size();
        static_cast<Gwen::Controls::Label*>(m_buttons[m_currentButton])->SetText(m_controls[m_currentButton1]);
        break;

    case 3:
        m_currentButton2 = (m_currentButton2 + 1) % m_controls.size();
        static_cast<Gwen::Controls::Label*>(m_buttons[m_currentButton])->SetText(m_controls[m_currentButton2]);
        break;
    }
}

void CControlScreen::MoveLeft()
{
    switch (m_currentButton)
    {
    case 2:
        if (!m_currentButton1)
        {
            m_currentButton1 = m_controls.size() - 1;
        }
        else
        {
            --m_currentButton1;
        }
        static_cast<Gwen::Controls::Label*>(m_buttons[m_currentButton])->SetText(m_controls[m_currentButton1]);
        break;

    case 3:
        if (!m_currentButton2)
        {
            m_currentButton2 = m_controls.size() - 1;
        }
        else
        {
            --m_currentButton2;
        }
        static_cast<Gwen::Controls::Label*>(m_buttons[m_currentButton])->SetText(m_controls[m_currentButton2]);
        break;
    }
}

void CControlScreen::Enter()
{
    switch (m_currentButton)
    {
        bool isInverted;
    case 0:
    {
        Gwen::Controls::CheckBoxWithLabel* keyboardBox = static_cast<Gwen::Controls::CheckBoxWithLabel*>(m_buttons[m_currentButton]);
        isInverted = keyboardBox->Checkbox()->IsChecked();
        if (isInverted)
        {
            keyboardBox->Checkbox()->SetChecked(false);
        }
        else
        {
            keyboardBox->Checkbox()->SetChecked(true);
        }
        break;
    }

    case 1:
    {
        Gwen::Controls::CheckBoxWithLabel* gamepadBox = static_cast<Gwen::Controls::CheckBoxWithLabel*>(m_buttons[m_currentButton]);
        isInverted = gamepadBox->Checkbox()->IsChecked();
        if (isInverted)
        {
            gamepadBox->Checkbox()->SetChecked(false);
        }
        else
        {
            gamepadBox->Checkbox()->SetChecked(true);
        }
        break;
    }
    }
}

void CControlScreen::ChangeControl(bool ok)
{
    if (!ok)
    {
        if (m_currentButton < 2)
        {
            Gwen::Controls::Label* label = static_cast<Gwen::Controls::CheckBoxWithLabel*>(m_buttons[m_currentButton])->Label();
            label->SetTextColor(Gwen::Colors::White);
        }
        else
        {
            static_cast<Gwen::Controls::Label*>(m_buttons[m_currentButton])->SetTextColor(Gwen::Colors::White);
        }
    }
    else
    {
        if (m_currentButton < 2)
        {
            Gwen::Controls::Label* label = static_cast<Gwen::Controls::CheckBoxWithLabel*>(m_buttons[m_currentButton])->Label();
            label->SetTextColor({ m_colors[0],m_colors[1],m_colors[2] });
        }
        else
        {
            static_cast<Gwen::Controls::Label*>(m_buttons[m_currentButton])->SetTextColor({ m_colors[0],m_colors[1],m_colors[2] });
        }
    }
}

