#ifndef H_MAPPARSER
#define H_MAPPARSER

#include <map>
#include <string>
#include "GameMap.h"
#include "TileLayer.h"
#include "Imagelayer.h"
#include "tinyxml2.h"
#include <memory>



class MapParser
{
    static std::shared_ptr<MapParser> s_MapParserInstance;

    std::map<std::string, std::shared_ptr<GameMap>> m_MapDictionary;

    bool Parse(const std::string& id, const char* sourceFile);

    Tileset ParseTileset(tinyxml2::XMLElement* xmlTileset);
    std::shared_ptr<TileLayer> ParseLayer(tinyxml2::XMLElement* xmlLayer, TilesetList& tilesets, int tilesize, int rowcount, int colcount);
    std::shared_ptr<ImageLayer> ParseImageLayer(tinyxml2::XMLElement* xmlImgLayer);

public:

    MapParser() {}
    ~MapParser()
    {
        Clean();
    }
    
    inline static std::shared_ptr<MapParser> GetInstance()
    {
        return s_MapParserInstance = (s_MapParserInstance == nullptr)? std::make_shared<MapParser>() : s_MapParserInstance;
    }

    bool Load();

    void Clean();
    
    inline std::shared_ptr<GameMap> GetMap(const std::string& id) { return m_MapDictionary[id]; }



};

#endif /* H_MAPPARSER */
