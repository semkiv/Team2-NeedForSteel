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
#include "../Input/InputManager.h"

#include "../Infrastructure/World.h"
#include "../Input/Controller.h"
#include "../Messaging/Switchboard.h"
#include "../Util/StringUtil.h"
#include "../Util/FileUtil.h"
#include "../Infrastructure/Log.h"
#include "../Infrastructure/Preferences.h"

constexpr float g_stickSensitivity = 0.75f;

InputKeys XBOX_BUTTON(int controllerId, InputKeys key)
{
    return (InputKeys)((int)key + controllerId * XBOX_CONTROLLER_SHIFT);
}

InputKeys XBOX_LEFTSTICK_X(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_LEFTSTICK_X);
}

InputKeys XBOX_LEFTSTICK_Y(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_LEFTSTICK_Y);
}

InputKeys XBOX_LEFTSTICK_LEFT(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_LEFTSTICK_LEFT);
}

InputKeys XBOX_LEFTSTICK_RIGHT(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_LEFTSTICK_RIGHT);
}

InputKeys XBOX_LEFTSTICK_UP(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_LEFTSTICK_UP);
}

InputKeys XBOX_LEFTSTICK_DOWN(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_LEFTSTICK_DOWN);
}

InputKeys XBOX_RIGHTSTICK_LEFT(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_RIGHTSTICK_LEFT);
}

InputKeys XBOX_RIGHTSTICK_RIGHT(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_RIGHTSTICK_RIGHT);
}

InputKeys XBOX_RIGHTSTICK_UP(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_RIGHTSTICK_UP);
}

InputKeys XBOX_RIGHTSTICK_DOWN(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_RIGHTSTICK_DOWN);
}


InputKeys XBOX_RIGHTSTICK_X(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_RIGHTSTICK_X);
}

InputKeys XBOX_RIGHTSTICK_Y(int controllerId)
{
    return XBOX_BUTTON(controllerId, P1BUTTON_RIGHTSTICK_Y);
}

void InputBinding::SetKeyDownMessage( const String& keyDownMessage )
{
	_keyDownMessage = keyDownMessage;
}

void InputBinding::SetKeyUpMessage( const String& keyUpMessage )
{
	_keyUpMessage = keyUpMessage;
}

void InputBinding::OnKeyDown()
{
	if( _keyDownMessage.length() == 0 )
		return;
	
	theSwitchboard.Broadcast(new Message(_keyDownMessage));
}

void InputBinding::OnKeyUp()
{
	if( _keyUpMessage.length() == 0 )
		return;

	theSwitchboard.Broadcast(new Message(_keyUpMessage));
}

InputManager* InputManager::s_Input = NULL;

InputManager &InputManager::GetInstance()
{
	if( s_Input == NULL )
	{
		s_Input = new InputManager();
		s_Input->Initialize();
	}
	return *s_Input;
}

void InputManager::Destroy()
{
	if( s_Input != NULL )
	{
		delete s_Input;
		s_Input = NULL;
	}
}

void TestBinding( const String& input )
{
	sysLog.Log("BUTTON: " + input);
}

void InputManager::Initialize()
{
#define USEINPUTKEY(e,hashVal,text) \
	_keyNameTable[ToUpper(text)] = e;
#include "InputKeys.h"
#undef USEINPUTKEY

	//Clear Xbox Button States
	ClearXboxButtonStates();
	
	StringList bindings;
	GetLinesFromFile("Config/input_bindings.ini", bindings);
	StringList::iterator it = bindings.begin();
	while (it != bindings.end())
	{
		if (((*it).size() == 0) || ((*it)[0] == ';'))
		{
			it++;
			continue;
		}
		StringList splitBinding = SplitString(*it, "=:", false);
		if (splitBinding.size() >= 2)
		{
			splitBinding[0] = TrimString(splitBinding[0]);
			splitBinding[1] = TrimString(splitBinding[1]);
			BindKey(splitBinding[0], splitBinding[1]);
		}
		it++;
	}
}

