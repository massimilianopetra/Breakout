#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"




const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FPS = 120;
const int DELAY_TIME = 1000.0f / FPS;

Game* g_game = NULL;

int main( int argc, char* args[] )
{
	Uint32 frameStart, frameTime;
	
	g_game = new Game();
	g_game->init("Breakout", 300, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	while(g_game->running())
	{		
		frameStart = SDL_GetTicks();
		
		g_game->handleEvents();
		g_game->update();
		g_game->render();
		
		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime< DELAY_TIME)
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));
		}
	}
	
	g_game->clean();
	return 0;
}
