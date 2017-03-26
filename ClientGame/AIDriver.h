#pragma once

class CControlableUnit;

class CAI
{
public:
    virtual void Update(float dt) = 0;
    bool IsEmpty() const { return m_isEmpty; }

protected:
    bool m_isEmpty;
};

class CAIDriver :
    public CAI
{
public:
    CAIDriver();
    virtual ~CAIDriver() = default;

    std::weak_ptr<CControlableUnit> GetUnit() const;
    void SetUnit(std::shared_ptr<CControlableUnit> unit);
    void ResetUnit();

    virtual void MoveTo(Vector2 pos);
    virtual void OnDestinationReached();
    virtual void Update(float dt) override;

protected:
    float m_destionationDelta;

private:
    std::weak_ptr<CControlableUnit> m_unit;
    Vector2 m_pos;
    bool m_destinationReached;    
};