InputManager::~InputManager()
{
	for( BindingTable::iterator itr = _bindingTable.begin(); itr != _bindingTable.end(); itr++ )
	{
		delete (*itr).second;
	}

	_bindingTable.clear();
}

void InputManager::BindKey( const String& keyId, const String& command )
{
	if( command.length() == 0 )
		return;

	int hashVal = GetHashFromKeyName( keyId );

	InputBinding* pBinding = GetBinding(hashVal);
	if( pBinding == NULL )
	{
		pBinding = new InputBinding();
		_bindingTable[hashVal] = pBinding;
	}

	String setCommand = command;
	if( setCommand[0] == '-' )
	{
		setCommand.erase(0,1);
		pBinding->SetKeyUpMessage( setCommand );
	}
	else
	{
		if( setCommand[0] == '+' )
			setCommand.erase(0,1);

		pBinding->SetKeyDownMessage( setCommand );
	}
}

void InputManager::UnbindKey( const String& keyId )
{
	int hashVal = GetHashFromKeyName( keyId );

	InputBinding* pBinding = GetBinding(hashVal);
	if( pBinding == NULL )
		return;

	delete pBinding;
	_bindingTable.erase( hashVal );
}

bool InputManager::OnKeyDown( int keyVal )
{
    CurrentKeyboardButtonStates()[keyVal] = true;

	InputBinding* pBinding = GetBinding( tolower(keyVal) );
	if( pBinding == NULL )
		return false;

	pBinding->OnKeyDown();

	return true;
}

bool InputManager::OnKeyUp( int keyVal )
{
    CurrentKeyboardButtonStates()[keyVal] = false;

	InputBinding* pBinding = GetBinding( tolower(keyVal) );
    if( pBinding == NULL )
		return false;

	pBinding->OnKeyUp();

	return true;
}

void InputManager::BindPlayerInput(unsigned int playerId, SPlayerInputBinding const& binding)
{
    assert(playerId <= MAX_PLAYER_COUNT);

    m_playerBindingTable[playerId] = binding;
    m_playerBindingTable[playerId].m_isBinding = true;
}

SPlayerInputBinding const& InputManager::GetPlayerInputBinding(size_t playerId) const
{
    return m_playerBindingTable[playerId];
}

void InputManager::UnbindPlayer(unsigned int playerId)
{
    assert(playerId <= MAX_PLAYER_COUNT);

    m_playerBindingTable[playerId].m_isBinding = false;
}

SControllerInfo const& InputManager::GetControllerInfo(EInputControllerType type, size_t controllerId) const
{
    switch (type)
    {
    case EInputControllerType::KEYBOARD:
        return m_keyboardInfo;
    case EInputControllerType::XBOX:
        return m_xboxControllerInfo[controllerId];
    }

    return m_keyboardInfo;
}

void InputManager::AddPlayerListener(unsigned int playerId, std::shared_ptr<IPlayerInputListener> listener)
{
    assert(playerId <= MAX_PLAYER_COUNT);

    m_playerListenerTable[playerId].push_back(listener);
}

void InputManager::ClearPlayerListeners(unsigned int playerId)
{
    assert(playerId <= MAX_PLAYER_COUNT);

    m_playerListenerTable[playerId].clear();
}

struct XboxButtonBindRecord
{
	int HashKey;
	const bool (Controller::*CheckFunc)();
};

