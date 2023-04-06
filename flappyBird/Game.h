#pragma once

#include <SDL.h>
#include <iostream>
#include "SDL_image.h"
#include <vector>
#include "SDL_ttf.h"

struct obstacle
{
	SDL_Texture* up;
	SDL_Texture* down;
	SDL_Rect upRect;
	SDL_Rect downRect;
};

class Game
{
public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int hight, bool fullsecreen);
	void hundleEvents();
	void update();
	void render();
	void clear();

	void initObstacle();
	void moveObstacle(obstacle &tube);
	void keyBoardController();
	void collisionDown();
	void collisionObstacle(obstacle ob);
	void restart();

	bool running() { return isRunning; }

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static int points;
	static bool endGame;

	friend int randomPos();
private:
	int cnt = 0;
	SDL_Window* window;

	SDL_Texture* backGround;
	SDL_Texture* character;
	SDL_Texture* textStart;
	SDL_Texture* textScore;

	TTF_Font* font;

	obstacle tube1, tube2;

};