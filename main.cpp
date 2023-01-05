#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"drawing.h"





// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc, points = 0, xSpeed = 0, playerX = SCREEN_WIDTH / 1.2;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed, speed = 0, roadLinesPosition[5] = { 100, 200, 300, 400, 500 }, metres = 0;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *road, *player, *npc, *spy;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	                                 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
		};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "SpyHunter");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);



	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
		};
	SDL_SetColorKey(charset, true, 0x000000);

	road = SDL_LoadBMP("./road.bmp");
	if (road == NULL) {
		printf("SDL_LoadBMP(road.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(road);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	player = SDL_LoadBMP("./hunter.bmp");
	if (player == NULL) {
		printf("SDL_LoadBMP(hunter.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(player);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	int gray = SDL_MapRGB(screen->format, 0x69, 0x69, 0x69);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	etiSpeed = 1;



	while(!quit) {
		t2 = SDL_GetTicks();

		// w tym momencie t2-t1 to czas w milisekundach,
		// jaki uplyna³ od ostatniego narysowania ekranu
		// delta to ten sam czas w sekundach
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		distance += speed * delta;
		if (distance > 1) {
			distance = 0;
			points += 50;
		}
		//drawing road bmp
		DrawSurface(screen, road, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

		//drawing road lines
		for (int i = 0; i < 5; i++) {
			roadLinesPosition[i] = getRoadLinesPosition(speed, i, roadLinesPosition[i], 50);
		}
		drawRoadlines(screen, SCREEN_WIDTH / 2, roadLinesPosition, 8, 50, gray);

		//drawing player bmp
		playerX += xSpeed;
		DrawSurface(screen, player, playerX , SCREEN_HEIGHT / 1.5);

		fpsTimer += delta;
		if(fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

	
		sprintf_s(text, "Marcin Arasniewicz, 188857");
		DrawString(screen, NAME_X_POSITION, NAME_Y_POSITION, text, charset); 
		//	      "Esc - exit, \030 - faster, \031 - slower"
		sprintf_s(text, "TIME: %.1lf s POINTS: %i", worldTime, points);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, POINTS_Y_POSITION, text, charset);

		//zaimplementowane elementy
		sprintf_s(text, "a, b, d, f");
		DrawString(screen, SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		// obs³uga zdarzeñ (o ile jakieœ zasz³y) / handling of events (if there were any)
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if (event.key.keysym.sym == SDLK_UP && speed <= 1) speed += 0.2;
					else if (event.key.keysym.sym == SDLK_DOWN && speed >= 0.1) {
						speed -= 0.1;
						if (speed < 0.05)speed = 0;
					}
					else if (event.key.keysym.sym == SDLK_LEFT) xSpeed = -1.0;
					else if (event.key.keysym.sym == SDLK_RIGHT) xSpeed = 1.0;					
					else if (event.key.keysym.sym == SDLK_n) {
						speed = 0;
						roadLinesPosition[0] = 100;
						roadLinesPosition[1] = 200;
						roadLinesPosition[2] = 300;
						roadLinesPosition[3] = 400;
						roadLinesPosition[4] = 500;
						distance = 0;
						points = 0;
						metres = 0;
						worldTime = 0;

					}
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_LEFT && xSpeed == -1.0) xSpeed = 0;
					else if (event.key.keysym.sym == SDLK_RIGHT && xSpeed == 1.0) xSpeed = 0;
					break;
				case SDL_QUIT:
					quit = 1;
					break;
				};
			};
		frames++;
		};

	// zwolnienie powierzchni / freeing all surfaces
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
	};