XboxButtonBindRecord sBindRecords[MAX_CONTROLLERS][12] =
{
    {
        { P1BUTTON_A, &Controller::IsAButtonDown },
        { P1BUTTON_B, &Controller::IsBButtonDown },
        { P1BUTTON_X, &Controller::IsXButtonDown },
        { P1BUTTON_Y, &Controller::IsYButtonDown },

        { P1BUTTON_START, &Controller::IsStartButtonDown },
        { P1BUTTON_BACK, &Controller::IsBackButtonDown },

        { P1BUTTON_LEFTTHUMB, &Controller::IsLeftThumbstickButtonDown },
        { P1BUTTON_RIGHTTHUMB, &Controller::IsRightThumbstickButtonDown },

        { P1BUTTON_LEFTTRIGGER, &Controller::IsLeftTriggerPressed },
        { P1BUTTON_RIGHTTRIGGER, &Controller::IsRightTriggerPressed },

        { P1BUTTON_LEFTBUMPER, &Controller::IsLeftBumperDown },
        { P1BUTTON_RIGHTBUMPER, &Controller::IsRightBumperDown }
    },
    {
        { P2BUTTON_A, &Controller::IsAButtonDown },
        { P2BUTTON_B, &Controller::IsBButtonDown },
        { P2BUTTON_X, &Controller::IsXButtonDown },
        { P2BUTTON_Y, &Controller::IsYButtonDown },

        { P2BUTTON_START, &Controller::IsStartButtonDown },
        { P2BUTTON_BACK, &Controller::IsBackButtonDown },

        { P2BUTTON_LEFTTHUMB, &Controller::IsLeftThumbstickButtonDown },
        { P2BUTTON_RIGHTTHUMB, &Controller::IsRightThumbstickButtonDown },

        { P2BUTTON_LEFTTRIGGER, &Controller::IsLeftTriggerPressed },
        { P2BUTTON_RIGHTTRIGGER, &Controller::IsRightTriggerPressed },

        { P2BUTTON_LEFTBUMPER, &Controller::IsLeftBumperDown },
        { P2BUTTON_RIGHTBUMPER, &Controller::IsRightBumperDown }
    }
};

void InputManager::HandleControl( Controller& controller)
{
	//TODO: make this so it's not just cut-and-pasted code iterating over two enums

    int controllerId = controller.GetControllerID();
    assert(controllerId < MAX_CONTROLLERS);

	int numXboxButtons = sizeof(sBindRecords[controllerId]) / sizeof(sBindRecords[controllerId][0]);
    for (int i = 0; i < numXboxButtons; i++)
    {
        XboxButtonBindRecord& rec = sBindRecords[controllerId][i];

        //Is button currently down
        bool bIsDown = (controller.*rec.CheckFunc)();
        bool bWasDown = LastXboxButtonStates(controllerId)[rec.HashKey];

        //Update key value
        CurrentXboxButtonStates(controllerId)[rec.HashKey] = bIsDown;

        InputBinding* pBinding = GetBinding(rec.HashKey);
        if (pBinding == NULL)
            continue;

        if (!bWasDown && bIsDown)
        {
            //BUTTON DOWN            
            pBinding->OnKeyDown();
        }
        else if (bWasDown && !bIsDown)
        {
            //BUTTON UP
            pBinding->OnKeyUp();
        }
    }

    auto leftStick = controller.GetLeftThumbstick();
    auto rightStick = controller.GetRightThumbstick();
    float maxStick = (int) ((1 << 15) - 1);

    InputKeys p = XBOX_LEFTSTICK_X(controllerId);

    CurrentXboxAnalogButtonStates(controllerId)[XBOX_LEFTSTICK_X(controllerId)] = (float)leftStick.X / maxStick;
    CurrentXboxAnalogButtonStates(controllerId)[XBOX_LEFTSTICK_Y(controllerId)] = (float)leftStick.Y / maxStick;
    CurrentXboxAnalogButtonStates(controllerId)[XBOX_RIGHTSTICK_X(controllerId)] = (float)rightStick.X / maxStick;
    CurrentXboxAnalogButtonStates(controllerId)[XBOX_RIGHTSTICK_Y(controllerId)] = (float)rightStick.Y / maxStick;

    CurrentXboxButtonStates(controllerId)[XBOX_LEFTSTICK_RIGHT(controllerId)] =
        (CurrentXboxAnalogButtonStates(controllerId)[XBOX_LEFTSTICK_X(controllerId)] >= g_stickSensitivity);

    CurrentXboxButtonStates(controllerId)[XBOX_LEFTSTICK_LEFT(controllerId)] =
        (CurrentXboxAnalogButtonStates(controllerId)[XBOX_LEFTSTICK_X(controllerId)] <= -g_stickSensitivity);

    CurrentXboxButtonStates(controllerId)[XBOX_LEFTSTICK_UP(controllerId)] = 
        (CurrentXboxAnalogButtonStates(controllerId)[XBOX_LEFTSTICK_Y(controllerId)] >= g_stickSensitivity);

    CurrentXboxButtonStates(controllerId)[XBOX_LEFTSTICK_DOWN(controllerId)] =
        (CurrentXboxAnalogButtonStates(controllerId)[XBOX_LEFTSTICK_Y(controllerId)] <= -g_stickSensitivity);

    CurrentXboxButtonStates(controllerId)[XBOX_RIGHTSTICK_RIGHT(controllerId)] =
        (CurrentXboxAnalogButtonStates(controllerId)[XBOX_RIGHTSTICK_X(controllerId)] >= g_stickSensitivity);

    CurrentXboxButtonStates(controllerId)[XBOX_RIGHTSTICK_LEFT(controllerId)] =
        (CurrentXboxAnalogButtonStates(controllerId)[XBOX_RIGHTSTICK_X(controllerId)] <= -g_stickSensitivity);

    CurrentXboxButtonStates(controllerId)[XBOX_RIGHTSTICK_UP(controllerId)] =
        (CurrentXboxAnalogButtonStates(controllerId)[XBOX_RIGHTSTICK_Y(controllerId)] >= g_stickSensitivity);

    CurrentXboxButtonStates(controllerId)[XBOX_RIGHTSTICK_DOWN(controllerId)] =
        (CurrentXboxAnalogButtonStates(controllerId)[XBOX_RIGHTSTICK_Y(controllerId)] <= -g_stickSensitivity);
}

