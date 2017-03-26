//////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2008-2013, Shane Liesegang
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of any
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../UI/UserInterface.h"
#include "../Infrastructure/Preferences.h"

#include "Gwen/Gwen.h"
#include "Gwen/Controls/Canvas.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/GroupBox.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/Slider.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/CheckBox.h"
#include "Gwen/Controls/Label.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Renderers/OpenGL.h"


#if defined(__APPLE__)
	// included files from gwen trigger Clang/GCC warnings
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdangling-else"
	#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
		#include "Gwen/Skins/TexturedBase.h"
#if defined(__APPLE__)
	#pragma GCC diagnostic pop
#endif

#include "../UI/GwenRenderer.h"
#include "../Infrastructure/Log.h"
#include "../Infrastructure/Camera.h"
#include "../Infrastructure/World.h"

typedef std::map<AngelUIHandle, std::shared_ptr<ICallBack>> ButtonMapping;
typedef std::map<AngelUIHandle, std::shared_ptr<ICallBack>> ChoiceMapping;

class EventHandler : public Gwen::Event::Handler
{
public:
	void AddButtonCallback(Gwen::Controls::Base* control, std::shared_ptr<ICallBack> callback)
	{
		_buttonCallbacks[control] = callback;
	}
	
	void AddChoiceCallback(Gwen::Controls::Base* control, std::shared_ptr<ICallBack> callback)
	{
		_choiceCallbacks[control] = callback;
	}
	
	
	void OnPress(Gwen::Controls::Base* control)
	{
		Check(control);
	}
	
private:
	void Check(Gwen::Controls::Base* control)
	{
		ButtonMapping::iterator buttonIt = _buttonCallbacks.find(control);
		if (buttonIt != _buttonCallbacks.end())
		{
            if (buttonIt->second != NULL)
            {
                buttonIt->second->operator()();
            }
			return;
		}

		ChoiceMapping::iterator choiceIt = _choiceCallbacks.find(control->GetParent());
		if (choiceIt != _choiceCallbacks.end())
		{
			int button = -1;
			Gwen::Controls::Base::List buttons = ((Gwen::Controls::Base*)choiceIt->first)->GetChildren();
			Gwen::Controls::Base::List::iterator butIt = buttons.begin();
			int count = 0;
			while (butIt != buttons.end())
			{
				if (*butIt == control)
				{
					button = count;
					break;
				}
				butIt++;
				count++;
			}
			
            if (choiceIt->second != NULL)
            {
                choiceIt->second->operator()(button);
            }
			theUI.RemoveUIElement(choiceIt->first);
			return;
		}
	}
	
	ButtonMapping _buttonCallbacks;
	ChoiceMapping _choiceCallbacks;
};


namespace 
{
	Gwen::Controls::Canvas* AngelCanvas;
	Gwen::Skin::Base* AngelSkin;
	
	EventHandler handler;
}

UserInterface* UserInterface::s_UserInterface = NULL;

UserInterface& UserInterface::GetInstance()
{
	if (s_UserInterface == NULL)
	{
		s_UserInterface = new UserInterface();
	}
	return *s_UserInterface;
}

UserInterface::UserInterface()
{
	#if !ANGEL_MOBILE
		double mpos_x, mpos_y;
		glfwGetCursorPos(theWorld.GetMainWindow(), &mpos_x, &mpos_y);
		_mousePosition.X = (float)mpos_x;
		_mousePosition.Y = (float)mpos_y;
	#else
		_mousePosition.X = _mousePosition.Y = 0;
	#endif

	const String skinTexture = "Resources/Images/DefaultSkin.png";

	_renderer = new GwenRenderer(skinTexture);

	AngelSkin = new Gwen::Skin::TexturedBase(_renderer);
	((Gwen::Skin::TexturedBase*)AngelSkin)->Init(skinTexture);	
    AngelSkin->SetDefaultFont(Gwen::Utility::StringToUnicode("Resources/Fonts/BlackOpsOne-Regular.ttf"), 20);
	_renderer->FinishInit();

	AngelCanvas = new Gwen::Controls::Canvas(AngelSkin);
	AngelCanvas->SetSize(theCamera.GetWindowWidth(), theCamera.GetWindowHeight()); // should be size of window (update when change)

	_locked = false;
}

UserInterface::~UserInterface()
{
	
}

void UserInterface::SetDefaultFont(std::string const& font, float size) 
{
    _renderer->Init();

    AngelSkin->SetDefaultFont(Gwen::Utility::StringToUnicode(font.c_str()), size);

    _renderer->FinishInit();

    m_defaultFont = font;
    m_defaultFontSize = size;
}

