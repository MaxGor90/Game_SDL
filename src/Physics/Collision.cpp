#include "Collision.h"
#include "Engine.h"




Collision* Collision::s_CollisionInstance {nullptr};

Collision::Collision()
{
    TileLayer* layer {nullptr};
    
    for (std::size_t i {0}; i < Engine::getInstance()->getMap()->GetMapLayers().size(); ++i)
    {
        if (Engine::getInstance()->getMap()->GetMapLayers().at(i)->LayerType == Layer::TILE)
        {
            layer = (TileLayer*)Engine::getInstance()->getMap()->GetMapLayers().at(i);
            if (layer->GetCollision())
            {
                m_CollisionLayers.push_back(layer);
                m_CollisionMaps.push_back(&(layer->GetTileMap()));
            }
        }
    }
}


bool Collision::CheckCollision(SDL_Rect* rect_a, SDL_Rect* rect_b)
{
    bool xCollision {   (rect_a->x + rect_a->w > rect_b->x) &&
                        (rect_a->x < rect_b->x + rect_b->w) };

    bool yCollision {   (rect_a->y + rect_a->h > rect_b->y) &&
                        (rect_a->y < rect_b->y + rect_b->h) };

    return xCollision && yCollision;
}


//  Check collision with map layers on left or right side of rect
//  If collision occurs transform->m_X is moved to closest to collision position without intersection +1
bool Collision::CollisionWithMapX(Transform** transform, SDL_Rect* rect)
{
    int rowCount { Engine::getInstance()->getMap()->getRowCount() };
    int colCount { Engine::getInstance()->getMap()->getColCount() };
    int tileSize { Engine::getInstance()->getMap()->getTileSize() };

    //  Position of Collision Box inside the Frame
    int CollisionDeltaX {rect->x - (int)(*transform)->m_X};


    int leftTile {(int)(rect->x / tileSize)};
    int rightTile {(int)((rect->x + rect->w) / tileSize)};
    int upperTile {((int)rect->y +1) / tileSize};              // Y upper collision won't be counted as X collision as well
    int lowerTile {(int)((rect->y + rect->h -1) / tileSize)};     // Y lower collision won't be counted as X collision as well

    if (leftTile < 0)   leftTile = 0;
    if (rightTile >= colCount)   rightTile = colCount -1;
    if (upperTile < 0)   upperTile = 0;
    if (lowerTile >= rowCount)   lowerTile = rowCount -1;

    for (TileMap* map : m_CollisionMaps)
    {
        
        /*
            Solution for upper corners collisions where transform position would create both X and Y collision  
        but should be treated differently depending on what movement created a collision.
            CollisionDepth is the amount of pixels collision box intersects with map.
            If movement is vertical (Jump) collisionDepthY will most likely be less than collisionDepthX:
        on Y axis collision box is approaching collision tile with small speed while on X axis when collision occurs collision box is located randomly,
        so there's a small chance that X depth will be smaller than Y, BUT THIS CHANCE STILL EXISTS.
            The same for X movement (Run, Roll).
            Possible bugs due to small chance of "unfortunate" placing close to tile borders will be looked after. 
        */
        if (map->at(upperTile).at(leftTile) != 0)
        {
            int collisionDepthX {(leftTile +1)*tileSize - rect->x};
            int collisionDepthY {(upperTile +1)*tileSize - rect->y};

            if (collisionDepthY < collisionDepthX)
            {
                CollisionWithMapY(transform, rect);
                return false;
            }
        }
        if (map->at(upperTile).at(rightTile) != 0)
        {
            int collisionDepthX {rect->x + rect->w - (rightTile)*tileSize};
            int collisionDepthY {(upperTile +1)*tileSize - rect->y};

            if (collisionDepthY < collisionDepthX)
            {
                CollisionWithMapY(transform, rect);
                return false;
            }
        }
        /*----------------------------------------------------------------*/

        for (int i {upperTile}; i < lowerTile; ++i)       
        {
            if (map->at(i).at(leftTile) != 0)      
            {
                (*transform)->m_X = (leftTile + 1)*tileSize + 1 - CollisionDeltaX;        //  Closest to collision position without intersection
                return true;
            } else 
            if (map->at(i).at(rightTile) != 0)   
            {
                (*transform)->m_X = (rightTile)*tileSize - rect->w - 1 - CollisionDeltaX;    //  Closest to collision position without intersection
                return true;
            }
        }
    }

    return false;
}

//  Check collision with map layers on upper and lower sides of rect
//  If collision occurs transform->m_Y is moved to closest collision position without intersection 
bool Collision::CollisionWithMapY(Transform** transform, SDL_Rect* rect)
{
    int rowCount { Engine::getInstance()->getMap()->getRowCount() };
    int colCount { Engine::getInstance()->getMap()->getColCount() };
    int tileSize { Engine::getInstance()->getMap()->getTileSize() };

    //  Position of Collision Box inside the Frame
    int CollisionDeltaY {rect->y - (int)(*transform)->m_Y};

    int leftTile {(int)(rect->x +1) / tileSize};                            // X left collision won't be counted as Y collision as well  
    int rightTile {(int)((rect->x + rect->w -1) / tileSize)};               // X right collision won't be counted as Y collision as well 
    int upperTile {(int)(rect->y / tileSize)};
    int lowerTile {(int)((rect->y + rect->h) / tileSize)};

    if (leftTile < 0)   leftTile = 0;
    if (rightTile >= colCount)   rightTile = colCount -1;
    if (upperTile < 0)   upperTile = 0;
    if (lowerTile >= rowCount)   lowerTile = rowCount -1;

    for (TileMap* map : m_CollisionMaps)
    {
        for (int i {leftTile}; i <= rightTile; ++i)       
        {
            if (map->at(upperTile).at(i) != 0)
            {    
                (*transform)->m_Y = (upperTile +1)*tileSize - CollisionDeltaY +1; //  Closest to collision position without intersection
                return true;
            } 
            if (map->at(lowerTile).at(i) != 0)
            {
                (*transform)->m_Y = lowerTile*tileSize - rect->h - CollisionDeltaY;  //  Collsiion with intersection by 1 pixel
                return true;
            }
        }
    }

    return false;
}