void InputManager::Update()
{
    UpdatePlayersControl();
}

void InputManager::OnUpdateEnd()
{
	for (unsigned int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		SwapXboxButtonStates(i);
	}

	SwapKeyboardButtonState();
}

void InputManager::BindButton(std::string const& button, InputKeys key)
{
    m_buttonsMap[button].push_back(key);
}

void InputManager::ResetButton(std::string const& button)
{
    m_buttonsMap.erase(button);
}

bool InputManager::IsButtonPressed(std::string const& button)
{
    for (auto& key : m_buttonsMap[button])
    {
        for (int i = 0; i < MAX_CONTROLLERS; ++i)
        {
            if (IsXboxPressed(i, key))
            {
                return true;
            }
        }

        if (IsKeyPressed(key))
        {
            return true;
        }
    }

    return false;
}

void InputManager::BindPlayersInput()
{
    for (int i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        std::string s = "Player" + IntToString(i + 1) + "Control";
        std::string str = thePrefs.GetString("Settings", "Player" + IntToString(i + 1) + "Control");

        if (str == "Gamepad1")
        {
            theInput.BindPlayerInput(i, DefaultXboxController1PlayerInput());
        }
        else if (str == "Gamepad2")
        {
            theInput.BindPlayerInput(i, DefaultXboxController2PlayerInput());
        }
        else
        {
            theInput.BindPlayerInput(i, DefaultKeyboardPlayerInput());
        }
    }

    theInput.SetInverted(EInputControllerType::XBOX, 0, thePrefs.GetInt("Settings", "GamepadInverted") != 0);
    theInput.SetInverted(EInputControllerType::XBOX, 1, thePrefs.GetInt("Settings", "GamepadInverted") != 0);
    theInput.SetInverted(EInputControllerType::KEYBOARD, 0, thePrefs.GetInt("Settings", "KeyboardInverted") != 0);
}

void InputManager::SetInverted(EInputControllerType type, size_t controllerid, bool inverted)
{
    switch (type)
    {
    case EInputControllerType::KEYBOARD:
        m_keyboardInfo.m_inverted = inverted;
        break;
    case EInputControllerType::XBOX:
        m_xboxControllerInfo[controllerid].m_inverted = inverted;
        break;
    default:
        break;
    }
}

