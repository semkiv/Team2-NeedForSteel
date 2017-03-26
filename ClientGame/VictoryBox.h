#pragma once
#include "Angel.h"
#include "Common.h"

class CVictoryBox
{
public :
    CVictoryBox(Vector2 position);

    void Show(size_t winner);
    void Hide();
    bool IsVisible();

private:
    bool m_isVisible;
    std::shared_ptr<Actor>m_winner;
};
