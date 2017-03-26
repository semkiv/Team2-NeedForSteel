#pragma once
#include "Screen.h"

class CLevelScreen :
    public CMenuScreen
{
public:
    CLevelScreen(IEnumScreenStack* stack);
    virtual void Start() override;
    virtual void Stop() override;

    virtual void Update(float dt) override;

    virtual void MoveUp() override;
    virtual void MoveDown() override;
    virtual void Enter() override;
    virtual void MoveRight() override;
    virtual void MoveLeft() override;

    virtual ~CLevelScreen() = default;
};