#pragma once

#include "Angel.h"
#include "Team.h"

#include <memory>
#include <vector>
#include <string>

struct SGameLevelConfig
{
    struct SSpawner
    {
        float m_spawnAngle;
        Vec2i m_spawnPoint;
        float m_spawnTime;
    };  

    struct SHeadQuarterConfig
    {
        bool m_isNull;
        std::string m_type;
        Vec2i m_position;
    };

    struct SMineSpawner
    {
        std::vector<Vec2i> m_path;
        SSpawner m_spawner;
        std::string m_mine;
        float m_detectionRadius;
    };

    struct SPlayerConfig
    {
        bool m_isNull;
        std::string m_tank;
        SSpawner m_spawner;
    };

    struct STowerConfig
    {
        std::string m_tower;
        Vec2i m_position;
        float m_angle;
    };

    struct SShopConfig
    {
        std::string m_shop;
        float m_detectionRadius;
        Vec2i m_position;
        bool m_isNull;
    };

    struct STeamConfig
    {
        ETeam m_team;
        Color m_color;
        SHeadQuarterConfig m_headQuarter;
        std::vector<STowerConfig> m_towers;
        std::vector<SMineSpawner> m_mineSpawners;
        SPlayerConfig m_player;
        SShopConfig m_shop;
    };   

    std::vector<std::shared_ptr<STeamConfig>> m_teams;

    Vec2i m_towerSize;
    Vec2i m_headQuarterSize;

    std::string m_tiledMapFile; 
    float m_income;
};

std::shared_ptr<SGameLevelConfig> LoadGameLevelConfig(std::string const& fileName);
