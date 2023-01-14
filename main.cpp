#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"drawing.h"
#include"game.h"


#ifdef __cplusplus
extern "C"
#endif


struct colours {
	int black;
	int gray;
};


int main(int argc, char **argv) {
	
	double milestone = 1000;
	double tmpTime = 0;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *road, *player, *npc, *spy, *tree, *boom, *fCar, *sCar, *eCar;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	objects playerCar = {0, SCREEN_WIDTH / 1.2, 0};
	timeMeasuring time = {};
	colours color = {};
	generally general = {};
	objects trees = {0, MARGIN_WIDTH / 2, 0, 0};
	objects friendlyCar[5] = {};
	objects enemyCar[5] = {};
	objects spyCar[5] = {};
	

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	general.rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	                                 &window, &renderer);
	if(general.rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
		};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "SpyHunter");
	SDL_ShowCursor(SDL_DISABLE);

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);

	//loading bitmaps
	charset = SDL_LoadBMP("./cs8x8.bmp");
	road = SDL_LoadBMP("./road.bmp");
	playerCar.surface = SDL_LoadBMP("./hunter.bmp");
	for (int i = 0; i < 5; i++) {
		friendlyCar[i].surface = SDL_LoadBMP("./npc.bmp");
		spyCar[i].surface = SDL_LoadBMP("./spy.bmp");
		enemyCar[i].surface = SDL_LoadBMP("./enemy.bmp");
	}
	fCar = SDL_LoadBMP("./npc.bmp");
	eCar = SDL_LoadBMP("./enemy.bmp");
	sCar = SDL_LoadBMP("./spy.bmp");
	trees.surface = SDL_LoadBMP("./tree.bmp");
	boom = SDL_LoadBMP("./blowup.bmp");

	if (charset == NULL || road == NULL || playerCar.surface == NULL || friendlyCar[0].surface == NULL || enemyCar[0].surface == NULL ||
		spyCar[0].surface == NULL|| trees.surface == NULL || boom == NULL) {
		bmpError(screen, scrtex, window, renderer, charset, playerCar.surface, road, spyCar[0].surface, friendlyCar[0].surface, trees.surface);
		return 1;
	}
	
	SDL_SetColorKey(charset, true, 0x000000);
	setColors(screen, color.black, color.gray);

	newGame(playerCar, friendlyCar, spyCar, enemyCar, general.points, time, fCar, eCar, sCar);


	while(!general.quit) {
		
		time.t2 = SDL_GetTicks();
		time.delta = (time.t2 - time.t1) * 0.001;
		time.t1 = time.t2;
		time.worldTime += time.delta;

		//holding points
		if (milestone < 0) {
			tmpTime += time.delta;
			if (tmpTime >= 5) {
				tmpTime = 0;
				milestone = playerCar.distance + 1000;
			}
		}
			
		addDistanceAndPoints(playerCar, friendlyCar, spyCar, enemyCar, milestone, general.points, time.delta);
		//drawing road bmp
		DrawSurface(screen, road, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		
		showStats(screen, charset, playerCar.xSpeed, playerCar.xPosition, playerCar.speed, playerCar.distance, general.text);

		//drawing trees
		drawTrees(screen, trees.surface, trees.xPosition, playerCar.distance);

		//drawing cars
		drawFriendlyCars(screen, friendlyCar, playerCar.distance, time.t1, fCar);
		drawSpyCars(screen, spyCar, playerCar.distance, time.t1, sCar);
		drawEnemyCars(screen, enemyCar, playerCar.distance, time.t1, eCar);

		//drawing player bmp
		playerCar.xPosition += playerCar.xSpeed;
		DrawSurface(screen, playerCar.surface, playerCar.xPosition, PLAYER_Y_POSITION);

		attackPlayer(enemyCar, playerCar);
		
		checkColisions(friendlyCar, spyCar, enemyCar, playerCar, boom, screen, scrtex, renderer, time, general.points, milestone, fCar, eCar, sCar);

		//sprawdzanko czy wypad³ z drogi
		if (playerCar.xPosition > SCREEN_WIDTH - 100 || playerCar.xPosition < 100) {
			blowUp(playerCar, friendlyCar, spyCar, enemyCar, time, general.points, playerCar, boom, screen, scrtex, renderer, fCar, eCar, sCar);
		}

		drawTexts(screen, scrtex, renderer, charset, time.worldTime, general.points, general.text);

		//handling of events
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							general.quit = 1;
							break;
						case SDLK_p:
							pause(event, time.worldTime, playerCar, friendlyCar, spyCar, enemyCar);
							break;
						case SDLK_UP:
							if(playerCar.speed < 1000)playerCar.speed += 100;
							break;
						case SDLK_DOWN:
							if (playerCar.speed >= 100) {
								playerCar.speed -= 100;
								if (playerCar.speed < 0.05)playerCar.speed = 0;
							}
							break;
						case SDLK_LEFT:
							if(playerCar.speed != 0)playerCar.xSpeed = -1.0;
							break;
						case SDLK_RIGHT:
							if(playerCar.speed != 0)playerCar.xSpeed = 1.0;
							break;
						case SDLK_n:
							newGame(playerCar, friendlyCar, spyCar, enemyCar, general.points, time, fCar, eCar, sCar);
							break;
					}	
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_LEFT && playerCar.xSpeed == -1.0) playerCar.xSpeed = 0;
					else if (event.key.keysym.sym == SDLK_RIGHT && playerCar.xSpeed == 1.0) playerCar.xSpeed = 0;
					break;
				case SDL_QUIT:
					general.quit = 1;
					break;
				};
			};
		};

	// freeing all surfaces
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
	};
