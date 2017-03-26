#pragma once

#include "TiledMap.h"
#include "Wall.h"
#include "Interfaces.h"
#include <memory>
#include <vector>
#include <string>

class CGameMap :
    public IPathFinder
{
public:
    CGameMap(std::string const& fileName);
    virtual ~CGameMap();

    std::shared_ptr<CTiledMap> GetTiledMap() const;

    virtual CPath FindPath(Vector2 from, Vector2 to) override;
    CPath GetPath(std::vector<Vec2i> const& vec) const;

    void Render();

private:
    enum class EObstacle
    {
        BARRICADE = 1,
        WALL
    };

    std::vector<std::shared_ptr<CObstacle>> m_walls;
    std::vector<std::shared_ptr<Actor>> m_wallsSprites;
    std::shared_ptr<CTiledMap> m_tiledMap;
    
    std::shared_ptr<Actor> m_background;

private:
    void AddSprites(std::string const& sprite, STiledBlock block);

};
