#pragma once
#include <stdint.h>

enum class ETag
{
    PROJECTILE,
    DETECTOR,
    TARGETABLE,
    /*HEALABLE,*/
    INVISIBLE,
    DETECTABLE
};

enum class ELayer
{
    BACKGROUND,
    DEFAULT,
    WALL,
    PROJECTILE,
    UNIT,
    TOWER,
    TURRET,
    BASE,
    VISUAL_EFFECT,
    GAME_UI,
    UI   
};
