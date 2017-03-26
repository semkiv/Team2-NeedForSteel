#include "stdafx.h"
#include "GamePlayScreen.h"

CGamePlayScreen::CGamePlayScreen(IEnumScreenStack* stack) : 
CScreen(stack)
{}

void CGamePlayScreen::Start()
{
    m_game->SetPause(false);
}

void CGamePlayScreen::OnPush()
{
    m_game = std::make_shared<CGame>();
    m_game->SetLevel(("Config/Level/0" + std::to_string(thePrefs.GetInt("Settings", "Level")) + std::string(".lua")));
    m_game->StartGame();
}

void CGamePlayScreen::OnPop()
{
    m_game->StopGame();
    m_game.reset();
}

void CGamePlayScreen::Stop()
{
    m_game->SetPause(true);
}

void CGamePlayScreen::Update(float dt)
{
    if (!m_game->IsPaused() && !m_game->GameOver())
    {
        m_game->Update(dt);
    }

    if (!m_screenStack.Empty())
    {
        m_screenStack.GetTop()->Update(dt);
    }

    if (theInput.IsButtonPressed("Back") && m_screenStack.Empty())
    {
        if (!m_game->GameOver())
        {
            Stop();
            PushScreen(EScreen::PAUSE);
        }
        else
        {
            m_stack->PopScreen();
        }
    }
}

void CGamePlayScreen::Render()
{
    if (!m_game->IsPaused())
    {
        m_game->Render();
    }
}

void CGamePlayScreen::PushScreen(EScreen screen)
{
    if (screen == EScreen::END)
    {
        m_screenStack.PopScreen();
        m_stack->PopScreen();
    }
    else
    {
        m_screenStack.PushScreen(CreateScreen(screen));
    }
}

void CGamePlayScreen::PopScreen()
{
    m_screenStack.PopScreen();

    if (m_screenStack.Empty())
    {
        Start();
    }
}

std::shared_ptr<CScreen> CGamePlayScreen::CreateScreen(EScreen screen)
{
    auto scr = m_stack->CreateScreen(screen);
    scr->SetScreenStack(this);
    return scr;
}
