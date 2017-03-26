#pragma once

#include "Projectile.h"
#include "TargetDetection.h"

#include <vector>
#include <memory>

template<typename T>
class CPoolCreateFactory
{
public:
    T* Allocate();
    void Deallocate(T* item);
    void SetStep(size_t step);
    static CPoolCreateFactory<T>& GetInstance();

private:
    static std::unique_ptr<CPoolCreateFactory<T>> s_instance;

    size_t m_step = 10;

    std::list<T> m_items;
    std::vector<T*> m_freeItems;

private:
    CPoolCreateFactory<T>() = default;
};

template<typename T>
class CPoolFactory: public CPoolCreateFactory<T>
{
};

template<typename T>
T* CPoolCreateFactory<T>::Allocate()
{
    if (m_freeItems.empty())
    {
        for (size_t i{ 0 }; i < m_step; ++i)
        {
            m_items.emplace_back();
            m_freeItems.emplace_back(&m_items.back());
        }
    }
    auto tmp = m_freeItems.back();
    m_freeItems.pop_back();
    return tmp;
}

template<typename T>
void CPoolCreateFactory<T>::Deallocate(T* item)
{
    m_freeItems.emplace_back(item);
}

template<typename T>
void CPoolCreateFactory<T>::SetStep(size_t step)
{
    m_step = step;
}

template<typename T>
CPoolCreateFactory<T>& CPoolCreateFactory<T>::GetInstance()
{
    if (s_instance == nullptr)
    {
        s_instance.reset(new CPoolCreateFactory<T>{});
    }
    return *s_instance;
}

template<typename T>
std::unique_ptr<CPoolCreateFactory<T>> CPoolCreateFactory<T>::s_instance = nullptr;
