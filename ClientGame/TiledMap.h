#pragma once

#include "Angel.h"
#include "TiledMapUnit.h"
#include <vector>
#include <memory>

using STiledMapPosition = std::pair<size_t, size_t>;
using CTiledMapPath = std::vector<STiledMapPosition>;

enum class ETiledCellType
{
    EMPTY,
    WALL
};

struct STiledBlock
{
    STiledBlock() :
        m_left(0),
        m_right(0),       
        m_bottom(0),
        m_top(0)
    {
    }

    size_t m_left;
    size_t m_right;
    size_t m_top;
    size_t m_bottom;

    size_t Area() const 
    {
        return (m_right - m_left) * (m_top - m_bottom);
    }
};

class CTiledCell
{
public:
    CTiledCell();

    void SetWall(bool isWall);
    bool IsWall() const;
    bool IsEmpty() const;

private:
    ETiledCellType m_type;

};

class CTiledMap
{
public:
    CTiledMap(size_t rows, size_t columns, size_t cellSizeInPixels);

    size_t GetColumns() const;
    size_t GetRows() const;

    bool ContainsWall(STiledBlock block) const;

    void SetWall(STiledBlock block);
    void SetWall(std::vector<STiledBlock> blocks);
    void SetEmpty(STiledBlock block);
    void SetEmpty(std::vector<STiledBlock> blocks);

    Vector2 GetSizeByBlock(STiledBlock block) const;
    Vector2 GetPositionByRC(size_t row, size_t column) const;
    Vector2 GetPositionByRC(STiledMapPosition tilePosition) const;
    Vector2 GetCenterPositionByBlock(STiledBlock block) const;

    STiledMapPosition GetRCByPosition(Vector2 position) const;

    CTiledMapPath FindPath(STiledMapPosition from, STiledMapPosition to);

    void SetPosition(Vector2 position);
    Vector2 GetPosition() const;

    float CellWorldSize() const;
    void DrawTiledMap();

private:   
    using Row = std::vector<CTiledCell>;
    using Cells = std::vector<Row>;

    size_t m_cellSizeInPixels;
    size_t m_rows;
    size_t m_columns;    
    Cells m_cells;
    Vector2 m_position;
    std::vector<std::vector<int>> m_sums;
    std::vector<std::pair<int, int>> m_neighborsMask;
    size_t m_maxNeighborsDelta;

private:
    void DrawCell(Vector2 leftBottomPoint) const;    
    void RecalcSums();
    bool IsValid(int row, int column);

    CTiledMapPath GetPath(STiledMapPosition from, STiledMapPosition to, std::vector<std::vector<size_t>> const& distance);
};

std::unique_ptr<CTiledMap> CreateFullScreenTiledMap(size_t rows, size_t columns);

enum class EUniteBlockMode
{
    NONE,
    LARGEST_BLOCK
};  

STiledBlock LargestBlock(std::vector<std::vector<unsigned char>> const& arr);

std::vector<STiledBlock> UniteBlockByLargest(std::vector<std::vector<unsigned char>> arr);

std::vector<STiledBlock> LoadWallBlocks(size_t* outRows, size_t* outColumns, char const* fileName, EUniteBlockMode uniteMode);
