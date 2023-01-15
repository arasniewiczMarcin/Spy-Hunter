#include"drawing.h"
#include"game.h"


// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};



// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};



void drawRoadlines(SDL_Surface* screen, int x, double linesPosition[], int l, int k, Uint32 color) {
	for (int counter = 0; counter < 5; counter++) {
		DrawRectangle(screen, x, linesPosition[counter], l, k, color, color);
	}
		
}
void setColors(SDL_Surface* screen, int& black, int& gray) {
	black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	gray = SDL_MapRGB(screen->format, 0x69, 0x69, 0x69);
}
void drawTexts(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* charset, timeMeasuring time,
	int points, char* text, double timeLeft, int carsLeft) {
	//showing  my name and index number on the screen
	sprintf(text, "Marcin Arasniewicz, 188857");
	DrawString(screen, NAME_X_POSITION, NAME_Y_POSITION, text, charset);

	//showing time and points on the screen
	sprintf(text, "TIME: %.1lf s POINTS: %i", time.worldTime, points);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, POINTS_Y_POSITION, text, charset);

	//showing time left with unlimited amount of cars
	if (time.timeLeft > 0)sprintf(text, "unlimited amount of cars for: %.1lf s", timeLeft);
	else sprintf(text, "unlimited amount of cars for: 0 s");
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, TIME_LEFT_Y_POSITION, text, charset);

	//showing time left with unlimited amount of cars
	if(time.timeLeft > 0)sprintf(text, "amount of cars: Nan");
	else sprintf(text, "amount of cars: %i ", carsLeft);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, TIME_LEFT_Y_POSITION + 10, text, charset);

	//showing implemented elements
	sprintf(text, "a, b, c, d, e, f, g, i, j, k, l, m, n");
	DrawString(screen, SCREEN_WIDTH - 300, SCREEN_HEIGHT - 50, text, charset);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void drawSpyCars(SDL_Surface* screen, objects spyCar[], double playerDistance, int time, SDL_Surface* spy) {
	for (int i = 0; i < 5; i++) {
		if (playerDistance - spyCar[i].distance >= 8000) {
			spyCar[i].xPosition = getRandomCarX(time);
			spyCar[i].distance += getRandomCarDistance(time);
			spyCar[i].surface = spy;
			spyCar[i].speed = SPY_CARS_SPEED;
		}
		else if (spyCar[i].distance - playerDistance >= 10000) {
			spyCar[i].surface = spy;
			spyCar[i].xPosition = getRandomCarX(time);
			spyCar[i].distance = playerDistance + getRandomCarDistanceInFrontOfPlayer(time);
			spyCar[i].speed = SPY_CARS_SPEED;
		}
	}
	for (int i = 0; i < 5; i++) {
		if (playerDistance - spyCar[i].distance + PLAYER_Y_POSITION > MARGIN_UP_HEIGHT + CAR_HEIGHT / 2 &&
			playerDistance - spyCar[i].distance + PLAYER_Y_POSITION < SCREEN_HEIGHT - MARGIN_DOWN_HEIGHT - CAR_HEIGHT / 2)
			DrawSurface(screen, spyCar[i].surface, spyCar[i].xPosition, playerDistance - spyCar[i].distance + PLAYER_Y_POSITION);
	}
}

void drawEnemyCars(SDL_Surface* screen, objects enemyCar[], double playerDistance, int time, SDL_Surface* enemy) {
	for (int i = 0; i < 5; i++) {
		if (playerDistance - enemyCar[i].distance >= 8000) {
			enemyCar[i].xPosition = getRandomCarX(time);
			enemyCar[i].distance += getRandomCarDistance(time);
			enemyCar[i].surface = enemy;
			enemyCar[i].speed = ENEMY_CARS_SPEED;
		}
		else if (enemyCar[i].distance - playerDistance >= 10000) {
			enemyCar[i].surface = enemy;
			enemyCar[i].xPosition = getRandomCarX(time);
			enemyCar[i].distance = playerDistance + getRandomCarDistanceInFrontOfPlayer(time);
			enemyCar[i].speed = ENEMY_CARS_SPEED;
		}
	}
	for (int i = 0; i < 5; i++) {
		if (playerDistance - enemyCar[i].distance + PLAYER_Y_POSITION > MARGIN_UP_HEIGHT + CAR_HEIGHT / 2 &&
			playerDistance - enemyCar[i].distance + PLAYER_Y_POSITION < SCREEN_HEIGHT - MARGIN_DOWN_HEIGHT - CAR_HEIGHT / 2)
			DrawSurface(screen, enemyCar[i].surface, enemyCar[i].xPosition, playerDistance - enemyCar[i].distance + PLAYER_Y_POSITION);
	}
}

