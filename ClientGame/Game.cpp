#include "stdafx.h"
#include "Game.h"
#include "Interfaces.h"
#include "PhysicsUnit.h"
#include "Tank.h"
#include "Tower.h"
#include "TiledMap.h"
#include "AITargetDriver.h"
#include "HeadQuarter.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "Mine.h"
#include "SpawnerUI.h"

CGame* CGame::ms_instance;

CGame& CGame::GetInstance()
{
    return *ms_instance;
}

void CGame::StartGame()
{
    ms_instance = this;

    m_gameOver = false;
    m_pause = false;
    m_gameStopped = false;

    theWorld.SetShowFps(false);
    theWorld.GetPhysicsWorld().SetAllowSleeping(false);

    m_physicsManager = std::make_shared<CPhysicsManager>();
    m_lifeTimeManager = std::make_shared<CLifeTimeManager>();
    m_spawnerManager = std::make_shared<CSpawnManager>();

    LoadLevel();

    InitializeMap();
    InitializeTeams();    
    InitializePlayers();
    InitializeAI();
    InitializeCreeps();
    InitializeGameUI();
}

void CGame::StopGame()
{    
    m_gameStopped = true;

    if (m_pause)
    {
        theWorld.ResumeSimulation();
    }
    
    theSoundManager.Stop();
    theEffectManager.Stop();
}

bool CGame::IsPaused() const
{
    return m_pause;
}

bool CGame::GameOver() const
{
    return m_gameOver;
}

bool CGame::GameStopped() const
{
    return m_gameStopped;
}

void CGame::SetPause(bool pause)
{
    if (!m_pause && pause)
    {
        StopListenPlayersInput();
        theWorld.PauseSimulation();
        theSoundManager.Pause(pause);
        m_pause = true;
    }
    else if(m_pause && !pause)
    {       
        m_pause = false;
        theSoundManager.Pause(pause);
        theWorld.ResumeSimulation();
        ListenPlayersInput();        
    }
}

void CGame::Update(float dt)
{
    UpdateTeams(dt);
    UpdateAI(dt);
    UpdateSpawners(dt);
    UpdateOnDestroy();
    UpdateIncome(dt);    

	theEventManager.Update();
}

void CGame::Render()
{
//    m_gameMap->Render();
//    m_aiManager->DrawPath();
}

void CGame::AddUnit(std::shared_ptr<CPhysicsUnit> unit)
{
    m_lifeTimeManager->ControlLifeTime(unit);
    m_physicsManager->AddPhysicsUnit(unit);
}

std::shared_ptr<CPlayer> CGame::GetPlayer(int playerId)
{
    return m_playerManager->GetPlayer(playerId);
}

void CGame::SetLevel(std::string const& levelName)
{
    m_levelName = levelName;
}

std::shared_ptr<CGameMap> CGame::GetMap() const
{
    return m_gameMap;
}

std::shared_ptr<CPhysicsManager> CGame::GetPhysicsManager() const
{
    return m_physicsManager;
}

std::shared_ptr<CGameUI> CGame::GetGameUI() const
{
    return m_gameUI;
}

void CGame::InitializeTeams()
{
    m_teamManager = std::make_shared<CTeamManager>();

    for (size_t i = 0; i < m_levelConfig->m_teams.size(); ++i)
    {
        m_teamManager->AddTeam(CreateTeam(*m_levelConfig->m_teams[i]));
    }   

    for (size_t i = 0; i < m_levelConfig->m_teams.size(); ++i)
    {
        for (size_t j = i + 1; j < m_levelConfig->m_teams.size(); ++j)
        {
            m_teamManager->GetTeam(i)->AddEnemyTeam(
                m_teamManager->GetTeam(j));
            m_teamManager->GetTeam(j)->AddEnemyTeam(
                m_teamManager->GetTeam(i));
        }
    }
}

std::shared_ptr<CTeam> CGame::CreateTeam(SGameLevelConfig::STeamConfig const& teamConfig)
{
    auto team = std::make_shared<CTeam>(teamConfig.m_team);  
    team->SetTeamColor(teamConfig.m_color);

    if (!teamConfig.m_headQuarter.m_isNull)
    {
        STiledBlock hqBlock;
        hqBlock.m_left = teamConfig.m_headQuarter.m_position.Y;
        hqBlock.m_right = hqBlock.m_left + m_levelConfig->m_headQuarterSize.Y;
        hqBlock.m_bottom = teamConfig.m_headQuarter.m_position.X;
        hqBlock.m_top = hqBlock.m_bottom + m_levelConfig->m_headQuarterSize.X;

        auto hq = CreateUnit<CHeadQuarter>(teamConfig.m_headQuarter.m_type);
        auto size = m_gameMap->GetTiledMap()->GetSizeByBlock(hqBlock);
        hq->SetSize(size.X, size.Y);
        hq->SetChildrenPositions(
            m_gameMap->GetTiledMap()->GetCenterPositionByBlock(
                hqBlock));
        hq->GetDetector()->InitPhysics();
        hq->InitPhysics();
        team->AddTeamMember(hq);

        auto func = [this, team = team->GetTeamType()]()
        {
            OnHeadQuarterFallen(team);
        };

        auto listener = std::make_shared<CRenderableTypedListener<decltype(func)>>(func);
        hq->SetListener(listener);
    }

    if (!teamConfig.m_shop.m_isNull)
    {
        std::shared_ptr<CShop> shop = std::make_shared<CShop>(teamConfig.m_shop.m_shop);
        std::shared_ptr<CTeamShop> teamShop = std::make_shared<CTeamShop>(
            shop, 
            m_gameMap->GetTiledMap()->GetPositionByRC(
                teamConfig.m_shop.m_position.X,
                teamConfig.m_shop.m_position.Y), 
            teamConfig.m_shop.m_detectionRadius);
        team->SetTeamShop(teamShop);
    }

    for (auto const& towerConfig : teamConfig.m_towers)
    {
        AddTower(team, towerConfig);
    }

    return team;
}

