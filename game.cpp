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
		enemyCar[i].speed = ENEMY_CARS_SPEED;
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

void initializeBullets(objects bullet[], objects& playerCar) {
	for (int i = 0; i < 3; i++) {
		if (bullet[i].distance < -500) {
			bullet[i].distance = playerCar.distance + 50;
			bullet[i].speed = BULLET_SPEED;
			bullet[i].xPosition = playerCar.xPosition;
		}

	}
}

void newGame(objects &playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& gun, int& points, timeMeasuring& time,
	SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar, double& milestone) {
	playerCar.speed = 0;
	playerCar.distance = 0;
	playerCar.carsLeft = 2;
	playerCar.xPosition = SCREEN_WIDTH / 1.2;

	points = 0;
	milestone = POINTS_EVERY_DISTANCE;
	time.worldTime = 0;
	time.timeLeft = TIME_WITH_UNLIMITED_CARS;

	initializeCars(time, friendlyCar, spyCar, enemyCar, fCar, eCar, sCar);

	gun.distance = 500;
	gun.xPosition = getRandomCarX(time.t1);
	if (gun.xPosition < SCREEN_WIDTH / 2)gun.xPosition += 50;
	else gun.xPosition -= 50;
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
	objects enemyCar[], objects bullet[], double& milestone, int& points, double delta, bool& added) {
	player.distance += player.speed * delta;
	for (int i = 0; i < 5; i++) {
		friendlyCar[i].distance += friendlyCar[i].speed * delta;
		spyCar[i].distance += spyCar[i].speed * delta;
		enemyCar[i].distance += enemyCar[i].speed * delta;
		if (i < 3) bullet[i].distance += bullet[i].speed * delta;
	}
	if (player.distance > milestone && milestone > 0) {
		points += 50;
		milestone += POINTS_EVERY_DISTANCE;
		added = false;
	}
}

int getRandomCarX(int time) {
	return ((time * 27) % (ROAD_WIDTH - MARGIN_WIDTH - CAR_WIDTH / 2)) + MARGIN_WIDTH + CAR_WIDTH / 2;
}
int getRandomCarDistance(int time) {
	return ((time * 27) % DISTANCE_BETWEEN_FRIENDLY_CARS) + 8000;
}
int getRandomCarDistanceInFrontOfPlayer(int time) {
	return ((time * 27) % DISTANCE_BETWEEN_FRIENDLY_CARS) + 2000;
}
int getRandomCarXOnInitialize(int time) {
	return ((time * 177) % (ROAD_WIDTH - MARGIN_WIDTH - CAR_WIDTH / 2)) + MARGIN_WIDTH + CAR_WIDTH / 2;
}

int getRandomCarDistanceOnInitialize(int time) {
	return ((time * 177) % (DISTANCE_BETWEEN_FRIENDLY_CARS * 4));
}

int setColisionDirection(objects& playerCar, objects& car) {
	if (playerCar.xPosition - car.xPosition > CAR_WIDTH - 5)return LEFT;
	else if (car.xPosition - playerCar.xPosition > CAR_WIDTH - 5)return RIGHT;
	else if (playerCar.distance - car.distance > CAR_HEIGHT - 5)return DOWN;
	else return UP;
}

bool checkIfPlayerIsOffTheRoad(objects playerCar) {
	if (playerCar.xPosition > SCREEN_WIDTH - 100 || playerCar.xPosition < 100)return true;
	else return false;
}

void blowUp(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& gun,
	timeMeasuring& time, int& points, objects& car, SDL_Surface* boom, SDL_Surface* screen, SDL_Surface* charset,
	SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar, double &milestone) {

	char text[128];
	DrawSurface(screen, boom, car.xPosition, playerCar.distance - car.distance + PLAYER_Y_POSITION);
	DrawSurface(screen, playerCar.surface, playerCar.xPosition, PLAYER_Y_POSITION);
	DrawSurface(screen, boom, playerCar.xPosition, PLAYER_Y_POSITION);

	//showing time and points on the screen
	if (playerCar.carsLeft == 1) {
		sprintf(text, "AMOUNT OF CARS IS 0 SO ITS GAME OVER");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, POINTS_Y_POSITION + 30, text, charset);
	}

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	Sleep(1000);



	if (time.timeLeft > 0) {
		playerCar.xPosition = SCREEN_WIDTH / 1.2;
		playerCar.speed = 0;
	}
	else if (playerCar.carsLeft > 1) {
		playerCar.xPosition = SCREEN_WIDTH / 1.2;
		playerCar.speed = 0;
		playerCar.carsLeft--;
	}
	else {
		newGame(playerCar, friendlyCar, spyCar, enemyCar, gun, points, time, fCar, eCar, sCar, milestone);
	}
}

void npcBlowUp(objects& playerCar, objects& car, SDL_Surface* boom, double& milestone, int direction, int &points) {
	car.surface = boom;
	if (car.speed == FRIENDLY_CARS_SPEED){
		playerCar.carsLeft = 100;
		milestone = -1;
	}
	else if (car.speed == SPY_CARS_SPEED && milestone > 0) {
		points += 200;
	}
	car.speed = 0;
	if (direction == RIGHT && playerCar.speed < 1100) playerCar.xPosition -= 50;
	else if (direction == LEFT && playerCar.speed < 1100) playerCar.xPosition += 50;

}

