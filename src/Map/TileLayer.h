#ifndef TILELAYER_H
#define TILELAYER_H

#include "Layer.h"
#include <string>
#include <vector>

struct Tileset
{
    int FirstID, LastID;
    int RowCount, ColCount;
    int TileCount, TileSize;
    std::string Name, Source;
};

typedef std::vector<Tileset> TilesetList;
typedef std::vector< std::vector<int> > TileMap;

class TileLayer : public Layer
{
public:
    
    TileLayer(int tilesize, int rowcount, int colcount, const TileMap& tilemap, const TilesetList& tilesets, bool collision, bool front);
    TileLayer(TileLayer&& layer);
    virtual void Render();
    virtual void Update();

    inline TileMap& GetTileMap() { return m_TileMap; }
    inline bool GetCollision()  { return m_Collision; }
    inline bool GetFront() { return m_Front; }



private:

    int m_TileSize;
    int m_RowCount, m_ColCount;
    TileMap m_TileMap;
    TilesetList m_Tilesets;
    bool m_Collision;
    bool m_Front;
};

#endif /* TILELAYER_H */
