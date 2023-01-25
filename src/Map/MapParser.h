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
    MapParser() {}
    ~MapParser()
    {
        Clean();
    }

    std::map<std::string, std::shared_ptr<GameMap>> m_MapDictionary;

    bool Parse(const std::string& id, const char* sourceFile);

    Tileset ParseTileset(tinyxml2::XMLElement* xmlTileset);
    std::unique_ptr<TileLayer> ParseLayer(tinyxml2::XMLElement* xmlLayer, TilesetList& tilesets, int tilesize, int rowcount, int colcount);
    std::unique_ptr<ImageLayer> ParseImageLayer(tinyxml2::XMLElement* xmlImgLayer);

public:

    
    inline static MapParser& getInstance()
    {
        static MapParser MapParserInstance;
        return MapParserInstance;
    }

    bool Load();

    void Clean();
    
    inline std::shared_ptr<GameMap> GetMap(const std::string& id) { return m_MapDictionary[id]; }



};

#endif /* H_MAPPARSER */
