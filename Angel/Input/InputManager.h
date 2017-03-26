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

#pragma once

#include "../Util/StringUtil.h"
#include "../Input/Controller.h"

#include <vector>
#include <memory>

///(Internal) Handles the binding of keypresses to Messages
/**
* An internal class used by the InputManager, mapping keydown and keyup
*  events to the appropriate Messages.
*/
class InputBinding
{
public:

	void SetKeyDownMessage(const String& keyDownMessage);
	void SetKeyUpMessage(const String& keyUpMessage);

	void OnKeyDown();
	void OnKeyUp();

private:
	String _keyDownMessage;
	String _keyUpMessage;
};

enum InputKeys
{
#define USEINPUTKEY(e,hashVal,text) \
	e=hashVal,
#include "InputKeys.h"
#undef USEINPUTKEY
};

#define theInput InputManager::GetInstance()

class InputManager;

int constexpr MAX_PLAYER_COUNT = 2;
int const XBOX_KEYS_SHIFT = 1000;
int const XBOX_CONTROLLER_SHIFT = 100;

InputKeys XBOX_BUTTON(int controllerId, InputKeys key);
InputKeys XBOX_LEFTSTICK_X(int controllerId);
InputKeys XBOX_LEFTSTICK_Y(int controllerId);
InputKeys XBOX_RIGHTSTICK_X(int controllerId);
InputKeys XBOX_RIGHTSTICK_Y(int controllerId);

///(Internal) Handles keyboard input, and mapping controller input to messages
/**
* This internal class is used by the engine to take the settings from
*  input_bindings.ini and process all keyboard and controller data.
*
* Unless you're working on the engine itself, the only function in here that
*  matters to you would be InputManager::IsKeyDown.
*/
enum class EInputKeyType
{
    DIGITAL,
    ANALOG
};

using OnKeyboardKeyPressedEvent = void(*)(InputManager*);
using OnXboxKeyPressedEvent = void(*)(Controller&);

struct SInputKey
{
    SInputKey() :
        m_type(EInputKeyType::DIGITAL),
        m_key(InputKeys::ANGEL_KEY_NULL)
    {
    }

    SInputKey(EInputKeyType type, InputKeys key, OnXboxKeyPressedEvent onXboxKeyPressed = nullptr, OnKeyboardKeyPressedEvent onKeyboardKeyPressed = nullptr) :
        m_type(type),
        m_key(key),
        m_onXboxKeyPressed(onXboxKeyPressed),
        m_onKeyboardKeyPressed(onKeyboardKeyPressed)
    {
    }

    EInputKeyType   m_type;
    InputKeys       m_key;
    
    OnXboxKeyPressedEvent m_onXboxKeyPressed = nullptr;
    OnKeyboardKeyPressedEvent m_onKeyboardKeyPressed = nullptr;
    
};

enum class EInputControllerType
{
    KEYBOARD,
    XBOX
};

enum class EPlayerInputEventType
{
	NONE,
	SHOT,
	ROTATE_LEFT,
	ROTATE_RIGHT,
	SPEED_UP,
	SLOW_DOWN,
	ROTATE_TURRET_LEFT,
	ROTATE_TURRET_RIGHT,

    SHOP_ENTER,
    SHOP_DOWN,
    SHOP_UP
};

struct SPlayerAnalogInputEvent
{
    EPlayerInputEventType   m_type;
    float                   m_value;
};

struct SPlayerDigitalInputEvent
{
    EPlayerInputEventType m_type;
};

struct SPlayerInputBinding
{
    std::unordered_map<EPlayerInputEventType, SInputKey> m_keys;

    EInputControllerType    m_type = EInputControllerType::KEYBOARD;
    bool                    m_isBinding = false;
    size_t                  m_controllerId = 0;
};

class IPlayerInputListener
{
public:
	virtual void OnPlayerKeyPress(SPlayerDigitalInputEvent digitalInput, SPlayerInputBinding const& inputBinding) = 0;
    virtual void OnPlayerKeyRelease(SPlayerDigitalInputEvent digitalInput, SPlayerInputBinding const& inputBinding) = 0;
    virtual void OnPlayerAnalogInput(SPlayerAnalogInputEvent analogInput, SPlayerInputBinding const& inputBinding) = 0;
};

struct SControllerInfo
{
    bool m_inverted;
};

SPlayerInputBinding DefaultKeyboardPlayerInput();
SPlayerInputBinding DefaultXboxController1PlayerInput();
SPlayerInputBinding DefaultXboxController2PlayerInput();

