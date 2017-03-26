#pragma once

#include "UI/UserInterface.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include "Actors/Actor.h"

enum class EScreen
{
    MAIN_MENU,
    GAME_PLAY,
    OPTIONS,
    CONTROL,
    AUDIO,
    PAUSE,
    LEVEL,
    HELP,
    END
};

class CScreen;

class ISwitchable
{
public:
    virtual void MoveUp() = 0;
    virtual void MoveDown() = 0;
    virtual void Enter() = 0;
    virtual void MoveRight() = 0;
    virtual void MoveLeft() = 0;
    virtual void Back() = 0;
};

class IEnumScreenStack
{
public:
    virtual void PushScreen(EScreen screen) = 0;
    virtual void PopScreen() = 0;
    virtual std::shared_ptr<CScreen> CreateScreen(EScreen screen) = 0;
};

class CScreen
{
public:
    CScreen(IEnumScreenStack* stack);
    virtual void Start() = 0;
    virtual void OnPush() = 0;
    virtual void OnPop() = 0;
    virtual void Stop() = 0;
    virtual void Update(float dt) {}
    virtual void Render() {}

    void SetScreenStack(IEnumScreenStack* stack);

protected:
    IEnumScreenStack*           m_stack;
};

class CMenuScreen :
    public CScreen,
    public ISwitchable,
    public ICallBack,
    public std::enable_shared_from_this<CMenuScreen>
{
public:
    CMenuScreen(IEnumScreenStack* stack, String const& header, StringList const& labels);
    virtual void Start() override;
    virtual void OnPush() override;
    virtual void OnPop() override;
    virtual void Stop() override;

    virtual void MoveUp() override;
    virtual void MoveDown() override;
    virtual void Enter() override;
    virtual void MoveRight() override;
    virtual void MoveLeft() override;
    virtual void Back() override;

    virtual void ChangeControl(bool ok);
    virtual void Update(float dt) override;

    virtual void operator()(size_t option) override;
    virtual void operator()() override;

    virtual ~CMenuScreen();

protected:
    size_t                          m_currentButton;  
    std::vector<AngelUIHandle>      m_buttons;
  
    AngelUIHandle                   m_box;
    String                          m_header;
    unsigned char                   m_colors[4];

private:
    StringList                      m_labels;
};

class CScreenStack
{
public:
    using ScreenPtr = std::shared_ptr<CScreen>;

    void PushScreen(ScreenPtr screen);
    void PopScreen();

    ScreenPtr GetTop();
    bool Empty() const;

private:
    std::vector<ScreenPtr>          m_screensStack;
};