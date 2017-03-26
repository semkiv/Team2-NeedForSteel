#include "stdafx.h"
#include "Player.h"
#include "Tools.h"

CPlayer::CPlayer(std::shared_ptr<CTeamShop> shop) :
    m_gold(0),
    m_controlSensitivity(0.05f),
    m_shop(shop)
{
    m_currentUpgradeLevel.assign(shop->GetShop()->UpgradesCount(), 0);
}

CPlayer::~CPlayer()
{
    int l = 0;
}

void CPlayer::SetTank(std::shared_ptr<CTank> tank)
{
    m_tank = tank;
    tank->SetDamagerListener(shared_from_this());
    tank->AddDamageableListener(shared_from_this());
}

std::weak_ptr<CTank> CPlayer::GetTank() const
{
    return m_tank;
}

void CPlayer::SetGold(size_t gold)
{
    m_gold = gold;

    ForeachValidWeak(m_listeners, [gold](std::shared_ptr<IPlayerListener> listener)
    {
        listener->OnGoldChanged(gold);
    });
}

size_t CPlayer::GetGold() const
{
    return m_gold;
}

void CPlayer::OnPlayerKeyPress(SPlayerDigitalInputEvent digitInput, SPlayerInputBinding const& binding)
{
    auto tank = m_tank.lock();

    if (tank == nullptr || tank->IsDestroyed())
    {
        return;
    }

    bool handled = true;
        
    if (digitInput.m_type == EPlayerInputEventType::ROTATE_TURRET_RIGHT)
    {
        tank->HandleTurretRotationSignal(-1.0f);
    }
    else if (digitInput.m_type == EPlayerInputEventType::ROTATE_TURRET_LEFT)
    {
        tank->HandleTurretRotationSignal(1.0f);
    }
    else if (digitInput.m_type == EPlayerInputEventType::SPEED_UP)
    {
        tank->HandleAccelerationSignal(
            (theInput.GetControllerInfo(binding.m_type, binding.m_controllerId).m_inverted) ? (-1.0f) : (1.0f));
    }
    else if (digitInput.m_type == EPlayerInputEventType::SLOW_DOWN)
    {
        tank->HandleAccelerationSignal(
            (theInput.GetControllerInfo(binding.m_type, binding.m_controllerId).m_inverted) ? (1.0f) : (-1.0f));
    }
    else if (digitInput.m_type == EPlayerInputEventType::ROTATE_RIGHT)
    {
        tank->HandleRotationSignal(-1.0f);
    }
    else if (digitInput.m_type == EPlayerInputEventType::ROTATE_LEFT)
    {
        tank->HandleRotationSignal(1.0f);
    }
    else if (digitInput.m_type == EPlayerInputEventType::SHOT)
    {
        TankShoot(1.0f);
    }
    else
    {
        handled = false;
    }
        
    UpdateInputMask(digitInput.m_type, true);
        
    if (!handled)
    {
        auto shop = m_shop.lock();
        if (shop == nullptr)
        {
            return;
        }

        if (digitInput.m_type == EPlayerInputEventType::SHOP_ENTER)
        {
            if (shop->IsActive())
            {
                TryUpgradeTank(static_cast<EUpgrade>(shop->GetSelectedUpgrade()));
            }
        }
        else if (digitInput.m_type == EPlayerInputEventType::SHOP_UP)
        {
            if (shop->IsActive())
            {
                shop->MoveUp();
            }
        }
        else if (digitInput.m_type == EPlayerInputEventType::SHOP_DOWN)
        {
            if (shop->IsActive())
            {
                shop->MoveDown();
            }
        }
    }
}

void CPlayer::OnPlayerKeyRelease(SPlayerDigitalInputEvent digitInput, SPlayerInputBinding const& binding)
{
    auto tank = m_tank.lock();
    if (tank == nullptr || tank->IsDestroyed())
    {
        return;
    }

    if (digitInput.m_type == EPlayerInputEventType::ROTATE_TURRET_RIGHT)
    {
        if (!IsPressed(EPlayerInputEventType::ROTATE_TURRET_LEFT))
        {
            tank->HandleTurretRotationSignal(0.0f);
        }
        else
        {
            tank->HandleTurretRotationSignal(1.0f);
        }
    }
    else if (digitInput.m_type == EPlayerInputEventType::ROTATE_TURRET_LEFT)
    {
        if (!IsPressed(EPlayerInputEventType::ROTATE_TURRET_RIGHT))
        {
            tank->HandleTurretRotationSignal(0.0f);
        }
        else
        {
            tank->HandleTurretRotationSignal(-1.0f);
        }
    }
    else if (digitInput.m_type == EPlayerInputEventType::SPEED_UP)
    {
        if (!IsPressed(EPlayerInputEventType::SLOW_DOWN))
        {
            tank->HandleAccelerationSignal(0.0f);
        }
        else
        {
            tank->HandleAccelerationSignal(-1.0f);
        }
    }
    else if (digitInput.m_type == EPlayerInputEventType::SLOW_DOWN)
    {
        if (!IsPressed(EPlayerInputEventType::SPEED_UP))
        {
            tank->HandleAccelerationSignal(0.0f);
        }
        else
        {
            tank->HandleAccelerationSignal(1.0f);
        }
    }
    else if (digitInput.m_type == EPlayerInputEventType::ROTATE_RIGHT)
    {
        if (!IsPressed(EPlayerInputEventType::ROTATE_LEFT))
        {
            tank->HandleRotationSignal(0.0f);
        }
        else
        {
            tank->HandleRotationSignal(1.0f);
        }
    }
    else if (digitInput.m_type == EPlayerInputEventType::ROTATE_LEFT)
    {
        if (!IsPressed(EPlayerInputEventType::ROTATE_RIGHT))
        {
            tank->HandleRotationSignal(0.0f);
        }
        else
        {
            tank->HandleRotationSignal(-1.0f);
        }
    }
    else if (digitInput.m_type == EPlayerInputEventType::SHOT)
    {
        TankShoot(0.0f);
    }

    UpdateInputMask(digitInput.m_type, false);
}

