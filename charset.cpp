#include "charset.h"


Charset* Charset::s_pInstance = NULL;
SDL_Texture* Charset::s_pTexture = NULL;

bool Charset::load(std::string fileName, SDL_Renderer* pRenderer)
{
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
    
    if(pTempSurface == NULL)
    {
		std::cout << IMG_GetError();
        return false;
    }
    
    s_pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
    
    SDL_FreeSurface(pTempSurface);
    
    if(s_pTexture == NULL)
    {
        return false;
    }
    
    return true;
}

void Charset::draw(unsigned char c, int x, int y, SDL_Renderer *pRenderer, int size, double angle, int alpha, SDL_RendererFlip flip)
{
    SDL_Rect srcRect;
    SDL_Rect destRect;
    
    // If texture is not loaded do nothing
    if (s_pTexture == NULL) return;
    
    srcRect.x = 8 * (c % 16);
    srcRect.y = 8 * (c / 16);
    srcRect.w = 8;
    srcRect.h = 8;
    destRect.w = 8*size;
    destRect.h = 8*size;    
    destRect.x = x;
    destRect.y = y;
    
    SDL_SetTextureAlphaMod(s_pTexture, alpha);
    SDL_RenderCopyEx(pRenderer, s_pTexture, &srcRect, &destRect, angle, 0, flip);
}

void Charset::print(std::string str, int x, int y, SDL_Renderer *pRenderer, int size)
{
	for(int i = 0; i < str.size(); i++,x+=8*size) 
	{
	    draw(str[i],x,y,pRenderer,size);
	}
}
