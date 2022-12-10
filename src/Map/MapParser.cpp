#include "MapParser.h"
#include <sstream>
#include <iostream>
#include <string>

std::shared_ptr<MapParser> MapParser::s_MapParserInstance {nullptr};



bool MapParser::Load()
{
    return ( Parse("Level1", "../assets/Maps/map.tmx") );
}

bool MapParser::Parse(const std::string& id, const char* sourceFile)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(sourceFile);

    if (doc.Error())
    {
        std::cerr << "Failed to load the document: " << sourceFile << std::endl;
        return false;
    }

    tinyxml2::XMLElement* root = doc.RootElement();

    int rowcount { root->IntAttribute("height") },
        colcount { root->IntAttribute("width") },
        tilesize { root->IntAttribute("tilewidth") };


    std::shared_ptr<GameMap> gamemap = std::make_shared<GameMap>(colcount, rowcount, tilesize);

    for (tinyxml2::XMLElement* el { root->FirstChildElement("imagelayer")}; el != nullptr; el = el->NextSiblingElement("imagelayer"))
    {
        std::shared_ptr<ImageLayer> imagelayer {ParseImageLayer(el)};
        gamemap->m_MapLayers.push_back(imagelayer);
    }

    TilesetList tilesets;
    for (tinyxml2::XMLElement* el { root->FirstChildElement("tileset") }; el != nullptr; el = el->NextSiblingElement("tileset"))
    {
        tilesets.push_back(ParseTileset(el));
    }

    for (tinyxml2::XMLElement* el { root->FirstChildElement("layer") }; el != nullptr; el = el->NextSiblingElement("layer"))
    {
        std::shared_ptr<TileLayer> newLayer { ParseLayer(el, tilesets, tilesize, rowcount, colcount) };
        if (!newLayer->GetFront())
            gamemap->m_MapLayers.push_back(newLayer);
        else 
            gamemap->m_MapLayersFront.push_back(newLayer);
    }

    m_MapDictionary[id] = gamemap;

    return true;
}

Tileset MapParser::ParseTileset(tinyxml2::XMLElement* xmlTileset)
{
    Tileset tileset;

    tileset.Name        = xmlTileset->Attribute("name");
    tileset.FirstID     = xmlTileset->IntAttribute("firstgid");
    tileset.TileCount   = xmlTileset->IntAttribute("tilecount");
    tileset.ColCount    = xmlTileset->IntAttribute("columns");
    tileset.TileSize    = xmlTileset->IntAttribute("tilewidth");
    tileset.RowCount    = tileset.TileCount / tileset.ColCount;
    tileset.LastID      = tileset.FirstID + tileset.TileCount - 1;   //  FirstID starts with 1

    tinyxml2::XMLElement* image = xmlTileset->FirstChildElement();

    tileset.Source = image->Attribute("source");

    return tileset;
}

std::shared_ptr<TileLayer> MapParser::ParseLayer(tinyxml2::XMLElement* xmlLayer, TilesetList& tilesets, int tilesize, int rowcount, int colcount)
{
    tinyxml2::XMLElement* data { xmlLayer->FirstChildElement("data") };

    std::string matrix {data->GetText()};

    std::istringstream iss(matrix);

    std::string id;
    TileMap tilemap(rowcount, std::vector<int>(colcount, 0));
    for (int row {0}; row < rowcount; ++row)
    {
        for (int col {0}; col < colcount; ++col)
        {
            getline(iss, id, ',');

            tilemap.at(row).at(col) = std::stoi(id);

            if (!iss.good())
                break;
        }
        std::cout << std::endl;
    }
    
    bool collision {false};
    bool front {false};

    tinyxml2::XMLElement* properties = (xmlLayer->FirstChildElement("properties") == nullptr)? nullptr : xmlLayer->FirstChildElement("properties");

    if (properties != nullptr)
    {
        for ( tinyxml2::XMLElement* el = properties->FirstChildElement("property");
            el != nullptr; el = el->NextSiblingElement("property") )
            {
                if (el->FindAttribute("name")->Value() == std::string("collision") && el->BoolAttribute("value") == true)
                    collision = true;
                if (el->FindAttribute("name")->Value() == std::string("front") && el->BoolAttribute("value") == true)
                    front = true;
            }

    }
    return std::make_shared<TileLayer>(tilesize, rowcount, colcount, tilemap, tilesets, collision, front);
}


std::shared_ptr<ImageLayer> MapParser::ParseImageLayer(tinyxml2::XMLElement* xmlImgLayer)
{
    std::string Name { xmlImgLayer->Attribute("name") };
    std::string Source { xmlImgLayer->FirstChildElement("image")->Attribute("source") };

    double opacity {1.0};
    if (xmlImgLayer->FindAttribute("opacity") != nullptr)
        opacity = xmlImgLayer->DoubleAttribute("opacity");

    bool repX = (xmlImgLayer->FindAttribute("repeatx") == nullptr)? false : true;
    bool repY = (xmlImgLayer->FindAttribute("repeaty") == nullptr)? false : true;

    int width { xmlImgLayer->FirstChildElement("image")->IntAttribute("width")};
    int height { xmlImgLayer->FirstChildElement("image")->IntAttribute("height") };

    int offsetX {0}, offsetY {0};

    if (xmlImgLayer->FindAttribute("offsetx") != nullptr)
        offsetX = xmlImgLayer->IntAttribute("offsetx");

    if (xmlImgLayer->FindAttribute("offsety") != nullptr)
        offsetY = xmlImgLayer->IntAttribute("offsety");

    float speed {0.0f};
    tinyxml2::XMLElement* properties = (xmlImgLayer->FirstChildElement("properties") == nullptr)? nullptr : xmlImgLayer->FirstChildElement("properties");

    if (properties != nullptr)
    {
        for ( tinyxml2::XMLElement* el = properties->FirstChildElement("property");
            el != nullptr; el = el->NextSiblingElement("property") )
            {
                if (el->Attribute("name") == std::string("speed"))
                    speed = el->FloatAttribute("value");
                break;
            }
    }


    Image img {width, height, Name, Source};

    return std::make_shared<ImageLayer>(img, opacity, repX, repY, offsetX, offsetY, speed);
}


void MapParser::Clean()
{
    std::map<std::string, std::shared_ptr<GameMap>>::iterator it;
    for (it = m_MapDictionary.begin(); it != m_MapDictionary.end(); it++)
        it->second.reset();

    m_MapDictionary.clear();    
}

