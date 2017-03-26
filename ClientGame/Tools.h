#pragma once

#include <vector>
#include <memory>

template <typename T, typename Func>
void ForeachValidWeak(std::vector<std::weak_ptr<T>>& v, Func func)
{
    assert(&func != nullptr);

    std::shared_ptr<T> shared = nullptr;

    for (size_t i = 0; i < v.size();) 
    {
        shared = v[i].lock();
        if (shared != nullptr)
        {
            func(shared);
            ++i;
        }
        else
        {
            v[i] = v.back();
            v.pop_back();
        }
    }
}
