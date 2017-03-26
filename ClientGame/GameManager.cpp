#include "stdafx.h"
#include "GameManager.h"
#include "Interfaces.h"
#include "PhysicsUnit.h"
#include "GamePlayScreen.h"
#include "PauseScreen.h"
#include "LevelScreen.h"
#include "HelpScreen.h"

std::shared_ptr<CScreen> CreateScreen(EScreen screen, IEnumScreenStack* stack)
{
    std::shared_ptr<CScreen> ptr;
    switch (screen)
    {
    case EScreen::MAIN_MENU:
        ptr = std::make_shared<CMainMenuScreen>(stack);
        break;
    case EScreen::GAME_PLAY:
        ptr = std::make_shared<CGamePlayScreen>(stack);
        break;
    case EScreen::OPTIONS:
        ptr = std::make_shared<COptionsScreen>(stack);
        break;
    case EScreen::CONTROL:
        ptr = std::make_shared<CControlScreen>(stack);
        break;
    case EScreen::AUDIO:
        ptr = std::make_shared<CAudioScreen>(stack);
        break;
    case EScreen::PAUSE:
        ptr = std::make_shared<CPauseScreen>(stack);
        break;
    case EScreen::LEVEL:
        ptr = std::make_shared<CLevelScreen>(stack);
        break;
    case EScreen::HELP:
        ptr = std::make_shared<CHelpScreen>(stack);
        break;
    }

    return ptr;
}

CGameManager::CGameManager()
{
    InitializeInput();
    
    m_screens.reserve(static_cast<size_t>(EScreen::END));

    for (size_t i = 0, n = static_cast<size_t>(EScreen::END); i != n; ++i)
    {
        m_screens.emplace_back(::CreateScreen(static_cast<EScreen>(i), this));
    }

    m_background = std::make_shared<Actor>();
    m_background->SetSprite("Resources/Images/welocme_screen.png");
    m_background->SetSize(MathUtil::GetWorldDimensions());
    theWorld.Add(m_background.get());
    PushScreen(EScreen::MAIN_MENU);

    theInput.BindPlayersInput();
    theUI.SetDefaultFont("Resources/Fonts/BlackOpsOne-Regular.ttf", 20.0f);
    theUI.SetDefaultColor({ 1.0f, 0.65f, 0.0f, 1.0f });
}

void CGameManager::Update(float dt)
{
    m_screenStack.GetTop()->Update(dt);
}

void CGameManager::Render()
{
    m_screenStack.GetTop()->Render();
}

void CGameManager::PushScreen(EScreen screen)
{
    if (screen == EScreen::END)
    {
        theWorld.StopGame();
    }
    else
    {
        m_screenStack.PushScreen(
            CreateScreen(screen));
    }
    if (screen == EScreen::GAME_PLAY)
    {
        theWorld.Remove(m_background.get());
    }
}

void CGameManager::PopScreen()
{
    if (m_screenStack.GetTop() == m_screens[static_cast<size_t>(EScreen::GAME_PLAY)])
    {
        theWorld.Add(m_background.get());
    }

    m_screenStack.PopScreen();
}

std::shared_ptr<CScreen> CGameManager::CreateScreen(EScreen screen)
{
    auto scr = m_screens[static_cast<size_t>(screen)];
    scr->SetScreenStack(this);
    return scr;
}

void CGameManager::InitializeInput()
{
    theInput.BindButton("Up", ANGEL_KEY_UPARROW);
    theInput.BindButton("Up", P1BUTTON_LEFTSTICK_UP);
    theInput.BindButton("Up", P2BUTTON_LEFTSTICK_UP);

    theInput.BindButton("Left", ANGEL_KEY_LEFTARROW);
    theInput.BindButton("Left", P1BUTTON_LEFTSTICK_LEFT);
    theInput.BindButton("Left", P2BUTTON_LEFTSTICK_LEFT);

    theInput.BindButton("Right", ANGEL_KEY_RIGHTARROW);
    theInput.BindButton("Right", P1BUTTON_LEFTSTICK_RIGHT);
    theInput.BindButton("Right", P2BUTTON_LEFTSTICK_RIGHT);

    theInput.BindButton("Down", ANGEL_KEY_DOWNARROW);
    theInput.BindButton("Down", P1BUTTON_LEFTSTICK_DOWN);
    theInput.BindButton("Down", P2BUTTON_LEFTSTICK_DOWN);

    theInput.BindButton("Back", ANGEL_KEY_ESCAPE);
    theInput.BindButton("Back", P1BUTTON_B);
    theInput.BindButton("Back", P2BUTTON_B);

    theInput.BindButton("Enter", ANGEL_KEY_ENTER);
    theInput.BindButton("Enter", P1BUTTON_A);
    theInput.BindButton("Enter", P2BUTTON_A);
}
