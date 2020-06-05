#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"

#ifndef __GAME_H__
#define __GAME_H__

enum GameState_t { __BEGIN__, __INGAME__};

class Game
{
	public:
		
		Game();
		~Game() {};

		bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
		void render();
		void update();
		void handleEvents();
		void clean();
		bool running() { return m_bRunning; }
		
	private:
		
		// Engine variables
		SDL_Window* m_pWindow;
		SDL_Renderer* m_pRenderer;
		GameState_t m_state;
		bool m_bRunning;
		int m_currentFrame;
		int m_score;
		int m_hiscore;
		
		// Specific variables
		int m_wall[6][25];
		Player*  m_pPlayer;
		Ball*  m_pBall;
		int m_bounce_cnt;
		int m_lives;
		
		// Engine method
		void onMouseMove(SDL_Event& event);
		void onMousePress(SDL_Event& event);
		void newlevel();
		int collision();		
		
};

#endif