void CGame::InitializeMap()
{
    m_gameMap = std::make_shared<CGameMap>(m_levelConfig->m_tiledMapFile);
}

void CGame::InitializePlayers()
{
    m_incomeValue = m_levelConfig->m_income;
    m_playerManager = std::make_shared<CPlayerManager>();
    m_controllerEffectManager = std::make_shared<CControllerEffectManager>();

    size_t playerCnt = 0;

    for (auto& teamConfig : m_levelConfig->m_teams)
    {
        playerCnt += ((teamConfig->m_player.m_isNull) ? (0) : (1));
    }

    m_gameUI = std::make_shared<CGameUI>(playerCnt);

    for (auto& teamConfig : m_levelConfig->m_teams)
    {
        if (!teamConfig->m_player.m_isNull)
        {
            std::shared_ptr<CPlayer> m_player;
            m_playerManager->AddPlayer(
                m_player = CreatePlayer(m_teamManager->GetTeam(teamConfig->m_team), teamConfig->m_player));
        
            m_controllerEffectManager->AddControllerEffectsToPlayer(
                static_cast<size_t>(teamConfig->m_team), m_player);
        }
    }   
}

void CGame::InitializeCreeps()
{
    for(auto& team: m_levelConfig->m_teams)
    {
        for (auto& spawner : team->m_mineSpawners)
        {
            InitializeTeamCreeps(
                m_teamManager->GetTeam(team->m_team),
                spawner);
        }
    }
}

void CGame::InitializeAI()
{
    m_aiManager = std::make_shared<CAIManager>();
    m_aiManager->SetUpdatePathTime(0.5f); 
    m_aiManager->SetUpdateTargetTime(0.5f);
}

void CGame::InitializeGameUI()
{
    size_t playerCnt = m_playerManager->PlayersCount();    

    for (size_t i = 0; i < playerCnt; ++i)
    {
        auto player = m_playerManager->GetPlayer(i);
        player->AddPlayerListener(
            m_gameUI->GetPlayerUI(i));
    }

    for (size_t i = 0; i < playerCnt; ++i)
    {
        m_teamManager->GetTeam(i)->GetTeamShop()->SetShopUI(
            m_gameUI->GetShopUI(i));
    }
}

void CGame::InitializeTeamCreeps(std::shared_ptr<CTeam> team, SGameLevelConfig::SMineSpawner const& mineSpawner)
{
    auto spawnFunc = [this, team, &mineSpawner](CPositionalSpawner& spawner)
    {
        auto mine = CreateUnit<CMine>(mineSpawner.m_mine.c_str());
        mine->SetPosition(spawner.GetSpawnPosition());
        mine->SetRotation(spawner.GetSpawnRotatoin());
        mine->InitPhysics();

        team->AddTeamMember(mine);

        auto ai = std::make_shared<CAITargetDriver>();
        ai->SetUnit(mine);
        ai->SetDefaultPath(m_gameMap->GetPath(mineSpawner.m_path));
        ai->SetPathFinder(m_gameMap);

        auto targetUpdateAi = std::make_shared<CAIUpdateTarget>(
            ai, mine, m_physicsManager);
        targetUpdateAi->SetRadius(mineSpawner.m_detectionRadius);

        m_aiManager->AddAI(targetUpdateAi);
        m_aiManager->AddUpdateTargetAI(targetUpdateAi);

        m_aiManager->AddAI(ai);
        m_aiManager->AddUpdatePathAI(ai);
    };

    auto spawner = std::make_shared<CFunctionalSpawner<decltype(spawnFunc)>>(spawnFunc);
    spawner->SetSpawnTime(mineSpawner.m_spawner.m_spawnTime);
    spawner->SetSpawnPosition(
        m_gameMap->GetTiledMap()->GetPositionByRC(
            mineSpawner.m_spawner.m_spawnPoint.X,
            mineSpawner.m_spawner.m_spawnPoint.Y));
    spawner->SetSpawnRotation(mineSpawner.m_spawner.m_spawnAngle);
    spawner->Spawn();
    m_spawnerManager->AddSpawner(spawner);
}

