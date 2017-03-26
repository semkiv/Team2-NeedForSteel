#pragma once
#include "Screen.h"

class CControlScreen :
public CMenuScreen
{
public:
	CControlScreen(IEnumScreenStack* stack);
	
    virtual void Start() override;
    virtual void Stop() override;
	
	virtual void Update(float dt) override;

    virtual void MoveRight() override;
    virtual void MoveLeft() override;
	virtual void Enter() override;

    virtual void ChangeControl(bool ok) override;

	virtual ~CControlScreen() = default;

private:
    StringList m_controls;
    size_t m_currentButton1;
    size_t m_currentButton2;
};