void UserInterface::SetDefaultColor(Color color)
{
    m_defaultColor = color;
}

Color UserInterface::GetDefaultColor() const
{
    return m_defaultColor;
}

std::string UserInterface::GetDefaultFont() const
{
    return m_defaultFont;
}

float UserInterface::GetDefaultFontSize() const
{
    return m_defaultFontSize;
}

void UserInterface::Shutdown()
{
	delete AngelCanvas;
	delete AngelSkin;
	
	delete _renderer;
}

void UserInterface::Render()
{
	for (unsigned int i=0; i < _deferredRemoves.size(); i++)
	{
		RemoveUIElement(_deferredRemoves[i]);
	}
	_deferredRemoves.clear();
	
	_locked = true;
		AngelCanvas->DoThink();
		AngelCanvas->RenderCanvas();
	_locked = false;
}

#if ANGEL_MOBILE
    void UserInterface::TouchMotionEvent(Touch* movedTouch)
    {
        Vec2i translatedPoint(movedTouch->CurrentPoint.X * 2, movedTouch->CurrentPoint.Y * 2);
        int deltaX = _mousePosition.X - translatedPoint.X;
        int deltaY = _mousePosition.Y - translatedPoint.Y;
        _mousePosition.X = translatedPoint.X;
        _mousePosition.Y = translatedPoint.Y;
        AngelCanvas->InputMouseMoved(translatedPoint.X, translatedPoint.Y, deltaX, deltaY);
    }

    void UserInterface::TouchEndEvent(Touch* endedTouch)
    {
        _locked = true;
            TouchMotionEvent(endedTouch);
            AngelCanvas->InputMouseButton(0, false); // 0 = left button
        _locked = false;
    }

    void UserInterface::TouchStartEvent(Touch* startedTouch)
    {
        _locked = true;
            TouchMotionEvent(startedTouch);
            AngelCanvas->InputMouseButton(0, true); // 0 = left button
        _locked = false;
    }

#else //ANGEL_MOBILE
    void UserInterface::MouseMotionEvent(Vec2i screenCoordinates)
    {
        if (theWorld.GetConsole()->IsEnabled())
        {
            return;
        }
        
        _locked = true;
            int deltaX = _mousePosition.X - screenCoordinates.X;
            int deltaY = _mousePosition.Y - screenCoordinates.Y;
            _mousePosition.X = screenCoordinates.X;
            _mousePosition.Y = screenCoordinates.Y;
            AngelCanvas->InputMouseMoved(screenCoordinates.X, screenCoordinates.Y, deltaX, deltaY);
        _locked = false;
    }

    void UserInterface::MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button)
    {
        if (theWorld.GetConsole()->IsEnabled())
        {
            return;
        }
        
        _locked = true;
            AngelCanvas->InputMouseButton(button, true);
        _locked = false;
    }

    void UserInterface::MouseUpEvent(Vec2i screenCoordinates, MouseButtonInput button)
    {
        if (theWorld.GetConsole()->IsEnabled())
        {
            return;
        }
        
        _locked = true;
            AngelCanvas->InputMouseButton(button, false);
        _locked = false;
    }

    void UserInterface::MouseWheelEvent(const Vector2& position)
    {
        if (theWorld.GetConsole()->IsEnabled())
        {
            return;
        }
        
        _locked = true;
            AngelCanvas->InputMouseWheel(position.Y);
        _locked = false;
    }
#endif //ANGEL_MOBILE

void UserInterface::RemoveUIElement(AngelUIHandle element)
{
	if (_locked)
	{
		_deferredRemoves.push_back(element);
		return;
	}
	std::set<AngelUIHandle>::iterator it = _elements.find(element);
	if (it != _elements.end())
	{
		delete (Gwen::Controls::Base*)(*it);
		_elements.erase(it);
	}
}

AngelUIHandle UserInterface::AddButton(const String& label, Vec2i position, std::shared_ptr<ICallBack> callback, bool center, const String& font, Vec2i padding)
{
	Gwen::Controls::Button* button = new Gwen::Controls::Button(AngelCanvas);
	
	if (font != "")
	{
		button->SetFont(Gwen::Utility::StringToUnicode(font), 20, false);
	}
	button->SetText(label);
	button->SetPadding(Gwen::Padding(padding.X, padding.Y, padding.X, padding.Y));
	button->SizeToContents();
	if (center)
	{
		Gwen::Point size = button->GetSize();
		button->SetPos(position.X - (size.x / 2), position.Y - (size.y / 2));
	}
	else
	{
		button->SetPos(position.X, position.Y);
	}

	button->onPress.Add(&handler, &EventHandler::OnPress);
	handler.AddButtonCallback(button, callback);
	
	_elements.insert(button);
	
	return button;
}