void CPlayer::OnPlayerAnalogInput(SPlayerAnalogInputEvent analogInput, SPlayerInputBinding const& binding)
{
    auto tank = m_tank.lock();
    if (tank == nullptr || tank->IsDestroyed())
    {
        return;
    }

    if (fabs(analogInput.m_value) <= m_controlSensitivity)
    {
        analogInput.m_value = 0.0f;
    }

    if (analogInput.m_type == EPlayerInputEventType::ROTATE_TURRET_RIGHT)
    {
        tank->HandleTurretRotationSignal(-analogInput.m_value);
    }
    else if (analogInput.m_type == EPlayerInputEventType::ROTATE_TURRET_LEFT)
    {
        tank->HandleTurretRotationSignal(-analogInput.m_value);
    }
    else if (analogInput.m_type == EPlayerInputEventType::SPEED_UP)
    {
        tank->HandleAccelerationSignal(
            ((theInput.GetControllerInfo(binding.m_type, binding.m_controllerId).m_inverted) ? (-1.0f) : (1.0f)) * analogInput.m_value);
    }
    else if (analogInput.m_type == EPlayerInputEventType::SLOW_DOWN)
    {
        tank->HandleAccelerationSignal(
            ((theInput.GetControllerInfo(binding.m_type, binding.m_controllerId).m_inverted) ? (1.0f) : (-1.0f)) * analogInput.m_value);
    }
    else if (analogInput.m_type == EPlayerInputEventType::ROTATE_RIGHT)
    {
        tank->HandleRotationSignal(-analogInput.m_value);
    }
    else if (analogInput.m_type == EPlayerInputEventType::ROTATE_LEFT)
    {
        tank->HandleRotationSignal(-analogInput.m_value);
    }
    else if (analogInput.m_type == EPlayerInputEventType::SHOT)
    {
        TankShoot(analogInput.m_value);
    }
}

void CPlayer::OnKilled(std::weak_ptr<CDamageable> victim)
{
    auto v = victim.lock();
    if (v != nullptr)
    {
        SetGold(m_gold + v->GetWorth());
    }
}

void CPlayer::OnTakeDamage()
{
    ForeachValidWeak(m_listeners, [](std::shared_ptr<IPlayerListener> listener)
    {
        listener->OnTakeDamage();
    });
}

void CPlayer::AddPlayerListener(std::weak_ptr<IPlayerListener> listener)
{
    m_listeners.emplace_back(listener);
}

void CPlayer::TankShoot(float signal)
{
    auto tank = m_tank.lock();
    
    if (tank->GetTurret()->IsReloaded())
    {
        tank->HandleShootSignal(signal);

        if (signal > MathUtil::Epsilon)
        {
            ForeachValidWeak(m_listeners, [](std::shared_ptr<IPlayerListener> listener)
            {
                listener->OnShoot();
            });
        }
    }
}

bool CPlayer::IsPressed(EPlayerInputEventType input)
{
    return (m_inputMask & (static_cast<MaskType>(1) << static_cast<MaskType>(input))) != 0;
}

void CPlayer::UpdateInputMask(EPlayerInputEventType input, bool pressed)
{
    m_inputMask &= ~(static_cast<MaskType>(1) << static_cast<MaskType>(input));
    m_inputMask |= static_cast<MaskType>((pressed) ? (1) : (0)) << static_cast<MaskType>(input);
}

void CPlayer::TryUpgradeTank(EUpgrade upgrade)
{
    size_t index = static_cast<size_t>(upgrade);
    auto shop = m_shop.lock()->GetShop();
    size_t price = 0;

    if (shop->CanUpgrade(upgrade, m_currentUpgradeLevel[index]) 
        && (price = shop->UpgradePrice(upgrade, m_currentUpgradeLevel[index])) <= m_gold)
    {
        SetGold(m_gold - price);
        UpgradeTank(upgrade);
    }
}

void CPlayer::UpgradeTank(EUpgrade upgrade)
{
    auto tank = m_tank.lock();
    size_t index = static_cast<size_t>(upgrade);
    auto shop = m_shop.lock()->GetShop();
    float value = shop->Upgrade(upgrade, m_currentUpgradeLevel[index]);

    switch (upgrade)
    {
    case EUpgrade::HP:
        tank->SetMaxHealth(value + tank->GetMaxHealth());
        tank->Heal(value);
        break;
    case EUpgrade::DAMAGE:
        tank->GetTurret()->SetDamage(tank->GetTurret()->GetDamage() + value);
        break;
    case EUpgrade::COOLDOWN:
        tank->GetTurret()->SetCoolDown(tank->GetTurret()->GetCoolDown() + value);
        break;
    default:
        break;
    }

    m_shop.lock()->Upgrade();
    ++m_currentUpgradeLevel[index];
}

CPlayerManager::~CPlayerManager()
{
    m_players.clear();
    int l = 0;
}

size_t CPlayerManager::PlayersCount() const
{
    return m_players.size();
}

void CPlayerManager::AddPlayer(std::shared_ptr<CPlayer> player)
{
    m_players.push_back(player);
}

std::shared_ptr<CPlayer> CPlayerManager::GetPlayer(size_t playerId) const
{
    return m_players[playerId];
}
