#include "stdafx.h"
#include "TiledMap.h"
#include <vector>
#include <memory>
#include <fstream>
#include <limits>

#undef max

CTiledCell::CTiledCell() :
    m_type(ETiledCellType::EMPTY)
{
}

void CTiledCell::SetWall(bool isWall)
{
    if (isWall)
    {
        m_type = ETiledCellType::WALL;
    }
    else
    {
        m_type = ETiledCellType::EMPTY;
    }
}

bool CTiledCell::IsWall() const
{
    return m_type == ETiledCellType::WALL;
}

bool CTiledCell::IsEmpty() const
{
    return m_type == ETiledCellType::EMPTY;
}

CTiledMap::CTiledMap(size_t rows, size_t columns, size_t cellSizeInPixels) :
    m_cellSizeInPixels(cellSizeInPixels),
    m_rows(rows),
    m_columns(columns),
    m_neighborsMask{ {-1, 0}, {1, 0}, {0, 1}, {0, -1} },
    m_maxNeighborsDelta(1)
{
    m_cells.resize(rows, Row(columns));
}

size_t CTiledMap::GetColumns() const
{
    return m_columns;
}

size_t CTiledMap::GetRows() const
{
    return m_rows;
}

bool CTiledMap::ContainsWall(STiledBlock block) const
{
    int sum = m_sums[block.m_top - 1][block.m_right - 1];

    if (block.m_left > 0)
    {
        sum -= m_sums[block.m_top - 1][block.m_left - 1];
    }

    if (block.m_bottom > 0)
    {
        sum -= m_sums[block.m_bottom - 1][block.m_right - 1];
    }

    if (block.m_left > 0 && block.m_bottom > 0)
    {
        sum += m_sums[block.m_bottom - 1][block.m_left - 1];
    }

    return sum > 0;
}

void CTiledMap::SetWall(STiledBlock block)
{
    for (size_t j = block.m_left; j < block.m_right; ++j)
    {
        for (size_t i = block.m_bottom; i < block.m_top; ++i)
        {
            m_cells[i][j].SetWall(true);
        }
    }

    RecalcSums();
}

void CTiledMap::SetWall(std::vector<STiledBlock> blocks)
{
    for (auto& block : blocks)
    {
        for (size_t j = block.m_left; j < block.m_right; ++j)
        {
            for (size_t i = block.m_bottom; i < block.m_top; ++i)
            {
                m_cells[i][j].SetWall(true);
            }
        }
    }

    RecalcSums();
}

void CTiledMap::SetEmpty(STiledBlock block)
{
    for (size_t j = block.m_left; j < block.m_right; ++j)
    {
        for (size_t i = block.m_bottom; i < block.m_top; ++i)
        {
            m_cells[i][j].SetWall(false);
        }
    }

    RecalcSums();
}

void CTiledMap::SetEmpty(std::vector<STiledBlock> blocks)
{
    for (auto& block : blocks)
    {
        for (size_t j = block.m_left; j < block.m_right; ++j)
        {
            for (size_t i = block.m_bottom; i < block.m_top; ++i)
            {
                m_cells[i][j].SetWall(false);
            }
        }
    }

    RecalcSums();
}

Vector2 CTiledMap::GetSizeByBlock(STiledBlock block) const
{
    return Vector2(
        (block.m_right - block.m_left) * CellWorldSize(),
        (block.m_top - block.m_bottom) * CellWorldSize());
}

Vector2 CTiledMap::GetPositionByRC(size_t row, size_t column) const
{
    float cellWorldSize = CellWorldSize();

    return m_position + Vector2(
        (static_cast<float>(column) + 0.5f) * cellWorldSize,
        (static_cast<float>(row) + 0.5f) * cellWorldSize);
}

