#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <vector>
#include <random>
#include <iostream>


class Game {
private:
	//Window and Renderer Information
	const int WINDOW_SIZE = 800;
	bool isRunning = false;

	SDL_Window* win = nullptr;
	SDL_Renderer* ren = nullptr;

	//Title Page
	const char* titleText = "SNAKE";
	const char* playText = "Press P to Play!";
	bool pPressed = false;

	//Snake
	const int SNAKE_SIZE = 20;
	const int SNAKE_VEL = 8;

	int snakeX = WINDOW_SIZE / 2 - SNAKE_SIZE / 2;
	int snakeY = WINDOW_SIZE / 2 - SNAKE_SIZE / 2;

	int moveDir = 4;

	std::vector<std::pair<int, int>> snakeElement;

	//Points
	const int pointSize = 10;
	int pointX = 0;
	int pointY = 0;

	const char* textPoint = "Points: ";
	int point = 0;

	bool collided = false;

	//EndPage
	const char* endRemarks = "You LOST, "
		"press P to replay, Loser";
public:
	Game();
	void init(const char* title);
	bool running() {
		return isRunning;
	}
	void titleRender();
	void snakeMove();
	void snakeRender();
	void endPage();
	void handleEvents(SDL_Event& e);
	void pointsRender();
	void render();
	void clean();
};