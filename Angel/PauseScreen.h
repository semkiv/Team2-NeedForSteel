#pragma once
#include "Screen.h"

class CPauseScreen :
    public CMenuScreen
{
public:
    CPauseScreen(IEnumScreenStack* stack);

    virtual void Update(float dt) override;

    virtual void Enter() override;

    virtual ~CPauseScreen();
};
