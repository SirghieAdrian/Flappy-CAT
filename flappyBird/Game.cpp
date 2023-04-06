#include "Game.h"
#include "TextureManager.h"
#include <ctime>
#include <cstdlib>
#include <string.h>
#include <sstream>

int randomPos();
bool pause = 1;

SDL_Rect destRect = { 336,256,100,90 };
SDL_Rect textReact = { 10,10,50,50 };

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
bool Game::isRunning = false;
int Game::points = 0;
bool Game::endGame = false;
char stringPoints[10];

void int_to_char(int num, char* result) {
	int temp = num;
	int len = 0;

	while (temp > 0) {
		len++;
		temp /= 10;
	}

	for (int i = len - 1; i >= 0; i--) {
		result[i] = num % 10 + '0';
		num /= 10;
	}

	result[len] = '\0';
}


Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int hight, bool fullScreen)
{
	unsigned int seed = static_cast<unsigned int>(time(nullptr));
	srand(seed);
	TTF_Init();
	int flags = 0;
	if (fullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystem Initialised..." << std::endl;

		window = SDL_CreateWindow(title,xpos,ypos,width,hight,flags);
		if (window)
		{
			std::cout << "Window created..." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created..." << std::endl;
			isRunning = true;
		}
		else {
			isRunning = false;
		}
	}
	
	backGround = TextureManager::LoadTexture("assets/mapFB.png", renderer);
	
	initObstacle();

	character = TextureManager::LoadTexture("assets/finalCat.png", renderer);
    textStart = TextureManager::LoadTexture("assets/textStart.png", renderer);

	font = TTF_OpenFont("PressStart2P-Regular.ttf", 48);

}

void Game::hundleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}
void Game::update()
{
	int_to_char(points, stringPoints);
	if (endGame) {
		restart();
		pause = 1;
		endGame = false;
	}
	collisionDown();
	collisionObstacle(tube1);
	collisionObstacle(tube2);

	if (destRect.x == tube1.downRect.x || destRect.x == tube2.upRect.x)
	{
		points++;
	}
	keyBoardController();
	if (!pause) {
		destRect.y += 3;
		moveObstacle(tube1);
		if (cnt >= 480)
			moveObstacle(tube2);
		else
			cnt += 2;
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	SDL_RenderCopyEx(Game::renderer, backGround, NULL, NULL, NULL, NULL, SDL_FLIP_NONE);

	TextureManager::Draw(tube1.down,tube1.downRect,SDL_FLIP_NONE);
	TextureManager::Draw(tube1.up, tube1.upRect, SDL_FLIP_VERTICAL);

	TextureManager::Draw(tube2.down, tube2.downRect, SDL_FLIP_NONE);
	TextureManager::Draw(tube2.up, tube2.upRect, SDL_FLIP_VERTICAL);

	SDL_Surface* textSurf = TTF_RenderText_Solid(font, stringPoints, { 255,255,255 });
	textScore = SDL_CreateTextureFromSurface(renderer, textSurf);
	SDL_FreeSurface(textSurf);

	SDL_RenderCopy(renderer, textScore, NULL, &textReact);

	SDL_RenderCopyEx(Game::renderer, character, NULL, &destRect, NULL, NULL, SDL_FLIP_HORIZONTAL);

	if(pause)
		TextureManager::Draw(textStart, { 190,290,420,60 }, SDL_FLIP_NONE);

	SDL_RenderPresent(renderer);
}

void Game::clear()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	TTF_Quit();
	std::cout << "Game cleaned..." << std::endl;
}

void Game::moveObstacle(obstacle &tube)
{
		if (tube.downRect.x > -130 && tube.upRect.x > -130)
		{
			tube.upRect.x-=2;
			tube.downRect.x-=2;
		}
		else
		{
			tube.upRect.x = 830;
			tube.downRect.x = 830;
			tube.downRect.y = randomPos();
			tube.upRect.y = tube.downRect.y - 200 - tube.downRect.h;
		}
}

int randomPos()
{
	int random =rand() % (600-240+1)+240;
	return random;
}

void Game::keyBoardController()
{
	if (Game::event.type == SDL_KEYDOWN)
	{
		switch (Game::event.key.keysym.sym)
		{
		case SDLK_SPACE:
			if(destRect.y-50>=0)
				destRect.y-=50;
			pause = 0;
			break;
		case SDLK_ESCAPE:
			pause = 1;
			break;
		default:
			break;
		}
	}
}

void Game::collisionObstacle(obstacle ob)
{
	if (destRect.x + destRect.h >= ob.downRect.x &&
		ob.downRect.x + ob.downRect.w >= destRect.x &&
		destRect.y + destRect.h >= ob.downRect.y &&
		ob.downRect.y + ob.downRect.h >= destRect.y)
	{ 
		endGame = true;
	}
	if (destRect.x + destRect.h >= ob.upRect.x &&
		ob.upRect.x + ob.upRect.w >= destRect.x &&
		destRect.y + destRect.h >= ob.upRect.y &&
		ob.upRect.y + ob.upRect.h >= destRect.y)
	{
		endGame = true;
	}
}

void Game::initObstacle()
{
	tube1.down = TextureManager::LoadTexture("assets/tub2.png", renderer);
	tube1.up = TextureManager::LoadTexture("assets/tub2.png", renderer);
	tube1.downRect = { 830,randomPos(),128,512 };   //600 min    200 maxx
	tube1.upRect = { 830,tube1.downRect.y - 200 - tube1.downRect.h,128,512 };

	tube2.down = TextureManager::LoadTexture("assets/tub2.png", renderer);
	tube2.up = TextureManager::LoadTexture("assets/tub2.png", renderer);
	tube2.downRect = { 830,randomPos(),128,512 };
	tube2.upRect = { 830,tube2.downRect.y - 200 - tube2.downRect.h,128,512 };
}

void Game::restart()
{
	cnt = 0;
	points = 0;
	SDL_DestroyRenderer(renderer);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		std::cout << "Renderer created..." << std::endl;
		isRunning = true;
	}
	else {
		isRunning = false;
	}
	SDL_Surface* tempSurfaceMap = IMG_Load("assets/mapFB.png");
	backGround = SDL_CreateTextureFromSurface(Game::renderer, tempSurfaceMap);
	SDL_FreeSurface(tempSurfaceMap);

	initObstacle();

	SDL_Surface* tempSurfaceChar = IMG_Load("assets/finalCat.png");
	character = SDL_CreateTextureFromSurface(Game::renderer, tempSurfaceChar); 
	SDL_FreeSurface(tempSurfaceChar);

	textStart = TextureManager::LoadTexture("assets/textStart.png", renderer);

	font = TTF_OpenFont("PressStart2P-Regular.ttf", 48);
	SDL_Surface* textSurf = TTF_RenderText_Solid(font, "10", { 255,255,255 });
	textScore = SDL_CreateTextureFromSurface(renderer, textSurf);
	SDL_FreeSurface(textSurf);

	destRect.x = 336;
	destRect.y = 256;
}

void Game::collisionDown()
{
	if (destRect.y >= 550)
		destRect.y = 550;
}