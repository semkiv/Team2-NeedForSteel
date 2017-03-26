#include "stdafx.h"
#include "ControllerEffectManager.h"

constexpr int g_takeDamageVibrationPower = 65000;
constexpr int g_shootVibrationPower = 50000;
constexpr float g_takeDamageVibrationTime = 0.2f;
constexpr float g_shootVibrationTime = 0.1f;

class CPlayerControllerEffect :
    public IPlayerListener
{
public:
    CPlayerControllerEffect(int playerId) :
        m_playerId(playerId)
    {

    }

    virtual void OnTakeDamage() override
    {
        if (theInput.GetPlayerInputBinding(m_playerId).m_type == EInputControllerType::XBOX)
        {
            auto& controller = theControllerManager.GetController(
                theInput.GetPlayerInputBinding(m_playerId).m_controllerId);

            controller.SetLeftVibrationRaw(g_takeDamageVibrationPower);
            controller.SetRightVibrationRaw(g_takeDamageVibrationPower);

            auto func = [&controller]()
            {
                controller.SetLeftVibrationRaw(0);
                controller.SetRightVibrationRaw(0);
            };

            theWorld.AddTimer(std::make_shared<CCallbackTimer<decltype(func)>>(
                g_takeDamageVibrationTime, func));
        }
    }

    virtual void OnShoot() override
    {
        if (theInput.GetPlayerInputBinding(m_playerId).m_type == EInputControllerType::XBOX)
        {
            auto& controller = theControllerManager.GetController(
                theInput.GetPlayerInputBinding(m_playerId).m_controllerId);

            controller.SetLeftVibrationRaw(g_shootVibrationPower);
            controller.SetRightVibrationRaw(g_shootVibrationPower);

            auto func = [&controller]()
            {
                controller.SetLeftVibrationRaw(0);
                controller.SetRightVibrationRaw(0);
            };

            theWorld.AddTimer(std::make_shared<CCallbackTimer<decltype(func)>>(
                g_shootVibrationTime, func));
        }
    }

private:
    size_t m_playerId;
};

void CControllerEffectManager::AddControllerEffectsToPlayer(size_t playerId, std::shared_ptr<CPlayer> player)
{
    auto effect = std::make_shared<CPlayerControllerEffect>(playerId);
    player->AddPlayerListener(effect);
    m_playersListener.emplace_back(effect);
}
