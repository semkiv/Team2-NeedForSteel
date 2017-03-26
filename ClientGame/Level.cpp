#include "stdafx.h"
#include "Level.h"
#include "Factory.h"
#include "TMXParser.h"

CGameMap::CGameMap(std::string const& fileName)
{
    CTMXParser parser(fileName);

    m_background = std::make_shared<Actor>();
    theWorld.Add(m_background.get());
    m_background->SetSprite(parser.GetBackground());
    m_background->SetLayer(static_cast<int>(ELayer::BACKGROUND));
    m_background->SetSize(MathUtil::GetWorldDimensions());

    auto barricades = parser.GetMask(static_cast<int>(EObstacle::BARRICADE));
    std::reverse(barricades.begin(), barricades.end());
    auto walls = parser.GetMask(static_cast<int>(EObstacle::WALL));
    std::reverse(walls.begin(), walls.end());
    auto barricadeSprite = parser.GetTileTexture(static_cast<int>(EObstacle::BARRICADE));
    auto wallSprite = parser.GetTileTexture(static_cast<int>(EObstacle::WALL));

    assert(barricades.size() > 0 && barricades.front().size() > 0);

    size_t rows = barricades.size();
    size_t columns = barricades.front().size();
    auto barricadesBlocks = UniteBlockByLargest(barricades);
    auto wallsBlocks = UniteBlockByLargest(walls);

    m_tiledMap = CreateFullScreenTiledMap(rows, columns);

    for (auto& block : wallsBlocks)
    {
        auto wall = theFactory.Create<CWall>(ELayer::WALL);
        auto size = m_tiledMap->GetSizeByBlock(block);
        wall->SetSize(size.X, size.Y);
        wall->SetDrawShape(ADS_Square);
        wall->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
        wall->SetPosition(m_tiledMap->GetCenterPositionByBlock(block));
        wall->SetDensity(0.0f);
        wall->SetAlpha(0.0f);
        wall->InitPhysics();

        m_walls.push_back(wall);

        m_tiledMap->SetWall(block);
        AddSprites(wallSprite, block);
    }

    for (auto& block : barricadesBlocks)
    {
        auto wall = theFactory.Create<CBarricade>(ELayer::WALL);
        auto size = m_tiledMap->GetSizeByBlock(block);
        wall->SetSize(size.X, size.Y);
        wall->SetDrawShape(ADS_Square);
        wall->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
        wall->SetPosition(m_tiledMap->GetCenterPositionByBlock(block));
        wall->SetDensity(0.0f);
        wall->SetAlpha(0.0f);
        wall->InitPhysics();

        m_walls.push_back(wall);

        m_tiledMap->SetWall(block);
        AddSprites(barricadeSprite, block);
    }
}

CGameMap::~CGameMap()
{
    theWorld.Remove(m_background.get());
}

std::shared_ptr<CTiledMap> CGameMap::GetTiledMap() const
{
    return m_tiledMap;
}

CPath CGameMap::FindPath(Vector2 from, Vector2 to)
{
    STiledMapPosition fromTiled = m_tiledMap->GetRCByPosition(from);
    STiledMapPosition toTiled = m_tiledMap->GetRCByPosition(to);

    CPath path;
    auto tiledPath = m_tiledMap->FindPath(fromTiled, toTiled);
    path.reserve(path.size());

    for (auto& tiledPos : tiledPath)
    {
        path.emplace_back(m_tiledMap->GetPositionByRC(tiledPos));
    }

    return path;
}

CPath CGameMap::GetPath(std::vector<Vec2i> const& vec) const
{
    CPath path;

    for (auto& tiledPos : vec)
    {
        path.emplace_back(m_tiledMap->GetPositionByRC(tiledPos.X, tiledPos.Y));
    }
    
    return path;
}

void CGameMap::Render()
{
    m_tiledMap->DrawTiledMap();
}

void CGameMap::AddSprites(std::string const& sprite, STiledBlock block)
{
    for (size_t c = block.m_left; c < block.m_right; ++c)
    {
        for (size_t r = block.m_bottom; r < block.m_top; ++r)
        {
            auto wallSprite = theFactory.Create<Actor>(ELayer::WALL);
            wallSprite->SetSprite(sprite);
            wallSprite->SetSize(m_tiledMap->CellWorldSize(), m_tiledMap->CellWorldSize());
            wallSprite->SetPosition(m_tiledMap->GetPositionByRC(r, c));
            m_wallsSprites.emplace_back(wallSprite);
        }
    }
}
