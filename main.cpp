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
	bool throwBullet = false, paused = false, addedCars = false;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface* road, * boom, * fCar, * sCar, * eCar, * gunS;
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
	objects bullet[3] = {0, 0, 0, -1000, 0};
	objects gun = {};

	

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
		if(i < 3)bullet[i].surface = SDL_LoadBMP("./bullet.bmp");
	}
	fCar = SDL_LoadBMP("./npc.bmp");
	eCar = SDL_LoadBMP("./enemy.bmp");
	sCar = SDL_LoadBMP("./spy.bmp");
	trees.surface = SDL_LoadBMP("./tree.bmp");
	boom = SDL_LoadBMP("./blowup.bmp");
	gun.surface = SDL_LoadBMP("./gun.bmp");
	gunS = SDL_LoadBMP("./gun.bmp");


	if (charset == NULL || gun.surface == NULL || road == NULL || bullet[0].surface == NULL || playerCar.surface == NULL || friendlyCar[0].surface == NULL || enemyCar[0].surface == NULL ||
		spyCar[0].surface == NULL|| trees.surface == NULL || boom == NULL) {
		bmpError(screen, scrtex, window, renderer, charset, playerCar.surface, road, spyCar[0].surface, friendlyCar[0].surface, trees.surface);
		return 1;
	}
	
	SDL_SetColorKey(charset, true, 0x000000);
	setColors(screen, color.black, color.gray);
	newGame(playerCar, friendlyCar, spyCar, enemyCar, gun, general.points, time, fCar, eCar, sCar, milestone);


	while(!general.quit) {
		
		time.t2 = SDL_GetTicks();
		time.delta = (time.t2 - time.t1) * 0.001;
		time.t1 = time.t2;

		//holding points
		if (milestone < 0) {
			tmpTime += time.delta;
			if (tmpTime >= 5) {
				tmpTime = 0;
				milestone = playerCar.distance + 1000;
			}
		}
		//pause
		 if (paused == false) {
			time.worldTime += time.delta;
			if (time.timeLeft > 0)time.timeLeft -= time.delta;
			addDistanceAndPoints(playerCar, friendlyCar, spyCar, enemyCar, bullet, milestone, general.points, time.delta, addedCars);
		}			

		addLimitedCars(general.points, addedCars, playerCar);
		//drawing road bmp
		DrawSurface(screen, road, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

		//drawing trees
		drawTrees(screen, trees.surface, trees.xPosition, playerCar.distance);

		//drawing cars
		drawFriendlyCars(screen, friendlyCar, playerCar.distance, time.t1, fCar);
		drawSpyCars(screen, spyCar, playerCar.distance, time.t1, sCar);
		drawEnemyCars(screen, enemyCar, playerCar.distance, time.t1, eCar);

		//drawing guns to pick up
		drawGun(screen, gun, playerCar.distance, time.t1, gunS);

		//drawing player 
		playerCar.xPosition += playerCar.xSpeed;
		DrawSurface(screen, playerCar.surface, playerCar.xPosition, PLAYER_Y_POSITION);

		//enemy cars attack player
		attackPlayer(enemyCar, playerCar);
		
		checkColisions(friendlyCar, spyCar, enemyCar, playerCar, gun, boom, screen, charset, scrtex, renderer, time, general.points, milestone, fCar, eCar, sCar);

		//checks if player hit something with a gun
		if (checkIfThereIsColision(playerCar, gun) == true) {
			bullet[0].carsLeft = 10;
			gun.distance = -1000;
		}

		if (throwBullet == true) {
			shoot(screen, playerCar, bullet);
		}
		checkIfHit(friendlyCar, spyCar, enemyCar, bullet, playerCar, boom, milestone, UP, general.points);

		//check if player is out of the road
		if(checkIfPlayerIsOffTheRoad(playerCar) == true)
			blowUp(playerCar, friendlyCar, spyCar, enemyCar, gun, time, general.points, playerCar, boom, screen, charset, scrtex, renderer, fCar, eCar, sCar, milestone);
		
		drawTexts(screen, scrtex, renderer, charset, time, general.points, general.text, time.timeLeft, playerCar.carsLeft);

		//handling of events
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							general.quit = 1;
							break;
						case SDLK_p:
							if (paused == true)paused = false;
							else paused = true;
							break;
						case SDLK_UP:
							if(playerCar.speed < MAX_SPEED && paused == false)playerCar.speed += 100;
							break;
						case SDLK_DOWN:
							if (playerCar.speed >= 100 && paused == false) {
								playerCar.speed -= 100;
								if (playerCar.speed < 0.05)playerCar.speed = 0;
							}
							break;
						case SDLK_LEFT:
							if(playerCar.speed != 0 && paused == false)playerCar.xSpeed = -1.0;
							break;
						case SDLK_RIGHT:
							if(playerCar.speed != 0 && paused == false)playerCar.xSpeed = 1.0;
							break;
						case SDLK_n:
							if(paused == false)newGame(playerCar, friendlyCar, spyCar, enemyCar, gun, general.points, time, fCar, eCar, sCar, milestone);
							break;
						case SDLK_SPACE:
							if (paused == false) {
								initializeBullets(bullet, playerCar);
								throwBullet = true;
								bullet[0].carsLeft--;
							}
							break;
					}	
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_LEFT && playerCar.xSpeed == -1.0) playerCar.xSpeed = 0;
					else if (event.key.keysym.sym == SDLK_RIGHT && playerCar.xSpeed == 1.0) playerCar.xSpeed = 0;
					else if (event.key.keysym.sym == SDLK_SPACE);
					break;
				case SDL_QUIT:
					general.quit = 1;
					break;
				};
			};
		};

	// freeing surfaces
	freeSurfaces(charset, screen, road, boom, fCar, sCar, eCar, gunS, playerCar.surface, trees.surface, bullet, scrtex, renderer, window);
	SDL_Quit();
	return 0;
	};