void collisionWithoutBlowUp(objects& car1, objects& car2, SDL_Surface* boom, int direction, double& milestone, int& points) {
	switch (direction) {
	case LEFT:
		car2.xPosition -= 5;
		if (car2.xPosition < MARGIN_WIDTH)npcBlowUp(car1, car2, boom, milestone, direction, points);
		break;
	case RIGHT:
		car2.xPosition += 5;
		if (car2.xPosition > ROAD_WIDTH + MARGIN_WIDTH)npcBlowUp(car1, car2, boom, milestone, direction, points);
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

void collision(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[], objects &gun,
	timeMeasuring& time, int& points, objects& car, SDL_Surface* boom, SDL_Surface* screen, SDL_Surface* charset,
	SDL_Texture* scrtex, SDL_Renderer* renderer, double& milestone, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar) {
	int direction = 0;
	//checks if there is collision between cars
	if (checkIfThereIsColision(playerCar, car) == true) {
			//setting direction
		direction = setColisionDirection(playerCar, car);
			//if speed is high cars are going to explode
			if (playerCar.speed - car.speed > 200 && (direction == UP || direction == DOWN)) {
				blowUp(playerCar, friendlyCar, spyCar, enemyCar, gun, time , points, car, boom, screen, charset, scrtex, renderer, fCar, eCar, sCar, milestone);
			}
			//otherwise cars will just collide
			else {
				collisionWithoutBlowUp(playerCar, car, boom, direction, milestone, points);
			}
		}
}

void checkColisions(objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& playerCar, objects& gun, SDL_Surface* boom, SDL_Surface* screen, SDL_Surface* charset,
	SDL_Texture* scrtex, SDL_Renderer* renderer, timeMeasuring &time, int &points, double& milestone, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar) {

	for (int i = 0; i < 5; i++) {
		//checking collisions between player and other cars
		collision(playerCar, friendlyCar, spyCar, enemyCar, gun, time, points, friendlyCar[i], boom, screen, charset, scrtex, renderer, milestone, fCar, eCar, sCar);
		collision(playerCar, friendlyCar, spyCar, enemyCar, gun, time, points, spyCar[i], boom, screen, charset, scrtex, renderer, milestone, fCar, eCar, sCar);
		collision(playerCar, friendlyCar, spyCar, enemyCar, gun, time, points, enemyCar[i], boom, screen, charset, scrtex, renderer, milestone, fCar, eCar, sCar);
	}
	
}



bool checkIfThereIsColision(objects& car1, objects& car2) {
	if ((car1.distance > car2.distance - CAR_HEIGHT) && (car1.distance < car2.distance + CAR_HEIGHT)
		&& (car1.xPosition > car2.xPosition - CAR_WIDTH && car1.xPosition < car2.xPosition + CAR_WIDTH)) {
		return true;
	}
	return false;
}
void checkIfHit(objects friendlyCar[], objects spyCar[], objects enemyCar[], objects bullet[], objects& playerCar, SDL_Surface* boom,
	double& milestone, int direction, int& points) {
	//checking hit with other cars
	for (int i = 0; i < 5; i++) {
		if (checkIfThereIsColision(bullet[0], friendlyCar[i]) == true) {
			bullet[0].speed = 0;
			bullet[0].distance = -1000;
			npcBlowUp(bullet[0], friendlyCar[i], boom, milestone, direction, points);
		}
	}
	for (int i = 0; i < 5; i++) {
		if (checkIfThereIsColision(bullet[0], spyCar[i]) == true) {
			bullet[0].speed = 0;
			bullet[0].distance = -1000;
			npcBlowUp(bullet[0], spyCar[i], boom, milestone, direction, points);
			playerCar.carsLeft++;
		}
	}
	for (int i = 0; i < 5; i++) {
		if (checkIfThereIsColision(bullet[0], enemyCar[i]) == true) {
			bullet[0].distance = -1000;
			bullet[0].speed = 0;
		}
	}


}

void attackPlayer(objects enemyCar[], objects& player) {
	//enemy cars attack player and force him out of the road
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

void addLimitedCars(int points, bool& added, objects& playerCar) {

	if (points % 5000 == 0 && added == false) {
		playerCar.carsLeft++;
		added = true;
	}
}

void freeSurfaces(SDL_Surface* charset, SDL_Surface* screen, SDL_Surface* road, SDL_Surface* boom,
	SDL_Surface* fCar, SDL_Surface* sCar, SDL_Surface* eCar, SDL_Surface* gunS, SDL_Surface* playerCar,
	SDL_Surface* trees, objects bullet[], SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Window* window) {
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(road);
	SDL_FreeSurface(boom);
	SDL_FreeSurface(fCar);
	SDL_FreeSurface(sCar);
	SDL_FreeSurface(eCar);
	SDL_FreeSurface(gunS);
	SDL_FreeSurface(playerCar);
	SDL_FreeSurface(trees);
	for (int i = 0; i < 3; i++) {
		SDL_FreeSurface(bullet[i].surface);
	}
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

