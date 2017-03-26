#pragma once
#include "Angel.h"
#include "Common.h"
#include "ShopUI.h"

class CGameManager : public GameManager,
    public IEnumScreenStack
{
public:
    CGameManager();
    virtual void Update(float dt) override;
    virtual void Render() override;
    virtual void PushScreen(EScreen screen) override;
    virtual void PopScreen() override;
    virtual std::shared_ptr<CScreen> CreateScreen(EScreen screen) override;

private:
    void InitializeInput();

private:
    std::vector<CScreenStack::ScreenPtr> m_screens;
    CScreenStack m_screenStack;
    std::shared_ptr<Actor> m_background;

};