class InputManager
{
	typedef hashmap_ns::hash_map<int, InputBinding*>		BindingTable;
	typedef hashmap_ns::hash_map<String, int>				KeyNameTable;
	typedef hashmap_ns::hash_map<int, bool>					ButtonState;
    typedef hashmap_ns::hash_map<int, float>			    AnalogButtonState;
    using ButtonMap = hashmap_ns::hash_map<std::string, std::vector<InputKeys>>;

public:
	static InputManager &GetInstance();
	static void Destroy();

	void BindKey(const String& keyId, const String& command);
	void UnbindKey(const String& keyId);

	bool OnKeyDown(int keyVal);
	bool OnKeyUp(int keyVal);

	void BindPlayerInput(unsigned int playerId, SPlayerInputBinding const& binding);
    SPlayerInputBinding const& GetPlayerInputBinding(size_t playerId) const;
    void UnbindPlayer(unsigned int playerId);

    SControllerInfo const& GetControllerInfo(EInputControllerType type, size_t controllerId) const;

	void AddPlayerListener(unsigned int playerId, std::shared_ptr<IPlayerInputListener> listener);
    void ClearPlayerListeners(unsigned int playerId);

	//    EPlayerInputEventType GetPlayerMessage(int playerId, String const& message);

	/**
	* Find out whether a key is currently pressed. Can either be passed an
	*  individual char or any of the defined values in GL/glfw.h.
	*
	* @param keyVal The key value to test for (usually a char)
	* @return Whether the user is currently pressing that key.
	*/
	bool IsKeyDown(int keyVal);

    bool IsKeyPressed(int keyVal);
    bool IsKeyReleased(int keyVal);

    bool IsXboxPressed(unsigned int controllerId, int keyVal);
    bool IsXboxReleased(unsigned int controllerId, int keyVal);

    float GetXboxAnalogState(unsigned int controllerId, int keyVal);

	void HandleControl(class Controller& controller);

    void Update();  
	void OnUpdateEnd();

    void BindButton(std::string const& button, InputKeys key);
    void ResetButton(std::string const& button);
    bool IsButtonPressed(std::string const& button);

    void BindPlayersInput();
    void SetInverted(EInputControllerType type, size_t controllerid, bool inverted);

protected:
	InputManager() {}
	~InputManager();
	void Initialize();

private:
	InputBinding* GetBinding(int hashVal);
	int GetHashFromKeyName(const String& keyId);
    void ClearXboxButtonStates();

    ButtonState& CurrentKeyboardButtonStates();
    ButtonState& LastKeyboardButtonStates();
    AnalogButtonState& CurrentXboxAnalogButtonStates(unsigned int controllerId);
    ButtonState& CurrentXboxButtonStates(unsigned int controllerId);
    ButtonState& LastXboxButtonStates(unsigned int controllerId);

    void SwapXboxButtonStates(unsigned int controllerId);
    void SwapKeyboardButtonState();

    void UpdatePlayersControl();

    void UpdatePlayerXbox(unsigned int playerId);
    void UpdatePlayerKeyboard(unsigned int playerId);

    void UpdatePlayerKeyboardKey(unsigned int playerId, EPlayerInputEventType type, SInputKey key);
    void UpdatePlayerXboxKey(unsigned int playerId, EPlayerInputEventType type, SInputKey key);

    void OnPlayerKeyPress(unsigned int playerId, SPlayerDigitalInputEvent digitalInput);
    void OnPlayerKeyRelease(unsigned int playerId, SPlayerDigitalInputEvent digitalInput);
    void OnPlayerAnalogInput(unsigned int playerId, SPlayerAnalogInputEvent analogInput);

private:
	static InputManager* s_Input;

	KeyNameTable		_keyNameTable;
	BindingTable		_bindingTable;
    ButtonMap           m_buttonsMap;

    bool m_currentKeyboardButtonStateBuffer;
    ButtonState m_keyboardButtonStates[2];
    SControllerInfo m_keyboardInfo;

    bool                m_currentXboxButtonStateBuffer[2];
    ButtonState	        m_xboxButtonStates[MAX_CONTROLLERS][2];
    AnalogButtonState   m_xboxAnalogButtonStates[MAX_CONTROLLERS];    
    SControllerInfo     m_xboxControllerInfo[MAX_CONTROLLERS];

    SPlayerInputBinding m_defaultKeyboardBindings;
    SPlayerInputBinding m_defaultXboxBindings[MAX_CONTROLLERS];

    SPlayerInputBinding                                     m_playerBindingTable[MAX_PLAYER_COUNT];
    std::vector<std::weak_ptr<IPlayerInputListener>>        m_playerListenerTable[MAX_PLAYER_COUNT];
};