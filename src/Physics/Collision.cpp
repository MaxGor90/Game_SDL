#include "Collision.h"
#include "Engine.h"




std::shared_ptr<Collision> Collision::s_CollisionInstance {nullptr};

Collision::Collision()
{
    for (std::size_t i {0}; i < Engine::getInstance()->getMap()->GetMapLayers().size(); ++i)
    {
        if (Engine::getInstance()->getMap()->GetMapLayers().at(i)->LayerType == Layer::TILE)
        {
            std::shared_ptr<TileLayer> layer = std::dynamic_pointer_cast<TileLayer>(Engine::getInstance()->getMap()->GetMapLayers().at(i));
            if (layer->GetCollision())
            {
                m_CollisionLayers.push_back(layer);
                m_CollisionMaps.push_back(std::make_shared<TileMap>(layer->GetTileMap()));
            }
        }
    }
}

//  Collision between two collision boxes
bool Collision::CheckCollision(std::shared_ptr<SDL_Rect> collisionBox1, std::shared_ptr<SDL_Rect> collisionBox2)
{
    bool xCollision {   (collisionBox1->x + collisionBox1->w > collisionBox2->x) &&
                        (collisionBox1->x < collisionBox2->x + collisionBox2->w) };

    bool yCollision {   (collisionBox1->y + collisionBox1->h > collisionBox2->y) &&
                        (collisionBox1->y < collisionBox2->y + collisionBox2->h) };

    return xCollision && yCollision;
}


//  Check collision with map layers on left or right side of rect
//  If collision occurs transform->m_X is moved to closest to collision position without intersection +1
bool Collision::CollisionWithMapX(std::shared_ptr<Transform>* position, std::shared_ptr<SDL_Rect> collisionBiox)
{
    int rowCount { Engine::getInstance()->getMap()->getRowCount() };
    int colCount { Engine::getInstance()->getMap()->getColCount() };
    int tileSize { Engine::getInstance()->getMap()->getTileSize() };

    //  Position of Collision Box inside the Frame
    int CollisionDeltaX {collisionBiox->x - (int)(*position)->m_X};


    int leftTile {(int)(collisionBiox->x / tileSize)};
    int rightTile {(int)((collisionBiox->x + collisionBiox->w) / tileSize)};
    int upperTile {((int)collisionBiox->y +1) / tileSize};              // Y upper collision won't be counted as X collision as well
    int lowerTile {(int)((collisionBiox->y + collisionBiox->h -1) / tileSize)};     // Y lower collision won't be counted as X collision as well

    if (leftTile < 0)   leftTile = 0;
    if (rightTile >= colCount)   rightTile = colCount -1;
    if (upperTile < 0)   upperTile = 0;
    if (lowerTile >= rowCount)   lowerTile = rowCount -1;

    for (auto map : m_CollisionMaps)
    {
        
        /*
            Solution for upper corners collisions where position position would create both X and Y collision  
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
            int collisionDepthX {(leftTile +1)*tileSize - collisionBiox->x};
            int collisionDepthY {(upperTile +1)*tileSize - collisionBiox->y};

            if (collisionDepthY < collisionDepthX)
            {
                CollisionWithMapY(position, collisionBiox);
                return false;
            }
        }
        if (map->at(upperTile).at(rightTile) != 0)
        {
            int collisionDepthX {collisionBiox->x + collisionBiox->w - (rightTile)*tileSize};
            int collisionDepthY {(upperTile +1)*tileSize - collisionBiox->y};

            if (collisionDepthY < collisionDepthX)
            {
                CollisionWithMapY(position, collisionBiox);
                return false;
            }
        }
        /*----------------------------------------------------------------*/

        for (int i {upperTile}; i <= lowerTile; ++i)       
        {
            if (map->at(i).at(leftTile) != 0)      
            {
                (*position)->m_X = (leftTile + 1)*tileSize + 1 - CollisionDeltaX;        //  Closest to collision position without intersection
                return true;
            } else 
            if (map->at(i).at(rightTile) != 0)   
            {
                (*position)->m_X = (rightTile)*tileSize - collisionBiox->w - 1 - CollisionDeltaX;    //  Closest to collision position without intersection
                return true;
            }
        }
    }

    return false;
}

//  Check collision with map layers on upper and lower sides of rect
//  If collision occurs transform->m_Y is moved to closest collision position without intersection 
bool Collision::CollisionWithMapY(std::shared_ptr<Transform>* position, std::shared_ptr<SDL_Rect> cillisionBox)
{
    int rowCount { Engine::getInstance()->getMap()->getRowCount() };
    int colCount { Engine::getInstance()->getMap()->getColCount() };
    int tileSize { Engine::getInstance()->getMap()->getTileSize() };

    //  Position of Collision Box inside the Frame
    int CollisionDeltaY {cillisionBox->y - (int)(*position)->m_Y};

    int leftTile {(int)(cillisionBox->x +1) / tileSize};                            // X left collision won't be counted as Y collision as well  
    int rightTile {(int)((cillisionBox->x + cillisionBox->w -1) / tileSize)};               // X right collision won't be counted as Y collision as well 
    int upperTile {(int)(cillisionBox->y / tileSize)};
    int lowerTile {(int)((cillisionBox->y + cillisionBox->h) / tileSize)};

    if (leftTile < 0)   leftTile = 0;
    if (rightTile >= colCount)   rightTile = colCount -1;
    if (upperTile < 0)   upperTile = 0;
    if (lowerTile >= rowCount)   lowerTile = rowCount -1;

    for (auto map : m_CollisionMaps)
    {
        for (int i {leftTile}; i <= rightTile; ++i)       
        {
            if (map->at(upperTile).at(i) != 0)
            {    
                (*position)->m_Y = (upperTile +1)*tileSize - CollisionDeltaY +1; //  Closest to collision position without intersection
                return true;
            } 
            if (map->at(lowerTile).at(i) != 0)
            {
                (*position)->m_Y = lowerTile*tileSize - cillisionBox->h - CollisionDeltaY;  //  Collsiion with intersection by 1 pixel
                return true;
            }
        }
    }

    return false;
}



