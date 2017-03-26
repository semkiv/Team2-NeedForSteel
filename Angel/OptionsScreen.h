#pragma once
#include "Screen.h"

class COptionsScreen : 
	public CMenuScreen
{
public:
    COptionsScreen(IEnumScreenStack* stack);

	virtual void Update(float dt) override;
	virtual void Enter() override;

	virtual ~COptionsScreen() = default;
};
