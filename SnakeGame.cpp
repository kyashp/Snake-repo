#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "Game.h"

Game* loop = nullptr;
SDL_Event e;

int main(int arg, char* argv[]) {

	loop = new Game();

	const int frameRate = 30;
	const int frameDelay = 1000 / frameRate;

	int frameTime;
	loop->init("Snake");

	while (loop->running()) {

		Uint32 frameStart = SDL_GetTicks();
		loop->handleEvents(e);
		loop->render();
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	loop->clean();
	return 0;
}
