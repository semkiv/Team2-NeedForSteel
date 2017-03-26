#pragma once
#include "Screen.h"

class CHelpScreen:
    public CMenuScreen
{
public:
    CHelpScreen(IEnumScreenStack* stack);

    virtual void Start() override;
    virtual void Stop() override;

    virtual void Update(float dt) override;

    virtual void MoveUp() override;
    virtual void MoveDown() override;
    virtual void Enter() override;

private:
    std::shared_ptr<Actor>m_controls;
};