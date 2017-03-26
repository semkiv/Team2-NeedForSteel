#include "stdafx.h"
#include "Renderable.h"
#include "cassert"
#include <algorithm>

Renderable::Renderable() :
    m_isDestroyed(false)
{
}

void Renderable::Destroy()
{
    assert(!m_isDestroyed);

    m_isDestroyed = true;

    CustomDestroy();

    std::shared_ptr<Renderable> renderable;
    for (auto& component : m_components)
    {
        renderable = component.lock();
        if (renderable != nullptr)
        {
            renderable->Destroy();
        }
    }
    RemoveInvalidComponents();

    if (m_listener != nullptr)
    {
        m_listener->OnDestruction();
    }     
}

bool Renderable::IsDestroyed() const
{
    return m_isDestroyed;
}

int Renderable::GetLayer() const
{
    return m_layer;
}

void Renderable::Reset()
{    
    CustomReset();

    m_isDestroyed = false;
    m_listener.reset();

    std::shared_ptr<Renderable> renderable;
    for (auto& component : m_components)
    {
        renderable = component.lock();
        if (renderable != nullptr)
        {
            renderable->Reset();
        }
    }
    RemoveInvalidComponents();
}

void Renderable::SetListener(std::shared_ptr<IRenderableListener> listener)
{
    m_listener = listener;
}

void Renderable::AddComponent(std::weak_ptr<Renderable> component)
{
    m_components.push_back(component);
}

void Renderable::RemoveInvalidComponents()
{
    m_components.erase(std::remove_if(
        m_components.begin(), m_components.end(), [](std::weak_ptr<Renderable>& component)
    {
        return component.expired();
    }), m_components.end());    
}