#include <iostream>
#include <stdio.h>

#include "game.h"
#include "game_constant.h"
#include "texturemanager.h"
#include "charset.h"

extern int FPS;

Game::Game()
{
	m_pWindow = NULL;
	m_pRenderer = NULL;
	m_bRunning = false;
	m_score = 0;	
	
	newlevel();
}

void Game::newlevel()
{
	for (int r=0;r<6;r++)
		for (int c=0;c<25;c++)
			m_wall[r][c] = 1;
}
bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	// attempt to initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		printf("SDL init success\n");
		
		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		
		if(m_pWindow != NULL) // window init success
		{
			printf("window creation success\n");
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if(m_pRenderer != NULL) // renderer init success
			{
				printf("renderer creation success\n");
				SDL_SetRenderDrawColor(m_pRenderer,0x00,0x00,0x00,0xff);
				
				// Charset
				if(!Charset::Instance()->load("resource/alpha.png", m_pRenderer))
				{
					return false;
				}				
				
				// Player
				if(!TextureManager::Instance()->load("resource/player.png", "player", m_pRenderer))
				{
					return false;
				}
				
				m_pPlayer = new Player();
				m_pPlayer->load(PLAYER_X, PLAYER_Y, PLAYER_WIDTH, PLAYER_HEIGHT, "player");
				
				// Ball	
				if(!TextureManager::Instance()->load("resource/ball.png", "ball", m_pRenderer))
				{
					return false;
				}
				
				m_pBall = new Ball();
				m_pBall->load(400, 200, BALL_WIDTH, BALL_HEIGHT, "ball");
				m_pBall->start();
				m_bounce_cnt = 0;			
				
			}			
			else
			{
				printf("renderer init fail\n");
				return false; // renderer init fail
			}
		}
		else
		{
			printf("window init fail\n");
			return false; // window init fail
		}
	}
	else
	{
		printf("SDL init fail\n");
		return false; // SDL init fail
	}

    srand( SDL_GetTicks() );
    m_state = __BEGIN__;
    m_lives = 3;
    
	printf("init completed\n");
	m_bRunning = true; // everything inited successfully, start the main loop
	
	return true;
}

void Game::render()
{
	SDL_Rect rect;
	
	// clear the renderer to the draw color
	SDL_SetRenderDrawColor(m_pRenderer,0x00,0x00,0x00,0xff);
	SDL_RenderClear(m_pRenderer); 
	
	if (m_state == __BEGIN__)
	{
		// Score
		Charset::Instance()->print("Breakout",120,200,m_pRenderer,6);	
		Charset::Instance()->print("Press enter to Start",150,300,m_pRenderer,2);	
	}
	else
	{
		// Score
		char s[16];
		sprintf(s,"%05d      L:%d",m_score,m_lives);
		std::string str(s);
		Charset::Instance()->print(s,100,10,m_pRenderer,2);
		
		// Playfield
		SDL_SetRenderDrawColor(m_pRenderer,0xba,0xe5,0xff,0xff);
		
		rect.x = 40;
		rect.y = 40;
		rect.w = 10;
		rect.h = 440;
		
		SDL_RenderFillRect(m_pRenderer, &rect);
		
		rect.x = 600;
		SDL_RenderFillRect(m_pRenderer, &rect);
	
		rect.x = 40;
		rect.y = 40;
		rect.w = 560;
		rect.h = 10;	
	
		SDL_RenderFillRect(m_pRenderer, &rect);
		
		// Bricks
		
		
		for (int r=0; r<6;r++)
		{
			switch(r)
			{
				case 0:
				case 1:				
					SDL_SetRenderDrawColor(m_pRenderer,0xf0,0x00,0x00,0xff);
					break;
				
				case 2:
				case 3:
					SDL_SetRenderDrawColor(m_pRenderer,0x00,0xf0,0x00,0xff);
					break;
					
				case 4:
				case 5:
					SDL_SetRenderDrawColor(m_pRenderer,0x00,0x00,0xf0,0xff);
					break;				
				
			}
			
			for (int c=0;c<25;c++)
			{
				rect.x = 51+ c*22;
				rect.y = 80+ r*20;
				rect.w = 20;
				rect.h = 8;
				
				if (m_wall[r][c])
					SDL_RenderFillRect(m_pRenderer, &rect);	
			}
		}
		
		// Player
		m_pPlayer->draw(this->m_pRenderer);
		
		// Ball
		m_pBall->draw(this->m_pRenderer);
	}
	
	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void Game::update()
{
	int update = m_pBall->update();
	
	if (update == 1)
	{
		// Player collision detection
		if ((m_pBall->getX() > (m_pPlayer->getX()-BALL_WIDTH)) && (m_pBall->getX() <= (m_pPlayer->getX()+PLAYER_WIDTH)))
		{
			m_pBall->bounce();
			m_bounce_cnt = (m_bounce_cnt+1) % 20;
			
			if (m_bounce_cnt == 0)
				m_pBall->speedup();
		}	
	}
	else if (update == 2)
	{
		// Ball out of playfield
		m_lives--;
		if (m_lives == 0)
			m_state = __BEGIN__;
			
		m_pBall->start();
	}	
	else if (collision())
	{
		// Bricks collision detection
		m_pBall->bounce();	
		m_bounce_cnt = (m_bounce_cnt+1) % 20;
		
		if (m_bounce_cnt == 0)
		{
			m_pBall->speedup();
		}
		
		// Check level completion
		bool completion = true;
		
		for (int r=0;r<6;r++)
			for (int c=0;c<25;c++)
				if (m_wall[r][c] == 1)
					completion = false;
					
		if (completion)
		{
			newlevel();
		}
	}
}

int Game::collision()
{
	int r,y,c_left,c_right;
	
	c_left = (m_pBall->getX() - 51)/22;
	c_right = (m_pBall->getX() - 51+BALL_WIDTH)/22;
	
	r = -1;
	
	if (m_pBall->getYVelocity() > 0)
	{
		// Moving down
		y = m_pBall->getY()+BALL_HEIGHT-1;
	}
	else
	{
		// Moving up
		y = m_pBall->getY();
	}
	
	for (int i = 0; i<6; i++)
		if ((y >= 80+i*20) && (y <= 88+i*20))
			r = i;

	if (r == -1)
		return 0;
	
	if (m_wall[r][c_left] == 1)
	{
		m_wall[r][c_left] = 0;
		m_score += 5;
		return 1;
	}

	if (m_wall[r][c_right] == 1)
	{
		m_wall[r][c_right] = 0;
		m_score += 5;
		return 1;
	}
		
	return 0;
}

void Game::handleEvents()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				m_bRunning = false;
				break;
			case SDL_MOUSEMOTION:
				onMouseMove(event);
				break;
			case SDL_MOUSEBUTTONDOWN:
				onMousePress(event);
				break;	
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == 13 && m_state == __BEGIN__)
				{
					m_state = __INGAME__;
					m_lives = 3;
				}
						
			default:
				break;
		}
	}
}

void Game::onMouseMove(SDL_Event& event)
{
	//Get the mouse offsets
    int x = event.motion.xrel;
    
    m_pPlayer->update(x);
}

void Game::onMousePress(SDL_Event& event)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
    	//handle a left-click
    	m_pBall->move();
	}
}

void Game::clean()
{
	printf("cleaning game\n");
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}
