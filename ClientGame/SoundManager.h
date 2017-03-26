#pragma once
#include "SoundManager.h"
#include "EventManager.h"
#include <vector>
#include <unordered_map>

#define theSoundManager CSoundManager::GetInstance()

class CSoundManager
{
public:
    static CSoundManager& GetInstance();
    void PlaySound(EEvent event, std::weak_ptr<Actor> source = std::weak_ptr<Actor>{});
    void Pause(bool paused);
    void Update();
    void UpdateVolume(float volume);
    void SetState(bool state);
    void Stop();
private:
    struct Sound
    {
        Sound(AngelSoundHandle m_handle, std::weak_ptr<Actor> source);
        Sound(Sound const&) = default;
        Sound& operator=(Sound&& sound);
        ~Sound();
        bool Update();
        AngelSoundHandle m_handle;
        std::weak_ptr<Actor> m_source;
        
    };

private:
    CSoundManager();
private:
    static CSoundManager* ms_instance;
    std::unordered_map<EEvent, AngelSampleHandle> m_samples;
    std::vector<Sound> m_sounds;
    float m_volume;
    bool m_state;
};