Vector2 CTiledMap::GetPositionByRC(STiledMapPosition tilePosition) const
{
    float cellWorldSize = CellWorldSize();

    return m_position + Vector2(
        (static_cast<float>(tilePosition.second + 0.5f)) * cellWorldSize,
        (static_cast<float>(tilePosition.first + 0.5f)) * cellWorldSize);
}

Vector2 CTiledMap::GetCenterPositionByBlock(STiledBlock block) const
{
    Vector2 a = GetPositionByRC(block.m_bottom, block.m_left);
    Vector2 b = GetPositionByRC(block.m_top - 1, block.m_right - 1);

    return (a + b) * 0.5f;
}

STiledMapPosition CTiledMap::GetRCByPosition(Vector2 position) const
{
    assert(position.X < m_position.X + CellWorldSize() * m_columns && position.Y < m_position.Y + CellWorldSize() * m_rows);

    float cellWorldSize = CellWorldSize();
    Vector2 delta = position - m_position;

    return STiledMapPosition(
        static_cast<size_t>(delta.Y / cellWorldSize),
        static_cast<size_t>(delta.X / cellWorldSize)
    );
}

CTiledMapPath CTiledMap::FindPath(STiledMapPosition from, STiledMapPosition to)
{
    CTiledMapPath path;
    
    if (m_cells[from.first][from.second].IsWall())
    {
        return path;
    }
    
    std::vector<unsigned char> used;
    size_t maxValue = std::numeric_limits<size_t>::max() - 1;

    std::vector<std::vector<size_t>> distance(m_rows, std::vector<size_t>(m_columns, maxValue));
    std::vector<std::vector<unsigned char>> was(m_rows, std::vector<unsigned char>(m_columns, false));

    std::queue<STiledMapPosition> q;
    q.push(from);
    distance[from.first][from.second] = 0;

    STiledMapPosition current;
    size_t row = 0;
    size_t col = 0;

    while (!q.empty())
    {
        current = q.front();
        q.pop();
        row = current.first;
        col = current.second;

        if (was[row][col])
        {
            continue;
        }

        was[row][col] = true;

        for (auto other : m_neighborsMask)
        {
            other.first += (int)current.first;
            other.second += (int)current.second;

            if (!IsValid(other.first, other.second) || was[other.first][other.second] || m_cells[other.first][other.second].IsWall())
            {
                continue;
            }

            distance[other.first][other.second] = distance[row][col] + 1;

            q.push(STiledMapPosition{ (size_t)other.first, (size_t)other.second });
        }
    }

    if (was[to.first][to.second])
    {
        path = GetPath(from, to, distance);
    }

    used.assign(path.size(), 1);
    size_t delta = 0;
    STiledBlock block;
    size_t currentIndex = 0;
    size_t nextIndex = 2;

    for (; nextIndex < path.size(); ++nextIndex)
    {
        block.m_bottom = MathUtil::Min(path[currentIndex].first, path[nextIndex].first);
        block.m_top = MathUtil::Max(path[currentIndex].first, path[nextIndex].first) + 1;
        block.m_left = MathUtil::Min(path[currentIndex].second, path[nextIndex].second);
        block.m_right = MathUtil::Max(path[currentIndex].second, path[nextIndex].second) + 1;

        if (!ContainsWall(block))
        {
            used[nextIndex - 1] = 0;
        }
        else
        {
            currentIndex = nextIndex - 1;
        }
    }

    CTiledMapPath newPath;

    for (size_t i = 0; i < path.size(); ++i)
    {
        if (used[i] > 0)
        {
            newPath.push_back(path[i]);
        }
    }

    return newPath;
}

void CTiledMap::SetPosition(Vector2 position)
{
    m_position = position;
}

Vector2 CTiledMap::GetPosition() const
{
    return m_position;
}

