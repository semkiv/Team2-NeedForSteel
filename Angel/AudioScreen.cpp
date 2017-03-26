#include "stdafx.h"
#include "AudioScreen.h"
#include "../Angel/Infrastructure/World.h"
#include "Gwen/Controls/CheckBox.h"
#include "Infrastructure/Preferences.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/WindowControl.h"
#include "../ClientGame/SoundManager.h"

CAudioScreen::CAudioScreen(IEnumScreenStack* stack): 
    CMenuScreen(stack, "Audio", StringList{""})
{}

void CAudioScreen::Start()
{
    m_buttons.clear();
    m_currentButton = 0;
    m_box = theUI.ShowAudioChoiceBox(m_buttons, m_header, shared_from_this());
}

void CAudioScreen::Stop()
{
    thePrefs.SetInt("Settings", "SoundOn",static_cast<Gwen::Controls::CheckBoxWithLabel*>(m_buttons[0])->Checkbox()->IsChecked());
    thePrefs.SetFloat("Settings", "SoundVolume", static_cast<Gwen::Controls::HorizontalSlider*>(m_buttons[1])->GetFloatValue());
    thePrefs.SavePreferences();
    static_cast<Gwen::Controls::WindowControl*>(m_box)->CloseButtonPressed();
}

void CAudioScreen::Update(float dt)
{
	CMenuScreen::Update(dt);
}

void CAudioScreen::MoveUp()
{}

void CAudioScreen::MoveDown()
{}

void CAudioScreen::Enter()
{
    Gwen::Controls::CheckBoxWithLabel* checkBox = static_cast<Gwen::Controls::CheckBoxWithLabel*>(m_buttons[0]);
    bool isSoundOn = checkBox->Checkbox()->IsChecked();
    if (!isSoundOn)
    {
        checkBox->Label()->SetText("Sound On");
        checkBox->Checkbox()->SetChecked(true);

    }
    else
    {
        checkBox->Label()->SetText("Sound Off");
        checkBox->Checkbox()->SetChecked(false);
    }
    theSoundManager.SetState(!isSoundOn);
}

void CAudioScreen::MoveRight()
{
    Gwen::Controls::HorizontalSlider* slider = static_cast<Gwen::Controls::HorizontalSlider*>(m_buttons[1]);
    slider->SetFloatValue(MathUtil::Min(slider->GetFloatValue() + 0.1, 1.0));
    theSoundManager.UpdateVolume(static_cast<Gwen::Controls::HorizontalSlider*>(m_buttons[1])->GetFloatValue());
}

void CAudioScreen::MoveLeft()
{
    Gwen::Controls::HorizontalSlider* slider = static_cast<Gwen::Controls::HorizontalSlider*>(m_buttons[1]);
    slider->SetFloatValue(MathUtil::Max(slider->GetFloatValue() - 0.1, 0.0));
    theSoundManager.UpdateVolume(static_cast<Gwen::Controls::HorizontalSlider*>(m_buttons[1])->GetFloatValue());
}
