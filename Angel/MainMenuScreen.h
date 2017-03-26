#pragma once
#include "Screen.h"

class CMainMenuScreen :
    public CMenuScreen
{
public:
    CMainMenuScreen(IEnumScreenStack* stack);

    virtual void Update(float dt) override;
    virtual void Enter() override;
    virtual void Back() override;

    virtual ~CMainMenuScreen() = default;
};
