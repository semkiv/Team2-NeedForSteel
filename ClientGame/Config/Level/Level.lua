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
                path = {
                    { 2, 13 },
                    { 2, 21 },
                    { 8, 26 },
                    { 13, 27 },
                    { 16, 30 }
                },
                spawner = {
                    spawnPoint = { 2, 13 },
                    spawnAngle = 0.0,
                    spawnTime = 5.0
                }
            }
        },
        towers = {
            {
                tower = "Config/ActorDef/MainTower.lua",
                position = { 1, 9 },
                angle = 0.0
            },
            {
                tower = "Config/ActorDef/MainTower.lua",
                position = { 5, 1 },
                angle = 0.0
            },
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 14, 6 },
                angle = 0.0
            }
        },
        player = {
            spawner = {
                spawnPoint = { 2, 6 },
                spawnAngle = 90.0,
                spawnTime = 15.0
            },
            tank = "Config/ActorDef/BasicTank.lua"
        }
    },
    {
        team = "BLUE",
        color = { 0.7, 1.0, 0.7, 1.0 },
        headQuarter = {
            type = "Config/ActorDef/BasicHQ.lua",
            position = { 15, 29 }
        },   
        mineSpawners = {
            {
            mine = "Config/ActorDef/BasicMine.lua",
                path = {
                    { 15, 18 },
                    { 15, 10 },
                    { 9, 5 },
                    { 4, 4 },
                    { 1, 1 }
                },
                spawner = {
                    spawnPoint = { 15, 18 },
                    spawnAngle = 180.0,
                    spawnTime = 5.0
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
                tower = "Config/ActorDef/MainTower.lua",
                position = { 15, 21 },
                angle = 180.0
            },
            {
                tower = "Config/ActorDef/BasicTower.lua",
                position = { 2, 24 },
                angle = 180.0
            }
        }, 
        player = {
            spawner = {
                spawnPoint = { 15, 25 },
                spawnAngle = -90.0,
                spawnTime = 15.0
            },
            tank = "Config/ActorDef/BasicTank.lua"
        } 
    }
}
towerSize = {2, 2}
headQuarterSize = {2, 2}
tiledMapFile = "Config/Level/01.tmx"