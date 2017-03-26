teams = {
    {
        team = "RED",
        color = { 1.0, 0.7, 0.7, 1.0 },
        headQuarter = {
            type = "Config/ActorDef/BasicHQ.lua",
            position = { 1, 1 }
        },        
        mineSpawners = {
            {
                mine = "Config/ActorDef/BasicMine.lua",
                detectionRadius = 3.0,
                path = {
                    { 2, 18 },
                    { 3, 25 },
                    { 12, 27 },
                    { 16, 30 }
                },
                spawner = {
                    spawnPoint = { 2, 11 },
                    spawnAngle = 0.0,
                    spawnTime = 3.0
                }
            },
            {
                mine = "Config/ActorDef/BasicMine.lua",
                detectionRadius = 3.0,
                path = {
                    { 14, 6 },
                    { 15, 13 },
                    { 13, 20 },
                    { 12, 25 },
                    { 16, 30 }
                },
                spawner = {
                    spawnPoint = { 4, 4 },
                    spawnAngle = 90.0,
                    spawnTime = 4.5
                }                
            }
        },
        towers = {
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 1, 7 },
                angle = 0.0
            },
            {
                tower = "Config/ActorDef/MainTower.lua",
                position = { 5, 1 },
                angle = 0.0
            },
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 14, 1 },
                angle = 0.0
            }
        },
        player = {
            spawner = {
                spawnPoint = { 2, 6 },
                spawnAngle = 90.0,
                spawnTime = 10.0
            },
            tank = "Config/ActorDef/BasicTank.lua"
        },
        shop = {
            shop = "Config/UpgradesConfig.lua",
            position = { 2, 2 },
            detectionRadius = 5.0
        }
    },
    {
        team = "BLUE",
        color = { 0.7, 1.0, 0.7, 1.0 },
        headQuarter = {
            type = "Config/ActorDef/BasicHQ.lua",
            position = { 14, 28 }
        },   
        mineSpawners = {
            {
                mine = "Config/ActorDef/BasicMine.lua",
                detectionRadius = 3.0,
                path = {
                    { 15, 13 },
                    { 14, 6 },
                    { 5, 4 },
                    { 1, 1 }
                },
                spawner = {
                    spawnPoint = { 15, 20 },
                    spawnAngle = 180.0,
                    spawnTime = 3.0
                }
            },
            {
                mine = "Config/ActorDef/BasicMine.lua",
                detectionRadius = 3.0,
                path = {
                    { 3, 25 },
                    { 2, 18 },
                    { 4, 11 },
                    { 5, 6 },
                    { 1, 1 }
                },
                spawner = {
                    spawnPoint = { 13, 27 },
                    spawnAngle = -90.0,
                    spawnTime = 4.5
                }
            }
        },
        towers = {
            {
                tower = "Config/ActorDef/MainTower.lua",
                position = { 11, 29 },
                angle = 180.0
            },
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 15, 23 },
                angle = 180.0
            },
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 2, 29 },
                angle = 180.0
            }
        }, 
        player = {
            spawner = {
                spawnPoint = { 15, 25 },
                spawnAngle = -90.0,
                spawnTime = 10.0
            },
            tank = "Config/ActorDef/BasicTank.lua"
        },
        shop = {
            shop = "Config/UpgradesConfig.lua",
            position = { 15, 29 },
            detectionRadius = 5.0
        }
    },
    {
        team = "NEUTRAL",
        color = { 0.5, 0.5, 0.5, 1.0 },
        --[[mineSpawners = {
            {
                mine = "Config/ActorDef/BasicMine.lua",
                detectionRadius = 2.5,
                path = {
                    { 10, 11 },
                    { 5, 10 },
                    { 2, 2 }
                },
                spawner = {
                    spawnPoint = { 11, 9 },
                    spawnAngle = 0.0,
                    spawnTime = 10.0
                }
            },
            {
                mine = "Config/ActorDef/BasicMine.lua",
                detectionRadius = 2.5,
                path = {
                    { 7, 20 },
                    { 12, 21 },
                    { 15, 29 }
                },
                spawner = {
                    spawnPoint = { 6, 22 },
                    spawnAngle = 180.0,
                    spawnTime = 10.0
                }
            }
        }]]--
    }
}
towerSize = {2, 2}
headQuarterSize = {3, 3}
tiledMapFile = "Config/Level/01.tmx"
income = 0.5