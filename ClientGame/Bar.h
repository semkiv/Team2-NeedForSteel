#pragma once
#include "../Angel/Actors/Actor.h"

class CBar
{
public:
    CBar(Vector2 size = {2.f, 0.1f});
    void Show();
    void Hide();
    void SetValue(float value);
    void SetPosition(Vector2 position);  
    void SetColor(Color color);
    void SetBackgroundColor(Color color);

    Vector2 GetSize() const;
    Vector2 GetPosition() const;
protected:
    Actor m_background;
    Actor m_current;
};

class CCoolDownBar: public CBar
{
public:
};