void drawFriendlyCars(SDL_Surface* screen, objects friendlyCar[], double playerDistance, int time, SDL_Surface* npc) {
	for (int i = 0; i < 5; i++) {
		if (playerDistance - friendlyCar[i].distance >= 8000) {
			friendlyCar[i].surface = npc;
			friendlyCar[i].xPosition = getRandomCarX(time);
			friendlyCar[i].distance += getRandomCarDistance(time);
			friendlyCar[i].speed = FRIENDLY_CARS_SPEED;
		}
		else if (friendlyCar[i].distance - playerDistance >= 10000) {
			friendlyCar[i].surface = npc;
			friendlyCar[i].xPosition = getRandomCarX(time);
			friendlyCar[i].distance = playerDistance + getRandomCarDistanceInFrontOfPlayer(time);
			friendlyCar[i].speed = FRIENDLY_CARS_SPEED;
		}
	}
	for (int i = 0; i < 5; i++) {
		if (playerDistance - friendlyCar[i].distance + PLAYER_Y_POSITION > MARGIN_UP_HEIGHT + CAR_HEIGHT / 2 &&
			playerDistance - friendlyCar[i].distance + PLAYER_Y_POSITION < SCREEN_HEIGHT - MARGIN_DOWN_HEIGHT - CAR_HEIGHT / 2)
		DrawSurface(screen, friendlyCar[i].surface, friendlyCar[i].xPosition, playerDistance - friendlyCar[i].distance + PLAYER_Y_POSITION);
	}
}

void drawTrees(SDL_Surface* screen, SDL_Surface* tree, int x, double playerDistance) {
	int counter = 0;
	int treeYPosition[4];
	for (int i = MARGIN_UP_HEIGHT; i < ROAD_HEIGHT + MARGIN_UP_HEIGHT; i++) {
		if ((i + (int)playerDistance) % DISTANCE_BETWEEN_TREES == 0 && 
			ROAD_HEIGHT + MARGIN_UP_HEIGHT + MARGIN_DOWN_HEIGHT - i > MARGIN_UP_HEIGHT + 45) {
			treeYPosition[counter] = ROAD_HEIGHT + MARGIN_UP_HEIGHT + MARGIN_DOWN_HEIGHT - i;
			counter++;
		}
	}
	for (int i = 0; i < counter; i++) {
		DrawSurface(screen, tree, x, treeYPosition[i]);
		DrawSurface(screen, tree, SCREEN_WIDTH - x, treeYPosition[i]);
	}
	


}
 
void shoot(SDL_Surface* screen, objects player, objects bullet[]) {
	if (bullet[0].carsLeft > 0) {
		if (bullet[0].distance > player.distance + BULLET_RANGE + 75) {
			bullet[0].distance = -1000;
			bullet[0].speed = 0;
		}
	}
	else if (bullet[0].distance > player.distance + BULLET_RANGE) {
		bullet[0].distance = -1000;
		bullet[0].speed = 0;
	}
	if (player.distance - bullet[0].distance + PLAYER_Y_POSITION > MARGIN_UP_HEIGHT &&
		player.distance - bullet[0].distance + PLAYER_Y_POSITION < SCREEN_HEIGHT - MARGIN_DOWN_HEIGHT) {
		DrawSurface(screen, bullet[0].surface, bullet[0].xPosition, player.distance - bullet[0].distance + PLAYER_Y_POSITION);
	}

}

void drawGun(SDL_Surface* screen, objects& gun, double playerDistance, int time, SDL_Surface* gunS) {
		if (playerDistance - gun.distance >= 5000) {
			gun.xPosition = getRandomCarX(time);
			gun.distance += 10000;
			gun.surface = gunS;
			gun.speed = 0;
		}
		if (playerDistance - gun.distance + PLAYER_Y_POSITION > MARGIN_UP_HEIGHT + CAR_HEIGHT / 2 &&
			playerDistance - gun.distance + PLAYER_Y_POSITION < SCREEN_HEIGHT - MARGIN_DOWN_HEIGHT - CAR_HEIGHT / 2)
			DrawSurface(screen, gun.surface, gun.xPosition, playerDistance - gun.distance + PLAYER_Y_POSITION);
}