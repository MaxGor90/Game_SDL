#include "TextureManager.h"
#include "Engine.h"
#include "Camera.h"
#include "tinyxml2.h"


TextureManager& TextureManager::getInstance()
{
    static TextureManager TextureManagerInstance;
    return  TextureManagerInstance;
}

TextureManager::~TextureManager()
{
    Clean();
}

bool TextureManager::Load(const std::string& id, const std::string& filename, int width, int height, double opacity)
{
    SDL_Surface* surface { IMG_Load(filename.c_str()) };
    if( surface == nullptr)
    {
        SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* texture { SDL_CreateTextureFromSurface(Engine::getInstance().getRenderer(), surface) };
    if( texture == nullptr )
    {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        return false;
    }

    SDL_SetTextureAlphaMod(texture, 255 * opacity);     //  Opacity

    m_TextureMap[id] = {texture, width, height};

    return true;
}


//  Loads textures from passed XML file
bool TextureManager::LoadTextures(const char* fileSource)
{
    tinyxml2::XMLDocument file;
    file.LoadFile(fileSource);

    if (file.Error())
    {
        SDL_Log("%s: Failed to load the document %s: %s", SDL_FUNCTION, fileSource, file.ErrorStr());
        return false;
    }

    tinyxml2::XMLElement* root {file.RootElement()};

    for (tinyxml2::XMLElement* el {root->FirstChildElement("texture")}; el != nullptr; el = el->NextSiblingElement("texture"))
    {
        std::string id, source;
        int width, height;
        double opacity;

        id = el->Attribute("id");
        source = el->Attribute("source");
        width = el->IntAttribute("width");
        height = el->IntAttribute("height");
        opacity = (el->FindAttribute("opacity") == nullptr)? 1.0 : el->DoubleAttribute("opacity");

        Load(id, source, width, height, opacity);
    }
    return true;
}

void TextureManager::Draw(const std::string& id, int x, int y, SDL_RendererFlip flip)
{
    SDL_Rect sourceRect = { 0, 0, m_TextureMap[id].Width, m_TextureMap[id].Height };    // Source image   
    SDL_Rect destRect = { x, y, m_TextureMap[id].Width, m_TextureMap[id].Height };      // Destination place for image
    SDL_RenderCopyEx(Engine::getInstance().getRenderer(), m_TextureMap[id].Texture, &sourceRect, &destRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(const std::string& id, int x, int y, int row, int frame, SDL_RendererFlip flip)
{
    int width {m_TextureMap[id].Width};
    int height {m_TextureMap[id].Height};

    SDL_Rect sourceRect = { width * frame, height * (row - 1), width, height };    // Source image in a sheet, raw count from 1 rather than from 0  
    Vector2D cam = Camera::getInstance().GetPosition();
    SDL_Rect destRect = { x - (int)(cam.m_X), y - (int)(cam.m_Y), width, height };      // Destination place for image
    SDL_RenderCopyEx(Engine::getInstance().getRenderer(), m_TextureMap[id].Texture, &sourceRect, &destRect, 0, nullptr, flip);
}

void TextureManager::DrawTile(const std::string& tilesetID, int x, int y, int tilesize, int row, int col, SDL_RendererFlip flip)
{
    SDL_Rect sourceRect = { tilesize * col, tilesize * row, tilesize, tilesize };    // Source image in a tileset, raw count from 0
    Vector2D cam = Camera::getInstance().GetPosition();
    SDL_Rect destRect = { x - (int)(cam.m_X), y - (int)(cam.m_Y), tilesize, tilesize };      // Destination place for image
    SDL_RenderCopyEx(Engine::getInstance().getRenderer(), m_TextureMap[tilesetID].Texture, &sourceRect, &destRect, 0, nullptr, flip);
}


void TextureManager::DrawImage(const std::string& id, int x, int y, int width, int height, float speed, SDL_RendererFlip flip)
{
    SDL_Rect sourceRect = { 0, 0, width, height };    // Source image
    Vector2D cam = Camera::getInstance().GetPosition();
    SDL_Rect destRect = { x - (int)(cam.m_X*speed), y - (int)(cam.m_Y*speed), width, height };      // Destination place for image, moves while char is moving with passed speed
    SDL_RenderCopyEx(Engine::getInstance().getRenderer(), m_TextureMap[id].Texture, &sourceRect, &destRect, 0, nullptr, flip);
}



void TextureManager::Drop(const std::string& id)
{
    SDL_DestroyTexture(m_TextureMap[id].Texture);
    m_TextureMap.erase(id);
}

void TextureManager::Clean()
{
    std::map<std::string, TextureParams>::iterator it;
    for (it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
        SDL_DestroyTexture(it->second.Texture);

    m_TextureMap.clear();

    SDL_Log("TextureManager: Texture map cleared!");
}


