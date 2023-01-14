#include"game.h"
#include"drawing.h"

int bmpError(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer,
	SDL_Surface* charset, SDL_Surface* player, SDL_Surface* road, SDL_Surface* spy, SDL_Surface* npc, SDL_Surface* tree) {
	// wczytanie obrazka cs8x8.bmp
	SDL_FreeSurface(screen);
	SDL_FreeSurface(charset);
	SDL_FreeSurface(road);
	SDL_FreeSurface(player);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 1;
}

void pause(SDL_Event event, double& worldTime, objects& player, objects friendlyCar[], objects spyCar[], objects enemyCar[]) {
	int time1 = SDL_GetTicks();
	int quit = 0;
	double distance = player.distance;
	double distanceFriend[5], distanceSpy[5], distanceEnemy[5];
	double distanceFriend2[5], distanceSpy2[5], distanceEnemy2[5];
	for (int i = 0; i < 5; i++) {
		distanceFriend[i] = friendlyCar[i].distance;
		distanceSpy[i] = spyCar[i].distance;
		distanceEnemy[i] = enemyCar[i].distance;
	}
	while (quit == 0) {
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_p) {
				int time2 = SDL_GetTicks();
				double d = (time2 - time1) * 0.001;
				worldTime -= d;
				double distance2 = player.distance;
				double distanceDifference = distance2 - distance;
				player.distance -= distanceDifference;
				player.distance = distance;
				for (int i = 0; i < 5; i++) {
					distanceFriend2[i] = friendlyCar[i].distance;
					distanceSpy2[i] = spyCar[i].distance;
					distanceEnemy2[i] = enemyCar[i].distance;
				}
				for (int i = 0; i < 5; i++) {
					friendlyCar[i].distance -= distanceFriend2[i] + distanceFriend2[i];
					spyCar[i].distance -= distanceSpy2[i] + distanceSpy2[i];
					enemyCar[i].distance -= distanceEnemy2[i] + distanceEnemy2[i];
				}

				quit = 1;
			}
			break;
		}
	}
}

void initializeCars(timeMeasuring &time, objects friendlyCar[], objects spyCar[], objects enemyCar[],
	SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar) {
	for (int i = 0; i < 5; i++) {
		time.t1 = SDL_GetTicks();
		Sleep(15);
		friendlyCar[i].xPosition = getRandomCarXOnInitialize(time.t1);
		friendlyCar[i].distance = getRandomCarDistanceOnInitialize(time.t1) + i * 100;
		friendlyCar[i].speed = FRIENDLY_CARS_SPEED;
		friendlyCar[i].surface = fCar;
	}
	for (int i = 0; i < 5; i++) {
		time.t1 = SDL_GetTicks();
		Sleep(5);
		enemyCar[i].xPosition = getRandomCarXOnInitialize(time.t1);
		enemyCar[i].distance = getRandomCarDistanceOnInitialize(time.t1) + i * 3000;
		enemyCar[i].speed = FRIENDLY_CARS_SPEED;
		enemyCar[i].surface = eCar;

	}
	for (int i = 0; i < 5; i++) {
		time.t1 = SDL_GetTicks();
		Sleep(15);
		spyCar[i].xPosition = getRandomCarXOnInitialize(time.t1);
		spyCar[i].distance = getRandomCarDistanceOnInitialize(time.t1) + i * 100;
		spyCar[i].speed = SPY_CARS_SPEED;
		spyCar[i].surface = sCar;
		
	}
}

void newGame(objects &playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[], int& points, timeMeasuring& time,
	SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar) {
	playerCar.speed = 0;
	playerCar.distance = 0;
	points = 0;
	time.worldTime = 0;
	initializeCars(time, friendlyCar, spyCar, enemyCar, fCar, eCar, sCar);
	playerCar.xPosition = SCREEN_WIDTH / 1.2;
}

double getFriendlyCarY(double playerSpeed, double friendlyCarY, double friendlyCarSpeed) {
	if (playerSpeed > friendlyCarSpeed) {
		return friendlyCarY - friendlyCarSpeed + playerSpeed;
	}
	else if (playerSpeed <= friendlyCarSpeed) {
		return friendlyCarY - friendlyCarSpeed + playerSpeed;
	}

}

void addDistanceAndPoints(objects& player, objects friendlyCar[], objects spyCar[],
	objects enemyCar[], double& milestone, int& points, double delta) {
	player.distance += player.speed * delta;
	for (int i = 0; i < 5; i++) {
		friendlyCar[i].distance += friendlyCar[i].speed * delta;
		spyCar[i].distance += spyCar[i].speed * delta;
		enemyCar[i].distance += enemyCar[i].speed * delta;
	}
	if (player.distance > milestone && milestone > 0) {
		points += 50;
		milestone += 1000;
	}
}

int getRandomCarX(int time) {
	return ((time * 27) % (ROAD_WIDTH - MARGIN_WIDTH - CAR_WIDTH / 2)) + MARGIN_WIDTH + CAR_WIDTH / 2;
}
int getRandomCarDistance(int time) {
	return ((time * 27) % DISTANCE_BETWEEN_FRIENDLY_CARS) + 8000;

}
int getRandomCarXOnInitialize(int time) {
	return ((time * 177) % (ROAD_WIDTH - MARGIN_WIDTH - CAR_WIDTH / 2)) + MARGIN_WIDTH + CAR_WIDTH / 2;
}
int getRandomCarDistanceOnInitialize(int time) {
	return ((time * 177) % (DISTANCE_BETWEEN_FRIENDLY_CARS * 4));

}

