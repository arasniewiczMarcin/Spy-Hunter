#pragma once
#include<time.h>
#include<stdlib.h>
#include<Windows.h>

#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define FRIENDLY_CARS_SPEED 400
#define SPY_CARS_SPEED 600
#define ENEMY_CARS_SPEED 750
#define BULLET_SPEED 2000
#define TIME_WITH_UNLIMITED_CARS 20
#define MAX_SPEED 1000
#define POINTS_EVERY_DISTANCE 1000

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


struct objects {
	int xSpeed;
	double xPosition;
	double speed;
	double distance;
	SDL_Surface* surface;
	int carsLeft;
};
struct timeMeasuring {
	int t1;
	int t2;
	double delta;
	double worldTime;
	double timeLeft;
};
struct generally {
	char text[128];
	int quit;
	int rc;
	int points;
	double metres;
};


int bmpError(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer,
	SDL_Surface* charset, SDL_Surface* player, SDL_Surface* road, SDL_Surface* spy, SDL_Surface* npc, SDL_Surface* tree);

void initializeCars(timeMeasuring& time, objects friendlyCar[], objects spyCar[], objects enemyCar[],
	SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar);

void newGame(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& gun, int& points, timeMeasuring& time,
	SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar, double& milestone);

void initializeBullets(objects bullet[], objects& playerCar);

void npcBlowUp(objects& playerCar, objects& car, SDL_Surface* boom, double& milestone, int direction, int& points);

double getFriendlyCarY(double playerSpeed, double friendlyCarY, double friendlyCarSpeed);

void addDistanceAndPoints(objects& player, objects friendlyCar[], objects spyCar[],
	objects enemyCar[], objects bullet[], double& milestone, int& points, double delta, bool& added);

int getRandomCarX(int time);

int getRandomCarXOnInitialize(int time);

int getRandomCarDistance(int time);

int getRandomCarDistanceOnInitialize(int time);

int getRandomCarDistanceInFrontOfPlayer(int time);

void blowUp(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& gun,
	timeMeasuring& time, int& points, objects& car, SDL_Surface* boom, SDL_Surface* screen, SDL_Surface* charset,
	SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar, double& milestone);

void collisionWithoutBlowUp(objects& car1, objects& car2, SDL_Surface* boom, int direction, double& milestone, int& points);

void collision(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& gun,
	timeMeasuring& time, int& points, objects& car, SDL_Surface* boom, SDL_Surface* screen, SDL_Surface* charset,
	SDL_Texture* scrtex, SDL_Renderer* renderer, double& milestone, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar);

void checkColisions(objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& playerCar, objects& gun, SDL_Surface* boom, SDL_Surface* screen, SDL_Surface* charset,
	SDL_Texture* scrtex, SDL_Renderer* renderer, timeMeasuring& time, int& points, double& milestone, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar);

void checkIfHit(objects friendlyCar[], objects spyCar[], objects enemyCar[], objects bullet[], objects& playerCar, SDL_Surface* boom,
	double& milestone, int direction, int& points);

bool checkIfThereIsColision(objects& car1, objects& car2);

void attackPlayer(objects enemyCar[], objects& player);

bool checkIfPlayerIsOffTheRoad(objects playerCar);

void addLimitedCars(int points, bool& added, objects& playerCar);

void freeSurfaces(SDL_Surface* charset, SDL_Surface* screen, SDL_Surface* road, SDL_Surface* boom,
	SDL_Surface* fCar, SDL_Surface* sCar, SDL_Surface* eCar, SDL_Surface* gunS, SDL_Surface* playerCar,
	SDL_Surface* trees, objects bullet[], SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Window* window);
