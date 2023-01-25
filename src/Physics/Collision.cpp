#include "Collision.h"
#include "Engine.h"




Collision::Collision()
{
    for (auto layer : Engine::getInstance().getMap()->GetMapCollisionLayers())
    {
        if (layer->LayerType == Layer::TILE)
            m_CollisionMaps.push_back(std::make_shared<TileMap>(std::dynamic_pointer_cast<TileLayer>(layer)->GetTileMap()));
    }
}

//  Collision between two collision boxes with optional overlapping
bool Collision::CheckCollision(std::shared_ptr<SDL_Rect> collisionBox1, std::shared_ptr<SDL_Rect> collisionBox2, int overlapX, int overlapY)
{
    bool xCollision {   (collisionBox1->x + collisionBox1->w > collisionBox2->x + overlapX) &&
                        (collisionBox1->x + overlapX < collisionBox2->x + collisionBox2->w) };

    bool yCollision {   (collisionBox1->y + collisionBox1->h > collisionBox2->y + overlapY) &&
                        (collisionBox1->y + overlapY < collisionBox2->y + collisionBox2->h) };

    return xCollision && yCollision;
}


//  Check collision with map layers on left or right side of rect
//  If collision occurs transform->m_X is moved to closest to collision position without intersection +1
bool Collision::CollisionWithMapX(std::shared_ptr<Transform>* position, std::shared_ptr<SDL_Rect> collisionBox)
{
    int rowCount { Engine::getInstance().getMap()->getRowCount() };
    int colCount { Engine::getInstance().getMap()->getColCount() };
    int tileSize { Engine::getInstance().getMap()->getTileSize() };

    //  Position of Collision Box inside the Frame
    int CollisionDeltaX {collisionBox->x - (int)(*position)->m_X};


    int leftTile {(int)(collisionBox->x / tileSize)};
    int rightTile {(int)((collisionBox->x + collisionBox->w) / tileSize)};

    //  Since X collision check occurs before Y collision, Y collsiion would be counted as X collision as well resulting in X position shift.
    //  To avoid this we're "slightly" narrowing SDL box across Y axis to make sure Y collision isn't counted as X collision
    //  Buffers must be smaller than (collisionBox->h % TileSize)
    //  5 for upperBuffer and 10 for LowerBuffer (falling has more potential of going deeper into collision with map that jumping) seem to work fine.
    int upperBuffer {5}, lowerBuffer {10}; 
    int upperTile {((int)collisionBox->y +upperBuffer) / tileSize};                           
    int lowerTile {(int)((collisionBox->y + collisionBox->h -lowerBuffer) / tileSize)};      

    if (leftTile < 0)   leftTile = 0;
    if (rightTile >= colCount)   rightTile = colCount -1;
    if (upperTile < 0)   upperTile = 0;
    if (lowerTile >= rowCount)   lowerTile = rowCount -1;

    for (auto map : m_CollisionMaps)
    {
        for (int i {upperTile}; i <= lowerTile; ++i)       
        {
            if (map->at(i).at(leftTile) != 0)      
            {
                (*position)->m_X = (leftTile + 1)*tileSize + 1 - CollisionDeltaX;        //  Closest to collision position without intersection
                return true;
            } else 
            if (map->at(i).at(rightTile) != 0)   
            {
                (*position)->m_X = (rightTile)*tileSize - collisionBox->w - 1 - CollisionDeltaX;    //  Closest to collision position without intersection
                return true;
            }
        }
    }

    return false;
}

//  Check collision with map layers on upper and lower sides of rect
//  If collision occurs transform->m_Y is moved to closest collision position without intersection 
bool Collision::CollisionWithMapY(std::shared_ptr<Transform>* position, std::shared_ptr<SDL_Rect> collisionBox)
{
    int rowCount { Engine::getInstance().getMap()->getRowCount() };
    int colCount { Engine::getInstance().getMap()->getColCount() };
    int tileSize { Engine::getInstance().getMap()->getTileSize() };

    //  Position of Collision Box inside the Frame
    int CollisionDeltaY {collisionBox->y - (int)(*position)->m_Y};

    int leftTile {(int)(collisionBox->x +1) / tileSize};                            // X left collision won't be counted as Y collision as well  
    int rightTile {(int)((collisionBox->x + collisionBox->w -1) / tileSize)};               // X right collision won't be counted as Y collision as well 
    int upperTile {(int)(collisionBox->y / tileSize)};
    int lowerTile {(int)((collisionBox->y + collisionBox->h) / tileSize)};

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
                (*position)->m_Y = lowerTile*tileSize - collisionBox->h - CollisionDeltaY;  //  Collsiion with intersection by 1 pixel
                return true;
            }
        }
    }

    return false;
}



