#include "stdafx.h"
#include "ShopUI.h"
#include "Common.h"

#include <string>

CShopUI::CShopUI(EScreenSide side):
    m_side{ side },
    m_state{ false },
    m_currentIcon{ 0 },
    m_moveToTime{ .25f },
    m_iconSize{ 1.f }
{
    m_background.SetSize(Vector2{ m_iconSize, m_iconSize * 3 });
    switch (side)
    {
    case EScreenSide::LEFT:
        m_background.SetPosition(theCamera.GetWorldMinVertex().X - m_background.GetSize().X * 0.45f, 0.f);
        break;
    case EScreenSide::RIGHT:
        m_background.SetPosition(theCamera.GetWorldMaxVertex().X + m_background.GetSize().X * 0.45f, 0.f);
        break;
    }

    m_background.SetSprite("Resources/Images/shop_background.png");
    theWorld.Add(&m_background, static_cast<int>(ELayer::GAME_UI));

    m_icons.resize(3);
    float currentPos = m_background.GetPosition().Y + m_background.GetSize().Y / 2 + m_iconSize / 2;

    m_icons[0].LoadSpriteFrames("Resources/Images/Icons/damage/_01.png");
    m_icons[1].LoadSpriteFrames("Resources/Images/Icons/hp/_01.png");
    m_icons[2].LoadSpriteFrames("Resources/Images/Icons/reload/_01.png");
    
    for (auto& element : m_icons)
    {
        element.SetColor({ 1.0f, 1.0f, 1.0f, 1.f });
        element.SetSize({ m_iconSize, m_iconSize });
        element.SetPosition(m_background.GetPosition().X, currentPos -= m_iconSize);
        theWorld.Add(&element, static_cast<int>(ELayer::GAME_UI) + 1);
    }

    UpdatePrice();

    auto textPos = m_icons.front().GetPosition();
    textPos.Y += m_iconSize;
    m_currentPrice.SetPosition(textPos);
    m_currentPrice.SetAlignment(TXT_Center);
    m_currentPrice.SetFont(theUI.GetDefaultFont());
    m_currentPrice.SetColor(theUI.GetDefaultColor());
    theWorld.Add(&m_currentPrice, static_cast<int>(ELayer::GAME_UI) + 1);
}


CShopUI::~CShopUI()
{
    m_state = false;
    theWorld.Remove(&m_background);    

    for (auto& element : m_icons)
    {
        theWorld.Remove(&element);
    }
    theWorld.Remove(&m_currentPrice);
}

void CShopUI::Show()
{
    UpdatePrice();
    m_state = true;
    float xPos;
    switch (m_side)
    {
    case EScreenSide::LEFT:
        xPos = theCamera.GetWorldMinVertex().X + m_background.GetSize().X / 2;
        break;
    case EScreenSide::RIGHT:
        xPos = theCamera.GetWorldMaxVertex().X - m_background.GetSize().X / 2;
        break;

    }

    m_background.MoveTo(Vector2{ xPos, m_background.GetPosition().Y }, m_moveToTime);
    for (auto& element : m_icons)
    {
        element.MoveTo(Vector2{ xPos, element.GetPosition().Y }, m_moveToTime);
    }
    m_currentPrice.MoveTo(Vector2{ xPos, m_currentPrice.GetPosition().Y }, m_moveToTime);
    m_icons[m_currentIcon].HighLight(true);
}

void CShopUI::Hide()
{
    m_state = false;
    float xPos;
    switch (m_side)
    {
    case EScreenSide::LEFT:
        xPos = theCamera.GetWorldMinVertex().X - m_background.GetSize().X * 0.45f;
        break;
    case EScreenSide::RIGHT:
        xPos = theCamera.GetWorldMaxVertex().X + m_background.GetSize().X * 0.45f;
        break;
    }

    m_background.MoveTo(Vector2{ xPos, m_background.GetPosition().Y }, m_moveToTime);
    for (auto& element : m_icons)
    {
        element.MoveTo(Vector2{ xPos, element.GetPosition().Y }, m_moveToTime);
    }
    m_currentPrice.MoveTo(Vector2{ xPos, m_currentPrice.GetPosition().Y }, m_moveToTime);
    m_icons[m_currentIcon].HighLight(false);
}

size_t CShopUI::AcceptCommand(ENavigationCommand command)
{
    switch (command)
    {
    case ENavigationCommand::UP:
        if (m_currentIcon != 0)
        {
            m_icons[m_currentIcon].HighLight(false);
            --m_currentIcon;
            m_icons[m_currentIcon].HighLight(true);
        }
        break;
    case ENavigationCommand::DOWN:
        if (m_currentIcon != m_icons.size() - 1)
        {
            m_icons[m_currentIcon].HighLight(false);
            ++m_currentIcon;
            m_icons[m_currentIcon].HighLight(true);
        }
        break;
    case ENavigationCommand::UPGRADE:
        m_icons[m_currentIcon].Upgrade();
        break;
    }

    UpdatePrice();
    return CurrentIcon();
}

size_t CShopUI::CurrentIcon() const
{
    return m_currentIcon;
}

bool CShopUI::IsActive() const
{
    return m_state;
}

void CShopUI::SetUpgradePrice(std::shared_ptr<IUpgradePrice> shop)
{
    m_upgradePrice = shop;
}

void CShopUI::UpdatePrice()
{
    auto pricePtr = m_upgradePrice.lock();
    if (pricePtr)
    {
        m_currentPrice.SetDisplayString(
            std::to_string(pricePtr->UpgradePrice(static_cast<EUpgrade>(m_currentIcon),
                m_icons[m_currentIcon].currentLevel))
        );
    }
}

void CShopUI::SIconUI::HighLight(bool state)
{
    if (state)
    {
        SetSpriteFrame(GetSpriteFrame() + 1);
    }
    else
    {
        SetSpriteFrame(GetSpriteFrame() - 1);
    }
}

void CShopUI::SIconUI::Upgrade()
{
    SetSpriteFrame(GetSpriteFrame() + 2);
    ++currentLevel;
}
