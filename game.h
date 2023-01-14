#pragma once
#include<time.h>
#include<stdlib.h>
#include<Windows.h>

#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define FRIENDLY_CARS_SPEED 200
#define SPY_CARS_SPEED 300
#define ENEMY_CARS_SPEED 400

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
};
struct timeMeasuring {
	int t1;
	int t2;
	double delta;
	double worldTime;
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

void pause(SDL_Event event, double& worldTime, objects& player, objects friendlyCar[], objects spyCar[], objects enemyCar[]);

void initializeCars(timeMeasuring& time, objects friendlyCar[], objects spyCar[], objects enemyCar[],
	SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar);

void newGame(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[], int& points, timeMeasuring& time,
	SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar);

double getFriendlyCarY(double playerSpeed, double friendlyCarY, double friendlyCarSpeed);

void addDistanceAndPoints(objects& player, objects friendlyCar[], objects spyCar[],
	objects enemyCar[], double& milestone, int& points, double delta);

int getRandomCarX(int time);

int getRandomCarXOnInitialize(int time);

int getRandomCarDistance(int time);

int getRandomCarDistanceOnInitialize(int time);

void holdPoints(objects& playerCar, objects& car, SDL_Surface* boom, double& milestone, int direction);

void blowUp(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[],
	timeMeasuring& time, int& points, objects& car, SDL_Surface* boom, SDL_Surface* screen,
	SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar);

void collisionWithoutBlowUp(objects& car1, objects& car2, SDL_Surface* boom, int direction, double& milestone);

void collision(objects& playerCar, objects friendlyCar[], objects spyCar[], objects enemyCar[],
	timeMeasuring& time, int& points, objects& car, SDL_Surface* boom, SDL_Surface* screen,
	SDL_Texture* scrtex, SDL_Renderer* renderer, double& milestone, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar);

void checkColisions(objects friendlyCar[], objects spyCar[], objects enemyCar[], objects& playerCar, SDL_Surface* boom, SDL_Surface* screen,
	SDL_Texture* scrtex, SDL_Renderer* renderer, timeMeasuring& time, int& points, double& milestone, SDL_Surface* fCar, SDL_Surface* eCar, SDL_Surface* sCar);


bool checkIfThereIsColision(objects& car1, objects& car2);

void attackPlayer(objects enemyCar[], objects& player);