void InputManager::UpdatePlayersControl()
{
    for (uint32_t i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        SPlayerInputBinding& binding = m_playerBindingTable[i];
        if (!binding.m_isBinding)
        {
            continue;
        }
        
        switch (binding.m_type)
        {
        case EInputControllerType::KEYBOARD:
            UpdatePlayerKeyboard(i);
            break;
        case EInputControllerType::XBOX:
            UpdatePlayerXbox(i);
            break;
        }
    }
}

void InputManager::UpdatePlayerKeyboard(unsigned int playerId)
{
    SPlayerInputBinding& binding = m_playerBindingTable[playerId];

    for (auto& iter : binding.m_keys)
    {
		if (iter.second.m_key == ANGEL_KEY_NULL)
		{
			continue;
		}

        UpdatePlayerKeyboardKey(playerId, iter.first, iter.second);
    } 
}

void InputManager::UpdatePlayerXbox(unsigned int playerId)
{
    SPlayerInputBinding& binding = m_playerBindingTable[playerId];

    for (auto& iter : binding.m_keys)
    {
		if (iter.second.m_key == ANGEL_KEY_NULL)
		{
			continue;
		}

        UpdatePlayerXboxKey(playerId, iter.first, iter.second);
    }
}

void InputManager::UpdatePlayerKeyboardKey(unsigned int playerId, EPlayerInputEventType type, SInputKey key)
{
    if (key.m_type == EInputKeyType::DIGITAL)
    {
        if (IsKeyPressed((int)key.m_key))
        {
            if (key.m_onKeyboardKeyPressed != nullptr)
            {
                key.m_onKeyboardKeyPressed(this);
            }

            OnPlayerKeyPress(playerId, SPlayerDigitalInputEvent{ type });
        }
        else if (IsKeyReleased((int)key.m_key))
        {
            OnPlayerKeyRelease(playerId, SPlayerDigitalInputEvent{ type });
        }
    }
    else
    {
        //MAGIC
        ;
    }
}

void InputManager::UpdatePlayerXboxKey(unsigned int playerId, EPlayerInputEventType type, SInputKey key)
{
    int id = m_playerBindingTable[playerId].m_controllerId;

    if (key.m_type == EInputKeyType::DIGITAL)
    {
        if (IsXboxPressed(id, (int)key.m_key))
        {
            if (key.m_onXboxKeyPressed != nullptr)
            {
                key.m_onXboxKeyPressed(theControllerManager.GetController(m_playerBindingTable[playerId].m_controllerId));
            }

            OnPlayerKeyPress(playerId, SPlayerDigitalInputEvent{ type });
        }
        else if (IsXboxReleased(id, (int)key.m_key))
        {
            OnPlayerKeyRelease(playerId, SPlayerDigitalInputEvent{ type });
        }
    }
    else
    {
        OnPlayerAnalogInput(playerId, SPlayerAnalogInputEvent{ type, GetXboxAnalogState(id, (int)key.m_key) });
    }
}

void InputManager::ClearXboxButtonStates()
{
    for (int controllerid = 0; controllerid < MAX_CONTROLLERS; ++controllerid)
    {
        int numXboxButtons = sizeof(sBindRecords[controllerid]) / sizeof(sBindRecords[controllerid][0]);
        for (int i = 0; i < numXboxButtons; i++)
        {
            XboxButtonBindRecord& rec = sBindRecords[controllerid][i];
            CurrentXboxButtonStates(controllerid)[rec.HashKey] = false;
            LastXboxButtonStates(controllerid)[rec.HashKey] = false;
        }
    }
}

InputManager::ButtonState& InputManager::CurrentKeyboardButtonStates()
{
    int index = (m_currentKeyboardButtonStateBuffer) ? (1) : (0);

    return m_keyboardButtonStates[index];
}

