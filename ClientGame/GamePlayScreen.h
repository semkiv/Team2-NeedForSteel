#pragma once

#include "../Angel/Screen.h"
#include "Common.h"
#include "Game.h"

class CGamePlayScreen : 
	public CScreen,
    public IEnumScreenStack
{
public:
    CGamePlayScreen(IEnumScreenStack* stack);
    virtual void Start() override;
    virtual void OnPush() override;
    virtual void OnPop() override;
    virtual void Stop() override;
    virtual void Update(float dt) override;
    virtual void Render() override;

    virtual void PushScreen(EScreen screen) override;
    virtual void PopScreen() override;
    virtual std::shared_ptr<CScreen> CreateScreen(EScreen screen) override;

private:
    std::shared_ptr<CGame> m_game;
    CScreenStack m_screenStack;
};
