#ifndef H_GAMEMAP
#define H_GAMEMAP

#include "Layer.h"
#include <vector>
#include <memory>


class GameMap
{
public:
    
    GameMap(int colCount, int rowCount, int tileSize):
        m_ColCount {colCount}, m_RowCount { rowCount}, m_Tilesize {tileSize}
    {}

    int getColCount() { return m_ColCount; }
    int getRowCount() { return m_RowCount; }
    int getTileSize() { return m_Tilesize; }
    int getMapLength() { return m_ColCount * m_Tilesize; }


    //  Render non-Front layers
    void Render()
    {
        for (auto layer : m_MapLayers)
            layer->Render();
        for (auto layer : m_MapCollisionLayers)
            layer->Render();
    }
    //  Render Front layers
    void RenderFront()
    {
        for (auto layer : m_MapLayersFront)
            layer->Render();
    }
    //  Update non-Front layers
    void Update()
    {
        for (auto layer : m_MapLayers)
            layer->Update();
        for (auto layer : m_MapCollisionLayers)
            layer->Update();
    }
    //  Update Front layers
    void UpdateFront()
    {
        for (auto layer : m_MapLayersFront)
            layer->Update();
    }

    std::vector<std::shared_ptr<Layer>>& GetMapLayers() { return m_MapLayers; }
    std::vector<std::shared_ptr<Layer>>& GetMapCollisionLayers() { return m_MapCollisionLayers; }



private:

    std::vector<std::shared_ptr<Layer>> m_MapLayers;
    std::vector<std::shared_ptr<Layer>> m_MapLayersFront;
    std::vector<std::shared_ptr<Layer>> m_MapCollisionLayers;
    friend class MapParser;
    int m_ColCount, m_RowCount, m_Tilesize;
};

#endif /* H_GAMEMAP */
