#include "stdafx.h"
#include "SoundManager.h"
#include "EventManager.h"
#include <string>

CSoundManager* CSoundManager::ms_instance = nullptr;

CSoundManager& CSoundManager::GetInstance()
{
    if (ms_instance == nullptr)
    {
        ms_instance = new CSoundManager{};
    }
    return *ms_instance;
}

void CSoundManager::PlaySound(EEvent event, std::weak_ptr<Actor> source)
{
    if (m_state)
    {
        switch (event)
        {
        case EEvent::SHOT:
            m_sounds.emplace_back(theSound.PlaySound(m_samples[EEvent::SHOT], m_volume), source);
            break;
        case EEvent::EXPLOSION:
            break;
        }
    }
}

void CSoundManager::Pause(bool paused)
{
    for (auto const& element : m_sounds)
    {
        theSound.PauseSound(element.m_handle, paused);
    }
}

void CSoundManager::Update()
{
    for (auto it{ m_sounds.begin() }; it != m_sounds.end();)
    {
        if (!it->Update())
        {
            if (it == --m_sounds.cend())
            {
                m_sounds.pop_back();
                break;
            }
            else
            {
                *it = std::move(m_sounds.back());
                m_sounds.pop_back();
            }
        }
        else
        {
            ++it;
        }
    }
}

CSoundManager::CSoundManager():
    m_volume{ thePrefs.GetFloat("Settings", "SoundVolume") },
    m_state{ thePrefs.GetInt("Settings", "SoundOn") == 1 }
{
    m_samples[EEvent::SHOT] = theSound.LoadSample("Resources/Sounds/tank_shot_3.ogg", false);
}

void CSoundManager::UpdateVolume(float volume)
{
    m_volume = volume;

    for (auto const& element: m_sounds)
    {
        theSound.SetVolume(element.m_handle, m_volume);
    }

    PlaySound(EEvent::SHOT);
}

void CSoundManager::SetState(bool state)
{
    if (!(m_state = state))
    {
        Stop();
    }
}

void CSoundManager::Stop()
{
        m_sounds.clear();
}

CSoundManager::Sound::Sound(AngelSoundHandle handle, std::weak_ptr<Actor> source):
    m_handle{ handle },
    m_source{ source }
{
    Update();
}

CSoundManager::Sound& CSoundManager::Sound::operator=(Sound&& sound)
{
    m_handle = sound.m_handle;
    m_source = sound.m_source;

    sound.m_handle = 0;

    return *this;
}

CSoundManager::Sound::~Sound()
{
    if (m_handle)
    {
        theSound.StopSound(m_handle);
    }
}

bool CSoundManager::Sound::Update()
{
    if (!theSound.IsPlaying(m_handle))
    {
        return false;
    }

    auto ptr = m_source.lock();
    if (ptr)
    {
        theSound.SetPan(m_handle, 2 * (ptr->GetPosition().X - theCamera.GetWorldMinVertex().X) / (theCamera.GetWorldMaxVertex().X - theCamera.GetWorldMinVertex().X) - 1);
    }

    return true;
}