InputManager::ButtonState& InputManager::LastKeyboardButtonStates()
{
    int index = (!m_currentKeyboardButtonStateBuffer) ? (1) : (0);

    return m_keyboardButtonStates[index];
}

InputManager::AnalogButtonState& InputManager::CurrentXboxAnalogButtonStates(unsigned int controllerId)
{
    return m_xboxAnalogButtonStates[controllerId];
}

InputManager::ButtonState& InputManager::CurrentXboxButtonStates(unsigned int controllerId)
{
    int index = (m_currentXboxButtonStateBuffer[controllerId]) ? (1) : (0);

    return m_xboxButtonStates[controllerId][index];
}

InputManager::ButtonState& InputManager::LastXboxButtonStates(unsigned int controllerId)
{
    int index = (!m_currentXboxButtonStateBuffer[controllerId]) ? (1) : (0);

    return m_xboxButtonStates[controllerId][index];
}

void InputManager::SwapXboxButtonStates(unsigned int controllerId)
{
    int index = (m_currentXboxButtonStateBuffer[controllerId]) ? (1) : (0);

    m_currentXboxButtonStateBuffer[controllerId] = !m_currentXboxButtonStateBuffer[controllerId];

    CurrentXboxButtonStates(controllerId) = LastXboxButtonStates(controllerId);
}

void InputManager::SwapKeyboardButtonState()
{
    int index = (m_currentKeyboardButtonStateBuffer) ? (1) : (0);

    m_currentKeyboardButtonStateBuffer = !m_currentKeyboardButtonStateBuffer;

    CurrentKeyboardButtonStates() = LastKeyboardButtonStates();
}

void InputManager::OnPlayerAnalogInput(unsigned int playerId, SPlayerAnalogInputEvent analogInput)
{
    auto& listeners = m_playerListenerTable[playerId];
    std::shared_ptr<IPlayerInputListener> listener = nullptr;

    for(size_t i = 0; i < listeners.size(); ++i)
    {
        listener = listeners[i].lock();

        if (listener == nullptr)
        {
            listeners[i] = listeners.back();
            listeners.pop_back();
        }
        else
        {
            listener->OnPlayerAnalogInput(analogInput, m_playerBindingTable[playerId]);
        }        
    }
}

void InputManager::OnPlayerKeyPress(unsigned int playerId, SPlayerDigitalInputEvent digitalInput)
{
    auto& listeners = m_playerListenerTable[playerId];
    std::shared_ptr<IPlayerInputListener> listener = nullptr;

    for (size_t i = 0; i < listeners.size(); ++i)
    {
        listener = listeners[i].lock();

        if (listener == nullptr)
        {
            listeners[i] = listeners.back();
            listeners.pop_back();
        }
        else
        {
            listener->OnPlayerKeyPress(digitalInput, m_playerBindingTable[playerId]);
        }
    }
}

void InputManager::OnPlayerKeyRelease(unsigned int playerId, SPlayerDigitalInputEvent digitalInput)
{
    auto& listeners = m_playerListenerTable[playerId];
    std::shared_ptr<IPlayerInputListener> listener = nullptr;

    for (size_t i = 0; i < listeners.size(); ++i)
    {
        listener = listeners[i].lock();

        if (listener == nullptr)
        {
            listeners[i] = listeners.back();
            listeners.pop_back();
        }
        else
        {
            listener->OnPlayerKeyRelease(digitalInput, m_playerBindingTable[playerId]);
        }
    }
}

