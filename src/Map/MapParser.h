#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <map>
#include <string>
#include "GameMap.h"
#include "TileLayer.h"
#include "Imagelayer.h"
#include "tinyxml2.h"



class MapParser
{
    static MapParser* s_MapParserInstance;
    MapParser() {}
    ~MapParser()
    {
        Clean();
        delete s_MapParserInstance; 
    }

    std::map<std::string, GameMap*> m_MapDictionary;

    bool Parse(const std::string& id, const char* sourceFile);

    Tileset ParseTileset(tinyxml2::XMLElement* xmlTileset);
    TileLayer* ParseLayer(tinyxml2::XMLElement* xmlLayer, TilesetList& tilesets, int tilesize, int rowcount, int colcount);
    ImageLayer* ParseImageLayer(tinyxml2::XMLElement* xmlImgLayer);

public:

    inline static MapParser* GetInstance()
    {
        return s_MapParserInstance = (s_MapParserInstance == nullptr)? new MapParser : s_MapParserInstance;
    }

    bool Load();

    void Clean();
    
    inline GameMap* GetMap(const std::string& id) { return m_MapDictionary[id]; }



};

#endif /* MAPPARSER_H */
