#include "Game.h"
#include "sstream"
#include <random>
#include <chrono>
#include "Windows.h"

std::random_device rd;
std::uniform_int_distribution<int> p(20, 760);


SDL_Rect pointPos{};
SDL_Rect snake{};
SDL_Rect body{};

enum direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
Game::Game()
{
}

void Game::init(const char* title)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL cannot be initialised. SDL ERROR: %s", SDL_GetError());
	}
	else {
		win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
		if (win == nullptr) {
			printf("Window cannot be initialised. SDL ERROR: %s", SDL_GetError());
		}
		ren = SDL_CreateRenderer(win, -1, 0);
		if (ren == nullptr) {
			printf("Renderer cannot be initialised. SDL ERROR: %s", SDL_GetError());
		}
		SDL_SetRenderDrawColor(ren, 0,0,0, 255);
		if (TTF_Init() < 0) {
			printf("TTF cannot be initialised. TTF ERROR: %s", TTF_GetError());
		}
		isRunning = true;
		pointX = p(rd);
		pointY = p(rd);
		snakeElement.push_back({ snakeX, snakeY});
	}
}

void Game::titleRender()
{
	TTF_Font* font;
	font = TTF_OpenFont("Nunito-Black.ttf", 36);
	if (font == nullptr) {
		printf("Font cannot be initialised. TTF ERROR: %s", TTF_GetError());
	}

	SDL_Surface* title;
	SDL_Color titleColor{ 80, 200, 120 };
	title = TTF_RenderText_Solid(font, titleText, titleColor);

	SDL_Texture* titleTexture;
	titleTexture = SDL_CreateTextureFromSurface(ren, title);
	SDL_Rect titlePos{ WINDOW_SIZE / 2 - title->w / 2 ,340,title->w,title->h };
	SDL_RenderCopy(ren, titleTexture, NULL, &titlePos);
	
	font = TTF_OpenFont("Nunito-Black.ttf", 24);
	if (font == nullptr) {
		printf("Font cannot be initialised. TTF ERROR: %s", TTF_GetError());
	}
	SDL_Surface* playSign;
	SDL_Color playColor{ 53,94,59 };
	playSign = TTF_RenderText_Solid(font, playText, playColor);

	SDL_Texture* play;
	play = SDL_CreateTextureFromSurface(ren, playSign);
	SDL_Rect playPos{ WINDOW_SIZE / 2 - playSign->w / 2, 350 + title->h,playSign->w,playSign->h };
	SDL_RenderCopy(ren, play, NULL, &playPos);
}

void Game::snakeMove()
{
	std::pair<int, int> newHead = snakeElement.front();
	switch (moveDir) {
	case UP:
		newHead.second-=SNAKE_VEL;
		break;
	case DOWN:
		newHead.second+=SNAKE_VEL;
		break;
	case LEFT:
		newHead.first -= SNAKE_VEL;
		break;
	case RIGHT:
		newHead.first += SNAKE_VEL;
		break;
	}
	if (newHead.first > WINDOW_SIZE-SNAKE_SIZE || newHead.first < 0 || newHead.second > WINDOW_SIZE-SNAKE_SIZE || newHead.second < 0) {
		collided = true;
	}
	if (snakeElement.size() > 2) {
		for (int i = 1;i<snakeElement.size();i++) {
			if (newHead == snakeElement[i]) {
				collided = true;
			}
		}
	}
	if (newHead.first > (pointX - 20) && newHead.first < (pointX + 13) && newHead.second>(pointY - 20) && newHead.second < (pointY + 13)) {
		pointX = p(rd);
		pointY = p(rd);
		point++;
		snakeElement.insert(snakeElement.begin(), newHead);
	}
	else {
		snakeElement.insert(snakeElement.begin(), newHead);
		snakeElement.pop_back();
	}
}

void Game::snakeRender()
{
	SDL_SetRenderDrawColor(ren, 0, 180, 0, 255);
	for (const auto& segment : snakeElement) {
		snake = { segment.first, segment.second, SNAKE_SIZE, SNAKE_SIZE };
		SDL_RenderFillRect(ren, &snake);
	}
}

void Game::endPage()
{
	pPressed = false;
	point = 0;
	snakeX = WINDOW_SIZE / 2 - SNAKE_SIZE / 2;
	snakeY = WINDOW_SIZE / 2 - SNAKE_SIZE / 2;
	snakeElement.clear();
	snakeElement.push_back({ snakeX, snakeY });
	TTF_Font* font;
	font = TTF_OpenFont("Nunito-Black.ttf", 18);
	if (font == nullptr) {
		printf("Font cannot be initialised. TTF ERROR: %s", TTF_GetError());
	}

	SDL_Surface* endSurf;
	SDL_Color endColor{ 250, 69, 81 };
	endSurf = TTF_RenderText_Solid(font, endRemarks, endColor);

	SDL_Texture* endTexture;
	endTexture = SDL_CreateTextureFromSurface(ren, endSurf);
	SDL_Rect endPos = { WINDOW_SIZE/2 - endSurf->w/2 ,WINDOW_SIZE/2-endSurf->h/2,endSurf->w,endSurf->h };
	SDL_RenderCopy(ren, endTexture, NULL, &endPos);
}

void Game::handleEvents(SDL_Event& e)
{
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			SDL_Quit();
		}
		switch (e.key.keysym.sym) {
		case SDLK_p:
			pPressed = true;
			collided = false;
			break;
		case SDLK_UP:
			moveDir = UP;
			break;
		case SDLK_DOWN:
			moveDir = DOWN;
			break;
		case SDLK_LEFT:
			moveDir = LEFT;
			break;
		case SDLK_RIGHT:
			moveDir = RIGHT;
			break;
		default:
			break;
		}
	}
}

void Game::pointsRender()
{
	TTF_Font* font;
	font = TTF_OpenFont("Nunito-Black.ttf", 18);
	if (font == nullptr) {
		printf("Font cannot be initialised. TTF ERROR: %s", TTF_GetError());
	}

	SDL_Surface* pointSurf;
	std::stringstream strm;
	strm << textPoint << point;
	SDL_Color pointColor{ 72, 120, 170 };
	pointSurf = TTF_RenderText_Solid(font, strm.str().c_str(), pointColor);

	SDL_Texture* pointTexture;
	pointTexture = SDL_CreateTextureFromSurface(ren, pointSurf);
	pointPos = { WINDOW_SIZE - pointSurf->w - 20 ,10,pointSurf->w,pointSurf->h };
	SDL_RenderCopy(ren, pointTexture, NULL, &pointPos);
	
	SDL_Texture* point = IMG_LoadTexture(ren, "points.png");
	SDL_Rect pointPos{ pointX,pointY,pointSize,pointSize };
	SDL_RenderCopy(ren, point, NULL, &pointPos);
}

void Game::render()
{
	SDL_RenderClear(ren);

	if (pPressed == false && collided == false) {
		titleRender();
	}
	if (pPressed == true) {
		snakeRender();
		pointsRender();
		snakeMove();
	}
	if (collided == true) {
		endPage();
	}
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderPresent(ren);
}

void Game::clean()
{
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	
	SDL_Quit();
	exit(0);
}