AngelUIHandle UserInterface::ShowChoiceBox(std::vector<AngelUIHandle>& buttons, const String& choiceLabel, const StringList& labels, std::shared_ptr<ICallBack> callback, const String& font, Vec2i padding, bool modal)
{
    Gwen::Controls::WindowControl* box = new Gwen::Controls::WindowControl(AngelCanvas);
    box->SetTitle(choiceLabel);

    Gwen::TextObject fontTextObject = Gwen::Utility::StringToUnicode(font);
    Gwen::Point maxSize = { 300,80 };

    for (unsigned int i = 0; i < labels.size(); ++i)
    {
        Gwen::Controls::Button* button = new Gwen::Controls::Button(box);

        buttons.emplace_back(button);
        if (font != "")
        {
           button->SetFont(fontTextObject, 20, false);
        }
        button->SetText(labels[i]);
        button->SetPadding(Gwen::Padding(padding.X, padding.Y, padding.X, padding.Y));
        button->SetSize(maxSize);

        button->onPress.Add(&handler, &EventHandler::OnPress);
    }

    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        static_cast<Gwen::Controls::Button*>(buttons[i])->SetPos(maxSize.x / 2 - static_cast<Gwen::Controls::Button*>(buttons[i])->GetSize().x/2.05 , i * (maxSize.y + padding.Y));

    }

    box->SetSize(maxSize.x + (padding.X * 2), buttons.size() * (maxSize.y + padding.Y) + (padding.Y * 3));
    box->SetPos((AngelCanvas->GetSize().x / 2) - (box->GetSize().x / 2), (AngelCanvas->GetSize().y / 2) - (box->GetSize().y / 2));

    if (modal)
    {
        box->MakeModal();
    }
    box->SetDeleteOnClose(true);
    box->SetClosable(false);

    handler.AddChoiceCallback(box, callback);

    _elements.insert(box);

    return box;
}

AngelUIHandle UserInterface::ShowAudioChoiceBox(std::vector<AngelUIHandle>& buttons, const String& choiceLabel, std::shared_ptr<ICallBack> callback)
{
    Gwen::Controls::WindowControl* box = new Gwen::Controls::WindowControl(AngelCanvas);
    box->SetTitle(choiceLabel);

    Gwen::Point maxSize = { 150,25 };
    Gwen::Controls::CheckBoxWithLabel* checkBox = new Gwen::Controls::CheckBoxWithLabel(box);
    buttons.emplace_back(checkBox);
    checkBox->SetSize(maxSize);
    checkBox->SetPos(maxSize.x / 2 - checkBox->GetSize().x / 2.15, 0);
    
    int d = thePrefs.GetInt("Settings", "SoundOn");
    if (d == 1)
    {
        checkBox->Label()->SetText("Sound On");
        checkBox->Checkbox()->SetChecked(true);
    }
    else
    {
       checkBox->Label()->SetText("Sound Off");
       checkBox->Checkbox()->SetChecked(false);
    }

    Gwen::Controls::HorizontalSlider* slider = new Gwen::Controls::HorizontalSlider(box);
    buttons.emplace_back(slider);
    slider->SetSize(maxSize);
    slider->SetPos(maxSize.x / 2 - slider->GetSize().x / 2.15, maxSize.y + 10);
    
    float volume = thePrefs.GetFloat("Settings", "SoundVolume");
    slider->SetFloatValue(volume);

    box->SetSize(maxSize.x + (20), buttons.size() * (maxSize.y + 10) + (30));
    box->SetPos((AngelCanvas->GetSize().x / 2) - (box->GetSize().x / 2), (AngelCanvas->GetSize().y / 2) - (box->GetSize().y / 2));
    box->MakeModal();
    box->SetDeleteOnClose(true);
    box->SetClosable(false);

    handler.AddChoiceCallback(box, callback);

    _elements.insert(box);
    return box;
}

