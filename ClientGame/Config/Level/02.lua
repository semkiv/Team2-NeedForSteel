teams = {
    {
        team = "RED",
        color = { 1.0, 0.7, 0.7, 1.0 },
        headQuarter = {
            type = "Config/ActorDef/BasicHQ.lua",
            position = { 7, 6 }
        },        
        mineSpawners = {
            {
                mine = "Config/ActorDef/BasicMine.lua",
                detectionRadius = 4.0,
                path = {
                    { 12, 7 },
                    { 12, 16 },
                    { 14, 20 },
                    { 12, 27 },
					{ 9, 24 }
                },
                spawner = {
                    spawnPoint = { 15, 5 },
                    spawnAngle = 0.0,
                    spawnTime = 5.0
                }
            }
        },
        towers = {
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 15, 8 },
                angle = 0.0
            },
            {
                tower = "Config/ActorDef/MainTower.lua",
                position = { 1, 1 },
                angle = 0.0
            }
        },
        player = {
            spawner = {
                spawnPoint = { 15, 1 },
                spawnAngle = -90.0,
                spawnTime = 5.0
            },
            tank = "Config/ActorDef/BasicTank.lua"
        },
        shop = {
            shop = "Config/UpgradesConfig.lua",
            position = { 7, 7 },
            detectionRadius = 5.0
        }
    },
    {
        team = "BLUE",
        color = { 0.7, 1.0, 0.7, 1.0 },
        headQuarter = {
            type = "Config/ActorDef/BasicHQ.lua",
            position = { 8, 23 }
        },        
        mineSpawners = {
            {
                mine = "Config/ActorDef/BasicMine.lua",
                detectionRadius = 4.0,
                path = {
                    { 5, 24 },
                    { 5, 15 },
                    { 3, 11 },
                    { 5, 4 },
					{ 8, 7 }
                },
                spawner = {
                    spawnPoint = { 2, 26 },
                    spawnAngle = 0.0,
                    spawnTime = 5.0
                }
            }
        },
        towers = {
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 1, 22 },
                angle = 180.0
            },
            {
                tower = "Config/ActorDef/MainTower.lua",
                position = { 15, 29 },
                angle = 180.0
            }
        },
        player = {
            spawner = {
                spawnPoint = { 2, 30 },
                spawnAngle = 90.0,
                spawnTime = 5.0
            },
            tank = "Config/ActorDef/BasicTank.lua"
        },
        shop = {
            shop = "Config/UpgradesConfig.lua",
            position = { 8, 24 },
            detectionRadius = 5.0
        }
    }--[[,
    {
        team = "NEUTRAL",
        color = { 0.9, 0.9, 0.9, 1.0 },
        towers = {
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 8, 15 },
                angle = 90.0
            }
        },
    }--]]
}
towerSize = {2, 2}
headQuarterSize = {3, 3}
tiledMapFile = "Config/Level/02.tmx"
income = 0.5