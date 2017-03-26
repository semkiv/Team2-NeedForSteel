#pragma once

#include "../Angel/Infrastructure/Renderable.h"
#include "Common.h"
#include "PoolFactory.h"
#include "Projectile.h"
#include <type_traits>
#include <memory>

#define theFactory CFactory::GetInstance()

template <typename T>
void DestroyDeletor(T* ptr);

class CFactory
{
public:
    template <
        typename T,
        typename = std::enable_if_t<
        std::is_base_of<Renderable, T>::value>
    >
    std::shared_ptr<T> Create(ELayer layer = ELayer::DEFAULT);

    template <
        typename T,
        typename = std::enable_if_t<
        std::is_base_of<Renderable, T>::value>
    >
    std::shared_ptr<T> CreateFromLua(std::string const& configFile);

    template <
        typename T,
        typename = std::enable_if_t<
        std::is_base_of<Renderable, T>::value>
    >
    void Free(T* object);

    static CFactory& GetInstance();

private:
    CFactory() = default;
    static CFactory* s_instance;
};

template<typename T>
void DestroyDeletor(T* ptr)
{
	if (!ptr->IsDestroyed())
	{
		ptr->Destroy();
	}

    theFactory.Free(ptr);
}

template<typename T, typename>
std::shared_ptr<T> CFactory::Create(ELayer layer)
{
    auto obj = CPoolFactory<T>::GetInstance().Allocate();
	
	obj->Reset();
    theWorld.Add(obj, static_cast<int>(layer));
    return std::shared_ptr<T>(obj, DestroyDeletor<T>);
}

template<typename T, typename>
void CFactory::Free(T* object)
{
    object->OnFree();
    theWorld.Remove(object);
    CPoolFactory<T>::GetInstance().Deallocate(object);
}
