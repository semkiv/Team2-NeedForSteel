#include "stdafx.h"
#include "Weapon.h"
#include "Projectile.h"

float CRangeWeapon::GetCoolDown() const
{
	return m_coolDown;
}

void CRangeWeapon::SetCoolDown(float time)
{
	m_coolDown = time;
}

void CRangeWeapon::Reload()
{
    m_currentCoolDown = m_coolDown;
}

void CRangeWeapon::CustomReset()
{
    m_currentCoolDown = 0.0f;
}

bool CRangeWeapon::IsReloaded()
{
	return m_currentCoolDown <= 0.0f;
}

void CRangeWeapon::Update(float dt)
{
	if (!IsReloaded())
	{
        m_currentCoolDown -= dt;
	}	
}
