#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include <map>


struct TextureParams
{
    SDL_Texture* Texture;
    int Width;
    int Height;
};

class TextureManager
{
    TextureManager()
    {}

    ~TextureManager();
    
    static TextureManager* s_TextureManagerInstance;
    std::map<std::string, TextureParams> m_TextureMap;

public:

    static TextureManager* getInstance();

    bool Load(const std::string& id, const std::string& filename, int width, int height, double opacity = 1);  // Load texture into SDL_Surface -> SDL_Texture -> m_TextureMap
    bool LoadTextures(const char* fileSource);
    void Drop(const std::string& id);
    void Clean();

    //  Draws texture from m_TextureMap into render
    //  id - from map; x, y - upper left coords;  
    void Draw(const std::string& id, int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //  Draws a frame from sprite sheet - for animation
    void DrawFrame(const std::string& id, int x, int y, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
    
    //  Draws a tile from a tileset
    void DrawTile(const std::string& tilesetID, int x, int y, int tilesize, int row, int col, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //  Draws an image (usually background)
    void DrawImage(const std::string& id, int x, int y, int width, int height, float speed, SDL_RendererFlip flip = SDL_FLIP_NONE);


    int GetTextureWidth(std::string id) { return m_TextureMap[id].Width; }
    int GetTextureHeight(std::string id) { return m_TextureMap[id].Height; }

};

#endif /* TEXTUREMANAGER_H */
