#pragma once
#include "Angel.h"
#include "Common.h"
#include "Player.h"
#include "Unit.h"
#include "Factory.h"
#include "Level.h"
#include "Team.h"
#include "AIManager.h"
#include "PhysicsManager.h"
#include "LifeTimeManager.h"
#include "Spawner.h"
#include "GameLevelConfig.h"
#include "GameUI.h"
#include "Shop.h"
#include "ControllerEffectManager.h"

#include <memory>

#define theGame CGame::GetInstance()

class CGame
{
public:
    static CGame& GetInstance();

public:
    void StartGame();
    void StopGame();
    bool IsPaused() const;
    bool GameOver() const;
    bool GameStopped() const;
    void SetPause(bool pause);
    void Update(float dt);
    void Render();   

    void SetLevel(std::string const& levelName);
    std::shared_ptr<CGameMap> GetMap() const;
    std::shared_ptr<CPhysicsManager> GetPhysicsManager() const;
    std::shared_ptr<CGameUI> GetGameUI() const;

    template <
        typename T,
        typename = std::enable_if_t<
        std::is_base_of<CPhysicsUnit, T>::value>
    >
    std::shared_ptr<T> CreateUnit(std::string const& configFile);

    void AddUnit(std::shared_ptr<CPhysicsUnit> unit);
    std::shared_ptr<CPlayer> GetPlayer(int playerId);

private:
    static CGame* ms_instance;

private:
    std::shared_ptr<SGameLevelConfig> m_levelConfig;
    std::shared_ptr<CGameMap> m_gameMap;
    std::shared_ptr<CLifeTimeManager> m_lifeTimeManager;
    std::shared_ptr<CPhysicsManager> m_physicsManager;
    std::shared_ptr<CTeamManager> m_teamManager;
    std::shared_ptr<CPlayerManager> m_playerManager;    
    std::shared_ptr<CAIManager> m_aiManager;    
    std::shared_ptr<CSpawnManager> m_spawnerManager;    
    std::shared_ptr<CGameUI> m_gameUI;    
    std::shared_ptr<CControllerEffectManager> m_controllerEffectManager;

    float m_incomeValue;

    bool m_pause = false;
    bool m_gameOver = false;
    bool m_gameStopped = false;

    std::string m_levelName;

private:
    void InitializeTeams();
    void InitializeMap();
    void InitializePlayers();
    void InitializeCreeps();
    void InitializeAI();
    void InitializeGameUI();

    void InitializeTeamCreeps(std::shared_ptr<CTeam> team, SGameLevelConfig::SMineSpawner const& mineSpawner);

    void AddTower(std::shared_ptr<CTeam> team, SGameLevelConfig::STowerConfig const& tower);

    void ListenPlayersInput();
    void StopListenPlayersInput();

    void UpdateOnDestroy();
    void UpdateTeams(float dt);
    void UpdateAI(float dt);    
    void UpdateSpawners(float dt);   
    void UpdateIncome(float dt);

    std::shared_ptr<CPlayer> CreatePlayer(std::shared_ptr<CTeam> team, SGameLevelConfig::SPlayerConfig const& config);
    std::shared_ptr<CTeam> CreateTeam(SGameLevelConfig::STeamConfig const& teamConfig);
    void OnHeadQuarterFallen(ETeam team);

    void LoadLevel();
};

template<typename T, typename>
std::shared_ptr<T> CGame::CreateUnit(std::string const& configFile)
{
    auto unit = theFactory.CreateFromLua<T>(configFile);
    AddUnit(unit);
    return unit;
}