void CTiledMap::DrawTiledMap()
{
     Vector2 leftBottomPoint = m_position;

     glColor3f(0.0f, 1.0f, 0.0f);

     for (size_t i = 0; i < m_rows; ++i)
     {
         leftBottomPoint.X = m_position.X;
         for (size_t j = 0; j < m_columns; ++j)
         {
             if (!m_cells[i][j].IsWall())
             {
                 DrawCell(leftBottomPoint);
             }
             leftBottomPoint.X += CellWorldSize();
         }
         leftBottomPoint.Y += CellWorldSize();
     }

     leftBottomPoint = m_position;
     glColor3f(1.0f, 0.0f, 0.0f);

     for (size_t i = 0; i < m_rows; ++i)
     {
         leftBottomPoint.X = m_position.X;
         for (size_t j = 0; j < m_columns; ++j)
         {
             if (m_cells[i][j].IsWall())
             {
                 DrawCell(leftBottomPoint);
             }
             leftBottomPoint.X += CellWorldSize();
         }
         leftBottomPoint.Y += CellWorldSize();
     }
}

void CTiledMap::DrawCell(Vector2 leftBottomPoint) const
{
    DrawLine(leftBottomPoint, Vector2{ leftBottomPoint.X, leftBottomPoint.Y + CellWorldSize() });
    DrawLine(leftBottomPoint, Vector2{ leftBottomPoint.X + CellWorldSize(), leftBottomPoint.Y });
    DrawLine(Vector2{ leftBottomPoint.X + CellWorldSize(), leftBottomPoint.Y + CellWorldSize() }, Vector2{ leftBottomPoint.X, leftBottomPoint.Y + CellWorldSize() });
    DrawLine(Vector2{ leftBottomPoint.X + CellWorldSize(), leftBottomPoint.Y + CellWorldSize() }, Vector2{ leftBottomPoint.X + CellWorldSize(), leftBottomPoint.Y });
}

float CTiledMap::CellWorldSize() const
{
    return MathUtil::PixelsToWorldUnits(m_cellSizeInPixels);
}

void CTiledMap::RecalcSums()
{
    m_sums.resize(m_rows);

    for (auto& row : m_sums)
    {
        row.resize(m_columns);
    }

    for (size_t i = 0; i < m_rows; ++i)
    {
        for (size_t j = 0; j < m_columns; ++j)
        {
            m_sums[i][j] = (m_cells[i][j].IsWall()) ? (1) : (0);
        }
    }

    for (size_t i = 0; i < m_rows; ++i)
    {
        for (size_t j = 0; j < m_columns; ++j)
        {
            if (i > 0)
            {
                m_sums[i][j] += m_sums[i - 1][j];    
            }            
            if (j > 0)
            {
                m_sums[i][j] += m_sums[i][j - 1];
            }

            if (i > 0 && j > 0)
            {
                m_sums[i][j] -= m_sums[i - 1][j - 1];
            }
        }
    }
}

bool CTiledMap::IsValid(int row, int column)
{
    return row >= 0 && row < (int)m_rows && column >= 0 && column < (int)m_columns;
}

CTiledMapPath CTiledMap::GetPath(STiledMapPosition from, STiledMapPosition to, std::vector<std::vector<size_t>> const & distance)
{
    CTiledMapPath path;

    STiledMapPosition current = to;
    size_t row = 0;
    size_t col = 0;

    do 
    {
        path.push_back(current);

        for (auto other : m_neighborsMask)
        {
            other.first += (int)current.first;
            other.second += (int)current.second;

            if (!IsValid(other.first, other.second))
            {
                continue;
            }

            if (distance[other.first][other.second] == distance[current.first][current.second] - 1)
            {
                current.first = (size_t)other.first;
                current.second = (size_t)other.second;
                break;
            }
        }

    } while (current != from);

    path.emplace_back(current);

    std::reverse(path.begin(), path.end());

    return path;
}

float const g_pixelSizes[] =
{
    16.0f,
    24.0f,
    32.0f,
    48.0f,
    64.0f,
    128.0f
};