bool InputManager::IsKeyDown(int keyVal)
{
    if (glfwGetKey(theWorld.GetMainWindow(), toupper(keyVal)) == GLFW_PRESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::IsKeyPressed(int keyVal)
{
    if (CurrentKeyboardButtonStates().find(keyVal) == CurrentKeyboardButtonStates().end())
    {
        return false;
    }

    return CurrentKeyboardButtonStates()[keyVal] && !LastKeyboardButtonStates()[keyVal];
}

bool InputManager::IsKeyReleased(int keyVal)
{
    if (CurrentKeyboardButtonStates().find(keyVal) == CurrentKeyboardButtonStates().end())
    {
        return false;
    }

    return !CurrentKeyboardButtonStates()[keyVal] && LastKeyboardButtonStates()[keyVal];
}

bool InputManager::IsXboxPressed(unsigned int controllerId, int keyVal)
{
    if (CurrentXboxButtonStates(controllerId).find(keyVal) == CurrentXboxButtonStates(controllerId).end())
    {
        return false;
    }

    return CurrentXboxButtonStates(controllerId)[keyVal] && !LastXboxButtonStates(controllerId)[keyVal];
}

bool InputManager::IsXboxReleased(unsigned int controllerId, int keyVal)
{
    if (CurrentXboxButtonStates(controllerId).find(keyVal) == CurrentXboxButtonStates(controllerId).end())
    {
        return false;
    }

    return !CurrentXboxButtonStates(controllerId)[keyVal] && LastXboxButtonStates(controllerId)[keyVal];
}

float InputManager::GetXboxAnalogState(unsigned int controllerId, int keyVal)
{
    auto& states = CurrentXboxAnalogButtonStates(controllerId);

    if (states.find(keyVal) == states.end())
    {
        return 0.0f;
    }

    return states[keyVal];
}

InputBinding* InputManager::GetBinding( int hashVal )
{
	//Find existing binding
	BindingTable::iterator itr = _bindingTable.find(hashVal);
	if( itr == _bindingTable.end() )
		return NULL;

	return (*itr).second;
}

int InputManager::GetHashFromKeyName( const String& keyId )
{
	//Find key ID
	KeyNameTable::iterator itr = _keyNameTable.find(ToUpper(keyId));
	if( itr == _keyNameTable.end() )
		return -1;

	return (*itr).second;

}

SPlayerInputBinding DefaultKeyboardPlayerInput()
{
    SPlayerInputBinding binding;

    binding.m_keys[EPlayerInputEventType::SHOT] = { EInputKeyType::DIGITAL, InputKeys::ANGEL_KEY_SPACE };
    binding.m_keys[EPlayerInputEventType::ROTATE_LEFT] = { EInputKeyType::DIGITAL, InputKeys::ANGEL_GLFW_KEY_A };
    binding.m_keys[EPlayerInputEventType::ROTATE_RIGHT] = { EInputKeyType::DIGITAL, InputKeys::ANGEL_GLFW_KEY_D };
    binding.m_keys[EPlayerInputEventType::SPEED_UP] = { EInputKeyType::DIGITAL, InputKeys::ANGEL_GLFW_KEY_W };
    binding.m_keys[EPlayerInputEventType::SLOW_DOWN] = { EInputKeyType::DIGITAL, InputKeys::ANGEL_GLFW_KEY_S };
    binding.m_keys[EPlayerInputEventType::ROTATE_TURRET_LEFT] ={ EInputKeyType::DIGITAL, InputKeys::ANGEL_KEY_LEFTARROW };
    binding.m_keys[EPlayerInputEventType::ROTATE_TURRET_RIGHT] =  { EInputKeyType::DIGITAL, InputKeys::ANGEL_KEY_RIGHTARROW };

    binding.m_keys[EPlayerInputEventType::SHOP_ENTER] = { EInputKeyType::DIGITAL, InputKeys::ANGEL_GLFW_KEY_E };
    binding.m_keys[EPlayerInputEventType::SHOP_DOWN] = { EInputKeyType::DIGITAL, InputKeys::ANGEL_KEY_DOWNARROW };
    binding.m_keys[EPlayerInputEventType::SHOP_UP] = { EInputKeyType::DIGITAL, InputKeys::ANGEL_KEY_UPARROW };

    binding.m_type = EInputControllerType::KEYBOARD;
    
    return binding;
}

SPlayerInputBinding DefaultXboxController1PlayerInput()
{
    SPlayerInputBinding binding;

    binding.m_keys[EPlayerInputEventType::SHOT] = { EInputKeyType::DIGITAL, InputKeys::P1BUTTON_RIGHTTRIGGER };
    binding.m_keys[EPlayerInputEventType::ROTATE_LEFT] = { EInputKeyType::ANALOG, InputKeys::ANGEL_KEY_NULL };
    binding.m_keys[EPlayerInputEventType::ROTATE_RIGHT] = { EInputKeyType::ANALOG, InputKeys::P1BUTTON_LEFTSTICK_X };
    binding.m_keys[EPlayerInputEventType::SPEED_UP] = { EInputKeyType::ANALOG, InputKeys::P1BUTTON_LEFTSTICK_Y };
    binding.m_keys[EPlayerInputEventType::SLOW_DOWN] = { EInputKeyType::ANALOG, InputKeys::ANGEL_KEY_NULL };
    binding.m_keys[EPlayerInputEventType::ROTATE_TURRET_LEFT] = { EInputKeyType::ANALOG, InputKeys::ANGEL_KEY_NULL };
    binding.m_keys[EPlayerInputEventType::ROTATE_TURRET_RIGHT] = { EInputKeyType::ANALOG, InputKeys::P1BUTTON_RIGHTSTICK_X };

    binding.m_keys[EPlayerInputEventType::SHOP_ENTER] = { EInputKeyType::DIGITAL, InputKeys::P1BUTTON_A };
    binding.m_keys[EPlayerInputEventType::SHOP_DOWN] = { EInputKeyType::DIGITAL, InputKeys::P1BUTTON_RIGHTSTICK_DOWN };
    binding.m_keys[EPlayerInputEventType::SHOP_UP] = { EInputKeyType::DIGITAL, InputKeys::P1BUTTON_RIGHTSTICK_UP };

    binding.m_type = EInputControllerType::XBOX;
    binding.m_controllerId = 0;

    return binding;
}

SPlayerInputBinding DefaultXboxController2PlayerInput()
{
    SPlayerInputBinding binding;

    binding.m_keys[EPlayerInputEventType::SHOT] = { EInputKeyType::DIGITAL, InputKeys::P2BUTTON_RIGHTTRIGGER };
    binding.m_keys[EPlayerInputEventType::ROTATE_LEFT] = { EInputKeyType::ANALOG, InputKeys::ANGEL_KEY_NULL };
    binding.m_keys[EPlayerInputEventType::ROTATE_RIGHT] = { EInputKeyType::ANALOG, InputKeys::P2BUTTON_LEFTSTICK_X };
    binding.m_keys[EPlayerInputEventType::SPEED_UP] = { EInputKeyType::ANALOG, InputKeys::P2BUTTON_LEFTSTICK_Y };
    binding.m_keys[EPlayerInputEventType::SLOW_DOWN] = { EInputKeyType::ANALOG, InputKeys::ANGEL_KEY_NULL };
    binding.m_keys[EPlayerInputEventType::ROTATE_TURRET_LEFT] = { EInputKeyType::ANALOG, InputKeys::ANGEL_KEY_NULL };
    binding.m_keys[EPlayerInputEventType::ROTATE_TURRET_RIGHT] = { EInputKeyType::ANALOG, InputKeys::P2BUTTON_RIGHTSTICK_X };

    binding.m_keys[EPlayerInputEventType::SHOP_ENTER] = { EInputKeyType::DIGITAL, InputKeys::P2BUTTON_A };
    binding.m_keys[EPlayerInputEventType::SHOP_DOWN] = { EInputKeyType::DIGITAL, InputKeys::P2BUTTON_RIGHTSTICK_DOWN };
    binding.m_keys[EPlayerInputEventType::SHOP_UP] = { EInputKeyType::DIGITAL, InputKeys::P2BUTTON_RIGHTSTICK_UP };

    binding.m_type = EInputControllerType::XBOX;
    binding.m_controllerId = 1;

    return binding;
}