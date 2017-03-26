#pragma once

#include "Unit.h"

#include <memory>
#include <unordered_set>

namespace std
{
    template<typename T>
    struct hash<std::weak_ptr<T>>
    {
        using result_type = size_t;
        using argument_type = std::weak_ptr<T>;

        result_type operator()(argument_type const& ptr) const
        {
            return std::hash<std::shared_ptr<T>>()(ptr.lock());
        }
    };

    template<typename T>
    struct equal_to<std::weak_ptr<T>>
    {
        using result_type = bool;
        using first_argument_type = std::weak_ptr<T>;
        using second_argument_type = std::weak_ptr<T>;

        result_type operator()(first_argument_type const& l, second_argument_type const& r) const
        {
            return equal_to<std::shared_ptr<T>>()(l.lock(), r.lock());
        }
    };
}


class CTargetDetector : public CPhysicsUnit,
    public CChild
{
public:
    using ValueType = CPhysicsUnit;
    using ContainerType = std::unordered_set<std::weak_ptr<ValueType>>;

    CTargetDetector();
    virtual ~CTargetDetector();

    ContainerType& GetTargets();

    virtual void OnCollision(std::shared_ptr<ValueType> physicsUnit) override;
    virtual void EndCollision(std::shared_ptr<ValueType> physicsUnit) override;

    virtual std::shared_ptr<CTeamMember> GetTeamMember();
    bool IsTargretValid(std::shared_ptr<ValueType> target) const;

    virtual void CustomDestroy() override;

private:
    bool Check(std::shared_ptr<ValueType> physicsUnit);

private:
    ContainerType m_targets;
};
