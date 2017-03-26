#include "stdafx.h"
#include <cassert>
#include "TMXParser.h"

std::string TrimDots(std::string str)
{
    std::string::iterator iter = str.begin();

    while (*iter == '.' || *iter == '/')
    {
        ++iter;
    }

    return std::string(iter, str.end());
}

CTMXParser::CTMXParser(std::string const& src):
    m_doc{ src.c_str() }
{
    bool fileLoaded = m_doc.LoadFile();
    assert(fileLoaded);
}

std::string CTMXParser::GetBackground()
{
    auto map = m_doc.FirstChildElement("map");
    auto imagelayer = map->FirstChildElement("imagelayer");
    auto image = imagelayer->FirstChildElement("image");
    return TrimDots(image->Attribute("source"));     
}

std::vector<std::vector<unsigned char>> CTMXParser::GetMask(size_t index)
{
    std::vector<std::vector<unsigned char>> mask;
    
    auto indexStr = std::to_string(index);

    auto map = m_doc.FirstChildElement("map");
    auto layer = map->FirstChildElement("layer");
    auto data = layer->FirstChildElement("data");

    auto height = static_cast<decltype(mask)::size_type>(std::stoi(layer->Attribute("height")));
    auto width = static_cast<decltype(mask)::value_type::size_type>(std::stoi(layer->Attribute("width")));

    mask.resize(height);
    for (auto& element: mask)
    {
        element.resize(width);
    }

    std::string dataStr = data->GetText();
    dataStr += "  ";

    auto it = dataStr.begin();

    for (auto& element : mask)
    {
        for (auto& element2 : element)
        {
            std::string num;
            while (isdigit(*it))
            {
                num += *it;
                ++it;
            }
            if (num == indexStr)
            {
                element2 = 1;
            }
            ++it;
        }
        ++it;
    }

    return mask;
}

std::string CTMXParser::GetTileTexture(size_t index)
{
    auto indexStr = std::to_string(--index);

    auto map = m_doc.FirstChildElement("map");
    auto tileSet = map->FirstChildElement("tileset");
    auto tile = tileSet->FirstChildElement("tile");

    for (size_t i{ 0 }; i < static_cast<size_t>(std::stoi(tileSet->Attribute("tilecount"))); ++i)
    {
        auto x = static_cast<size_t>(std::stoi(tile->Attribute("id")));

        if (index == static_cast<size_t>(std::stoi(tile->Attribute("id"))))
        {
            auto image = tile->FirstChildElement("image");
            return TrimDots(std::string{ image->Attribute("source") });
        }
        tile = tile->NextSiblingElement();
    }

    return "";
}
