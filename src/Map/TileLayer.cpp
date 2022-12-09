#include "TileLayer.h"
#include "TextureManager.h"


TileLayer::TileLayer(int tilesize, int rowcount, int colcount, const TileMap& tilemap, const TilesetList& tilesets, bool collision, bool front) : 
    m_TileSize  {tilesize},
    m_RowCount  {rowcount},
    m_ColCount  {colcount},
    m_TileMap   {tilemap},
    m_Tilesets  {tilesets},
    m_Collision {collision},
    m_Front     {front}
{
    for (int k {0}; k < static_cast<int>(m_Tilesets.size()); k++)
        TextureManager::getInstance()->Load(m_Tilesets.at(k).Name, "../assets/Maps/" + m_Tilesets.at(k).Source, m_ColCount*m_TileSize, m_RowCount*m_TileSize);

    LayerType = TILE;
}

TileLayer::TileLayer(TileLayer* layer):
    m_TileSize  {layer->m_TileSize},
    m_RowCount  {layer->m_RowCount},
    m_ColCount  {layer->m_ColCount},
    m_TileMap   {layer->m_TileMap},
    m_Tilesets  {layer->m_Tilesets},
    m_Collision {layer->m_Collision},
    m_Front     {layer->m_Front}
{
    for (int k {0}; k < static_cast<int>(m_Tilesets.size()); k++)
        TextureManager::getInstance()->Load(m_Tilesets.at(k).Name, "../assets/Maps/" + m_Tilesets.at(k).Source, m_ColCount*m_TileSize, m_RowCount*m_TileSize);

    LayerType = TILE;
}


void TileLayer::Render()
{
    for (int row {0}; row < m_RowCount; row++)
        for (int col {0}; col < m_ColCount; col++)
        {
            int tileID {m_TileMap.at(row).at(col)};

            if (tileID == 0)
                continue;

            int index {0};

            if (m_Tilesets.size() > 1)
            {
                for (int k {0}; k < static_cast<int>(m_Tilesets.size()); k++)
                {
                    if (tileID > m_Tilesets.at(k).FirstID && tileID < m_Tilesets.at(k).LastID)
                    {
                        tileID = tileID - (m_Tilesets.at(k).LastID - m_Tilesets.at(k).TileCount);   //  Tile's Id in its tileset

                        index = k;
                        break;
                    }
                }
            }
        
            Tileset ts { m_Tilesets.at(index) };

            int tileRow { tileID / ts.ColCount };
            int tileCol { tileID % ts.ColCount - 1 };

            if (tileID % ts.ColCount == 0)
            {
                tileRow--;
                tileCol = ts.ColCount-1;
            }

            TextureManager::getInstance()->DrawTile(ts.Name, col * m_TileSize, row * m_TileSize, m_TileSize, tileRow, tileCol);
        }
    
}

void TileLayer::Update()
{
    
}