void CGame::AddTower(std::shared_ptr<CTeam> team, SGameLevelConfig::STowerConfig const& towerConfig)
{
    auto tower = CreateUnit<CTower>(towerConfig.m_tower);

    tower->SetLayer(static_cast<int>(ELayer::TOWER));
    tower->GetTurret()->SetLayer(static_cast<int>(ELayer::TURRET));    

    team->AddTeamMember(tower);

    STiledBlock block;
    block.m_left = towerConfig.m_position.Y;
    block.m_right = towerConfig.m_position.Y + m_levelConfig->m_towerSize.Y;
    block.m_bottom = towerConfig.m_position.X;
    block.m_top = towerConfig.m_position.X + m_levelConfig->m_towerSize.X;

    tower->SetChildrenPositions(
        m_gameMap->GetTiledMap()->GetCenterPositionByBlock(block));
    tower->SetChildrenRotations(towerConfig.m_angle);

    tower->InitPhysics();
    tower->GetDetector()->InitPhysics();
}

void CGame::ListenPlayersInput()
{
    int playerCnt = static_cast<int>(m_playerManager->PlayersCount());
    for (int i = 0; i < playerCnt; ++i)
    {
        theInput.AddPlayerListener(i, m_playerManager->GetPlayer(i));
    }
}

void CGame::StopListenPlayersInput()
{
    int playerCnt = static_cast<int>(m_playerManager->PlayersCount());
    for (int i = 0; i < playerCnt; ++i)
    {
        theInput.ClearPlayerListeners(i);
    }
}

void CGame::UpdateOnDestroy()
{
    m_lifeTimeManager->Update();
}

void CGame::UpdateTeams(float dt)
{
    m_teamManager->Update(dt);
}

void CGame::UpdateAI(float dt)
{
    m_aiManager->Update(dt);
}

void CGame::UpdateSpawners(float dt)
{
    m_spawnerManager->Update(dt);
}

void CGame::UpdateIncome(float dt)
{
    static float m_currentIncome = 0.0f;
    m_currentIncome += m_incomeValue * dt;

    if (m_currentIncome >= 1.0f)
    {
        size_t income = m_currentIncome;
        size_t playerCnt = m_playerManager->PlayersCount();

        for (size_t i = 0; i < playerCnt; ++i)
        {
            m_playerManager->GetPlayer(i)->SetGold(
                m_playerManager->GetPlayer(i)->GetGold() + income);
        }

        m_currentIncome -= income;
    }
}

std::shared_ptr<CPlayer> CGame::CreatePlayer(std::shared_ptr<CTeam> team, SGameLevelConfig::SPlayerConfig const& config)
{
    size_t playerId = static_cast<size_t>(team->GetTeamType());
    auto player = std::make_shared<CPlayer>(team->GetTeamShop());
    std::shared_ptr<CTank> tank = nullptr;

    Vector2 position = m_gameMap->GetTiledMap()->GetPositionByRC(
        config.m_spawner.m_spawnPoint.X, config.m_spawner.m_spawnPoint.Y);
    float angle = config.m_spawner.m_spawnAngle;

    tank = theFactory.CreateFromLua<CTank>(config.m_tank.c_str());
    player->SetTank(tank);
    tank->SetChildrenPositions(position);
    tank->SetChildrenRotations(angle);
    tank->InitPhysics();

    auto spawner = std::make_shared<CPhysicsUnitCooldownSpawner>();
    spawner->SetCooldown(config.m_spawner.m_spawnTime);
    spawner->SetUnit(tank);
    spawner->SetSpawnPosition(position);
    spawner->SetSpawnRotation(angle);

    m_spawnerManager->AddSpawner(spawner);
    team->AddTeamMember(tank);
    m_physicsManager->AddPhysicsUnit(tank);    

    theInput.AddPlayerListener(playerId, player);

    if (team->GetTeamShop() != nullptr)
    {
        team->GetTeamShop()->SetDetector(tank);
    }

    float tileSize = m_gameMap->GetTiledMap()->CellWorldSize();
    auto spawnerUI = std::make_shared<CSpawnerUI>(
        tank->GetPosition(),
        team->GetTeamColor(),
        Vector2{ tileSize, tileSize });
    spawner->SetCooldownListener(spawnerUI);
    m_gameUI->AddSpawnerUI(spawnerUI);

    return player;
}

void CGame::OnHeadQuarterFallen(ETeam team)
{
    if (!m_pause && !m_gameOver)
    {
        m_gameOver = true;
        SetPause(true);

        m_gameUI->ShowVictoryWindow(static_cast<size_t>(team));
    }    
} 

void CGame::LoadLevel()
{
    m_levelConfig = LoadGameLevelConfig(m_levelName.c_str());
}
