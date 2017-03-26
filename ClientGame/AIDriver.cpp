#include "stdafx.h"
#include "AIDriver.h"
#include "Unit.h"

CAIDriver::CAIDriver() :
    m_destionationDelta{1.0f},
    m_destinationReached(false)
{
}

std::weak_ptr<CControlableUnit> CAIDriver::GetUnit() const
{
    return m_unit;
}

void CAIDriver::SetUnit(std::shared_ptr<CControlableUnit> unit)
{    
    m_isEmpty = false;
    m_unit = unit;
}

void CAIDriver::ResetUnit()
{
    m_isEmpty = true;
    m_unit.reset();
}

void CAIDriver::MoveTo(Vector2 pos)
{
    m_destinationReached = false;
    m_pos = pos;
}

void CAIDriver::OnDestinationReached()
{
}

void CAIDriver::Update(float dt)
{
    auto unit = m_unit.lock();
    if (unit != nullptr && !unit->IsDestroyed())
    {
        auto unitPos = unit->GetPosition();

        if ((m_pos - unit->GetPosition()).LengthSquared() > m_destionationDelta * m_destionationDelta)
        {
            m_destinationReached = false;
            auto delta = MathUtil::DeltaAngle(
                MathUtil::AngleFromVector(m_pos - unit->GetPosition()),
                MathUtil::ToRadians(unit->GetRotation()));

            if (delta > 0.035 * unit->GetRotationSpeed())
            {
                unit->HandleRotationSignal(-1.0f);
            } 
            else if (delta < -0.035 * unit->GetRotationSpeed())
            {
                unit->HandleRotationSignal(1.0f);
            } 
            else
            {
                unit->HandleRotationSignal(0.0f);
                unit->HandleAccelerationSignal(1.0f);
            }
        }
        else
        {
            unit->HandleAccelerationSignal(0.0f);
            if (!m_destinationReached)
            {
                m_destinationReached = true;
                OnDestinationReached();
            }
        }
    }
    else
    {
        ResetUnit();
    }
}
