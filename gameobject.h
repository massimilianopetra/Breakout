#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class GameObject
{
	public:
		
		void load(int x, int y, int width, int height, std::string textureID);
		void draw(SDL_Renderer* pRenderer);
		void update();
		void clean();
		
		int getX() { return m_x;}
		int getY() { return m_y;}
		int getWidth() { return m_width;}
		int getHeight() { return m_height;}		
		void setX(int v) { m_x = v;}
		void setY(int v) { m_y = v;}
		int getXVelocity() { return m_xvelocity;}
		int getYVelocity() { return m_yvelocity;}
		
	protected:

		std::string m_textureID;
		int m_currentFrame; 
		int m_currentRow;
		int m_x;
		int m_y;
		int m_width;
		int m_height;
		int m_xvelocity;
		int m_yvelocity;
		bool m_moving;
};

#endif
