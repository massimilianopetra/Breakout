#include <iostream>
#include <string>
#include "gameobject.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

class Player : public GameObject
{
	public:
		
		void update(int x);
		void clean();
};

class Ball : public GameObject
{
	public:
		
		int update();
		void bounce();
		void start();
		void move();
		void speedup();
		void clean();
	
	private:
		
;
		
};

#endif