void blowUp(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[],
	timeMeasuring& time, int& points, objects& car, SDL_Surface* boom, SDL_Surface* screen,
	SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar) {
	DrawSurface(screen, boom, car.xPosition, playerCar.distance - car.distance + PLAYER_Y_POSITION);
	DrawSurface(screen, boom, playerCar.xPosition, PLAYER_Y_POSITION);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	Sleep(1000);
	newGame(playerCar, friendlyCar, spyCar, enemyCar, points, time, fCar, eCar, sCar);
}
void npcBlowUp(objects& playerCar, objects& car, SDL_Surface* boom, double& milestone, int direction) {
	car.surface = boom;
	car.speed = 0;
	if (car.speed == FRIENDLY_CARS_SPEED){
		milestone = -1;
	}
	if (direction == RIGHT) playerCar.xPosition -= 50;
	else if (direction == LEFT) playerCar.xPosition += 50;

}
void collisionWithoutBlowUp(objects& car1, objects& car2, SDL_Surface* boom, int direction, double& milestone) {
	switch (direction) {
	case LEFT:
		car2.xPosition -= 5;
		if (car2.xPosition < MARGIN_WIDTH)npcBlowUp(car1, car2, boom, milestone, direction);
		break;
	case RIGHT:
		car2.xPosition += 5;
		if (car2.xPosition > ROAD_WIDTH + MARGIN_WIDTH)npcBlowUp(car1, car2, boom, milestone, direction);
		break;
	case UP:
		car1.speed = car2.speed;
		car2.distance += 15;
		break;
	case DOWN:
		car1.speed = car2.speed;
		car2.distance -= 15;
		break;
	}
	
}

void collision(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[],
	timeMeasuring& time, int& points, objects& car, SDL_Surface* boom, SDL_Surface* screen,
	SDL_Texture* scrtex, SDL_Renderer* renderer, double& milestone, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar) {
	int direction = 0;
	//checks if there is collision between cars
	if ((playerCar.distance > car.distance - CAR_HEIGHT) && (playerCar.distance < car.distance + CAR_HEIGHT)
			&& (playerCar.xPosition > car.xPosition - CAR_WIDTH && playerCar.xPosition < car.xPosition + CAR_WIDTH)) {
			if (playerCar.xPosition - car.xPosition > CAR_WIDTH - 5)direction = LEFT;
			else if (car.xPosition - playerCar.xPosition > CAR_WIDTH - 5)direction = RIGHT;
			else if (playerCar.distance - car.distance > CAR_HEIGHT - 5)direction = DOWN;
			else if (car.distance - playerCar.distance > CAR_HEIGHT - 5)direction = UP;
			if (playerCar.speed - car.speed > 200) {
				blowUp(playerCar, friendlyCar, spyCar, enemyCar, time , points, car, boom, screen, scrtex, renderer, fCar, eCar, sCar);
			}
			else {
				collisionWithoutBlowUp(playerCar, car, boom, direction, milestone);
			}
		}
}

void checkColisions(objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& playerCar, SDL_Surface* boom, SDL_Surface* screen,
	SDL_Texture* scrtex, SDL_Renderer* renderer, timeMeasuring &time, int &points, double& milestone, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar) {
	//jesli xgracza jest w wide³kach xsamochodu - car_width/2 do xsamochodu + car_width/2 i playerdistance jest okolo cardistance - car_height / 2 to kolizja
	//jesli speedgracza jest wiekszy o 300 niz speed przeciwnika to wybuchnij oba auta jesli nie to spychaj swoj samochod w bok o troszke
	for (int i = 0; i < 5; i++) {
		collision(playerCar, friendlyCar, spyCar, enemyCar, time, points, friendlyCar[i], boom, screen, scrtex, renderer, milestone, fCar, eCar, sCar);
		collision(playerCar, friendlyCar, spyCar, enemyCar, time, points, spyCar[i], boom, screen, scrtex, renderer, milestone, fCar, eCar, sCar);
		collision(playerCar, friendlyCar, spyCar, enemyCar, time, points, enemyCar[i], boom, screen, scrtex, renderer, milestone, fCar, eCar, sCar);
	}
	
}

bool checkIfThereIsColision(objects& car1, objects& car2) {
	if ((car1.distance > car2.distance - CAR_HEIGHT) && (car1.distance < car2.distance + CAR_HEIGHT)
		&& (car1.xPosition > car2.xPosition - CAR_WIDTH && car1.xPosition < car2.xPosition + CAR_WIDTH)) {
		return true;
	}
	return false;
}

void attackPlayer(objects enemyCar[], objects& player) {
	for (int i = 0; i < 5; i++) {
		if (enemyCar[i].distance < player.distance + 35 && enemyCar[i].distance > player.distance - 50) {
			if (enemyCar[i].xPosition > player.xPosition) {
				enemyCar[i].xPosition -= 2;
				if (checkIfThereIsColision(enemyCar[i], player) == true) {
					if (player.xSpeed == 0) {
						player.xPosition -= 2;
						enemyCar[i].xPosition += 2;
					}
					else if (player.xSpeed < 0) {
						enemyCar[i].xPosition -= 0.2;
						player.xPosition += 0.5;
					}
				}
			}
			else if (enemyCar[i].xPosition < player.xPosition) {
				enemyCar[i].xPosition += 2;
				if (checkIfThereIsColision(enemyCar[i], player) == true) {
					if (player.xSpeed == 0) {
						player.xPosition += 2;
						enemyCar[i].xPosition -= 2;
					}
					else if (player.xSpeed > 0) {
						enemyCar[i].xPosition += 0.2;
						player.xPosition -= 0.5;
					}
				}
			}
		}
	}
}