std::unique_ptr<CTiledMap> CreateFullScreenTiledMap(size_t rows, size_t columns)
{
    Vector2 worldDimenstions = MathUtil::GetWorldDimensions();
    Vector2 screen = Vector2{ (float)theCamera.GetWindowWidth(), (float)theCamera.GetWindowHeight() };
    float pixelSize = MathUtil::Min(screen.X / (float)columns, screen.Y / (float)rows);

    auto map = std::make_unique<CTiledMap>(rows, columns, pixelSize);
    Vector2 mapSize
    {  
        MathUtil::PixelsToWorldUnits(columns * pixelSize),
        MathUtil::PixelsToWorldUnits(rows * pixelSize)
    };

    map->SetPosition(mapSize * (-0.5f));
    return map;
}

STiledBlock LargestBlock(std::vector<std::vector<unsigned char>> const& arr)
{
    assert(arr.size() != 0);

    STiledBlock block;
    int currentArea = 0;
    int rows = arr.size();
    int columns = arr[0].size();

    std::vector<int> d(columns, -1), d1(columns), d2(columns);
    std::vector<int> stack;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            if (arr[i][j] == 0)
            {
                d[j] = i;
            }
        }
        
        stack.resize(0);

        for (int j = 0; j < columns; ++j)
        {
            while (!stack.empty() && d[stack.back()] <= d[j])
            {
                stack.pop_back();
            }

            d1[j] = (stack.empty()) ? (-1) : (stack.back());
            stack.push_back(j);
        }
        
        stack.resize(0);

        for (int j = columns; j > 0; --j)
        {
            while (!stack.empty() && d[stack.back()] <= d[j - 1])
            {
                stack.pop_back();
            }

            d2[j - 1] = (stack.empty()) ? (columns) : (stack.back());
            stack.push_back(j - 1);
        }

        for (int j = 0; j < columns; ++j)
        {
            currentArea = (i - d[j]) * (d2[j] - d1[j] - 1);

            if (currentArea > (int)block.Area())
            {
                block.m_left = d1[j] + 1;
                block.m_right = d2[j];
                block.m_top = i + 1;
                block.m_bottom = d[j] + 1;
            }
        }
    }

    return block;
}

std::vector<STiledBlock> UniteBlockByLargest(std::vector<std::vector<unsigned char>> arr)
{
    std::vector<STiledBlock> blocks;

    STiledBlock currentBlock;

    while(true)
    {
        currentBlock = LargestBlock(arr);

        for (size_t j = currentBlock.m_left; j < currentBlock.m_right; ++j)
        {
            for (size_t i = currentBlock.m_bottom; i < currentBlock.m_top; ++i)
            {
                arr[i][j] = 0;
            }
        }

        if (currentBlock.Area() > 0)
        {
            blocks.push_back(currentBlock);
        }
        else
        {
            break;
        }
    }

    return blocks;
}

std::vector<STiledBlock> LoadWallBlocks(size_t* outRows, size_t* outColumns, char const* fileName, EUniteBlockMode uniteMode = EUniteBlockMode::LARGEST_BLOCK)
{
    std::vector<STiledBlock> blocks;
    std::vector<std::vector<unsigned char>> arr;

    std::ifstream file(fileName);

    int rows = 0;
    int columns = 0;
    std::string row;

    file >> rows >> columns;

    assert(rows > 0 && columns > 0);

    *outRows = static_cast<size_t>(rows);
    *outColumns = static_cast<size_t>(columns);
    arr.assign(rows, std::vector<unsigned char>(columns, 0));

    for (int i = 0; i < rows; ++i)
    {
        file >> row;
        for (int j = 0; j < columns; ++j)
        {
            arr[rows - i - 1][j] = (row[j] == '1') ? (1) : (0);
        }
    }

    switch (uniteMode)
    {
    case EUniteBlockMode::LARGEST_BLOCK:
        blocks = UniteBlockByLargest(std::move(arr));
        break;
    default:
        break;
    }

    return blocks;
}
