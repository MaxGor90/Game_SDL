#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Layer.h"
#include <vector>


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
        for (int i {0}; static_cast<std::size_t>(i) < m_MapLayers.size(); ++i)
            m_MapLayers.at(i)->Render();
    }
    //  Render Front layers
    void RenderFront()
    {
        for (int i {0}; static_cast<std::size_t>(i) < m_MapLayersFront.size(); ++i)
            m_MapLayersFront.at(i)->Render();
    }
    //  Update non-Front layers
    void Update()
    {
        for (int i {0}; static_cast<std::size_t>(i) < m_MapLayers.size(); ++i)
            m_MapLayers.at(i)->Update();
    }
    //  Update Front layers
    void UpdateFront()
    {
        for (int i {0}; static_cast<std::size_t>(i) < m_MapLayersFront.size(); ++i)
            m_MapLayersFront.at(i)->Update();
    }

    std::vector<Layer*>& GetMapLayers() { return m_MapLayers; }


private:

    std::vector<Layer*> m_MapLayers;
    std::vector<Layer*> m_MapLayersFront;
    friend class MapParser;
    int m_ColCount, m_RowCount, m_Tilesize;
};

#endif /* GAMEMAP_H */
