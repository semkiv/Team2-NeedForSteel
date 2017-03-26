#pragma once

#include <string>
#include "tinyxml.h"
#include <vector>

class CTMXParser
{
public:
    CTMXParser(std::string const& src);
    std::string GetBackground();
    std::vector<std::vector<unsigned char>> GetMask(size_t index);
    std::string GetTileTexture(size_t index);
private:
    TiXmlDocument m_doc;
};