AngelUIHandle UserInterface::ShowControlChoiceBox(std::vector<AngelUIHandle>& buttons, const String & choiceLabel, std::shared_ptr<ICallBack> callback)
{
    Gwen::Controls::WindowControl* box = new Gwen::Controls::WindowControl(AngelCanvas);
    box->SetTitle(choiceLabel);

    Gwen::Point maxSize = { 250,25 };
    Gwen::Controls::CheckBoxWithLabel* keyboardBox = new Gwen::Controls::CheckBoxWithLabel(box);
    buttons.emplace_back(keyboardBox);
    keyboardBox->SetSize(maxSize);
    keyboardBox->SetPos(maxSize.x / 2 - keyboardBox->GetSize().x / 2.15, 0);
    keyboardBox->Label()->SetText("KeyboardInverted");

    int d = thePrefs.GetInt("Settings", "KeyboardInverted");
    if (d == 1)
    {
        keyboardBox->Checkbox()->SetChecked(true);
    }
    else
    {
        keyboardBox->Checkbox()->SetChecked(false);
    }

    Gwen::Controls::CheckBoxWithLabel* gamepadBox = new Gwen::Controls::CheckBoxWithLabel(box);
    buttons.emplace_back(gamepadBox);
    gamepadBox->SetSize(maxSize);
    gamepadBox->SetPos(maxSize.x / 2 - gamepadBox->GetSize().x / 2.15, maxSize.y + 10);
    gamepadBox->Label()->SetText("GamepadInverted");

    d = thePrefs.GetInt("Settings", "GamepadInverted");
    if (d == 1)
    {
        gamepadBox->Checkbox()->SetChecked(true);
    }
    else
    {
        gamepadBox->Checkbox()->SetChecked(false);
    }

    Gwen::Controls::Label* player1 = new Gwen::Controls::Label(box);
    player1->SetSize(maxSize);
    player1->SetPos(maxSize.x / 2 - player1->GetSize().x / 2.15, 2 * (maxSize.y + 10));

    Gwen::Controls::Label* player1Control = new Gwen::Controls::Label(box);
    buttons.emplace_back(player1Control);
    player1Control->SetSize(maxSize.x,maxSize.y);
    player1->SetText("Player 1");
    player1Control->SetPos(player1->GetSize().x / 2 + (maxSize.x / 2 - player1Control->GetSize().x / 2.15), 2 * (maxSize.y + 10));

    String str = thePrefs.GetString("Settings", "Player1Control");
    player1Control->SetText(str);

    Gwen::Controls::Label* player2 = new Gwen::Controls::Label(box);
    player2->SetSize(maxSize);
    player2->SetText("Player 2");
    player2->SetPos(maxSize.x / 2 - player2->GetSize().x / 2.15, 3 * (maxSize.y + 10));

    Gwen::Controls::Label* player2Control = new Gwen::Controls::Label(box);
    buttons.emplace_back(player2Control);
    player2Control->SetSize(maxSize);
    player2Control->SetPos(player2->GetSize().x / 2 + (maxSize.x / 2 - player2Control->GetSize().x / 2.15), 3 * (maxSize.y + 10));

    str = thePrefs.GetString("Settings", "Player2Control");
    player2Control->SetText(str);

    box->SetSize(maxSize.x + (20), buttons.size() * (maxSize.y + 10) + (30));
    box->SetPos((AngelCanvas->GetSize().x / 2) - (box->GetSize().x / 2), (AngelCanvas->GetSize().y / 2) - (box->GetSize().y / 2));
    box->MakeModal();
    box->SetDeleteOnClose(true);
    box->SetClosable(false);

    _elements.insert(box);
    return box;
}

AngelUIHandle UserInterface::ShowLevelChoiceBox(std::vector<AngelUIHandle>& buttons, const String& choiceLabel, std::shared_ptr<ICallBack> callback)
{
    Gwen::Controls::WindowControl* box = new Gwen::Controls::WindowControl(AngelCanvas);
    box->SetTitle("          "+choiceLabel);

    Gwen::Point maxSize = { 200,50 };
    Gwen::Controls::Label* level1 = new Gwen::Controls::Label(box);
    buttons.emplace_back(level1);
    level1->SetSize(maxSize);
    level1->SetPos(maxSize.x / 2 - level1->GetSize().x/3, 0);
    level1->SetText("1");
   
    Gwen::Controls::Label* level2 = new Gwen::Controls::Label(box);
    buttons.emplace_back(level2);
    level2->SetSize(maxSize);
    level2->SetPos(maxSize.x / 2 + level2->GetSize().x/5, 0);
    level2->SetText("2");

    box->SetSize(maxSize.x, buttons.size() * (maxSize.y) );
    box->SetPos((AngelCanvas->GetSize().x / 2) - (box->GetSize().x / 2), (AngelCanvas->GetSize().y / 2) - (box->GetSize().y) / 2);
    box->MakeModal();
    box->SetDeleteOnClose(true);
    box->SetClosable(false);

    _elements.insert(box);
    return box;
}


