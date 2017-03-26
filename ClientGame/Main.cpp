#include "stdafx.h"
#include "GameManager.h"
#include "TiledMap.h"

int main()
{
    theWorld.SetShowFps(false);
    theWorld.Initialize(1920, 1080, "Tanks", true, true);
    theWorld.SetupPhysics(Vector2::Zero);
    theWorld.SetGameManager(new CGameManager{});
    theWorld.StartGame();
    theWorld.Destroy();

    return EXIT_SUCCESS;
}
