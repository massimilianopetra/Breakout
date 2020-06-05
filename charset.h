#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef __CHARSET_H__
#define __CHARSET_H__

class Charset
{
	public:
		
		static Charset* Instance()
	    {
	        if(s_pInstance == NULL)
	        {
	            s_pInstance = new Charset();
	            return s_pInstance;
	        }
	       
	        return s_pInstance;
	    }
	    
	    bool load(std::string fileName, SDL_Renderer* pRenderer);
	    void draw(unsigned char c, int x, int y, SDL_Renderer *pRenderer, int size = 1, double angle = 0, int alpha = 0xff, SDL_RendererFlip flip = SDL_FLIP_NONE);
	    void print(std::string str, int x, int y, SDL_Renderer *pRenderer, int size = 1);
	    
	private:
		
		Charset() {}
	    ~Charset() {}
	    
	    static SDL_Texture* s_pTexture;
	    static Charset* s_pInstance;
};

#endif
