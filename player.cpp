#include "player.h"
#include "game_constant.h"

void Player::update(int x)
{
	m_x += x;
	
	if (m_x < 50)
	{
		m_x = 50;
	}
	else if (m_x > 552)
	{
		m_x = 552;	
	}
}

void Ball::bounce()
{
	m_yvelocity = -m_yvelocity;	
}

void Ball::start()
{
	m_x = 100 + (rand() % 5)*100;
	m_y = 200;
	m_xvelocity = 0;
	m_yvelocity = 0;	
	m_moving = false;
}

void Ball::move()
{
	if (!m_moving)
	{
		m_yvelocity = 1;
		if (rand() % 2)
		{
			m_xvelocity = 1;
		}
		else
		{
			m_xvelocity = -1;
		}
		m_moving = true;	
	}
}

void Ball::speedup()
{
	if (m_xvelocity > 0)
		m_xvelocity++;
	else
		m_xvelocity--;	
	
	if (m_yvelocity > 0)
		m_yvelocity++;
	else
		m_yvelocity--;
}

int Ball::update()
{
	// 0 normal moving; 1 possible player hit; 2 ball out of playfield
	
	m_x += m_xvelocity;
	m_y += m_yvelocity;
	
	if (m_x < 50)
	{
		// Left bounce
		m_x = 50;
		m_xvelocity = -m_xvelocity;	
	}
	else if (m_x > 590)
	{
		// right bounce
		m_x = 590;
		m_xvelocity = -m_xvelocity;	
	}	
	
	if (m_y < 50)
	{
		// Top bounce
		m_y = 50;
		m_yvelocity = -m_yvelocity;	
	}
	else if (((m_y+m_width) >= PLAYER_Y) && (m_y < (PLAYER_Y+PLAYER_HEIGHT)))
	{
		// In player range
		return 1;	
	}
	else if (m_y >= (PLAYER_Y+PLAYER_HEIGHT))
	{
		// Out of playfield
		return 2;
	}
	
	return 0;
}
