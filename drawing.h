#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"game.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define ROAD_DOWN_BORDER 650
#define ROAD_TOP_BORDER 100
#define NAME_X_POSITION	10
#define NAME_Y_POSITION	10
#define POINTS_Y_POSITION 26
#define X_VERTICAL_LINE 0
#define Y_VERTICAL_LINE 1
#define ROAD_LINE_SHIFT 120
#define ROAD_HEIGHT 560
#define ROAD_WIDTH 1028
#define MARGIN_WIDTH 124
#define MARGIN_UP_HEIGHT 85
#define MARGIN_DOWN_HEIGHT 63
#define DISTANCE_BETWEEN_TREES 250
#define DISTANCE_BETWEEN_FRIENDLY_CARS 2000
#define CAR_WIDTH 48
#define CAR_HEIGHT 69
#define PLAYER_Y_POSITION 480


void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset);

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor);


void drawRoadlines(SDL_Surface* screen, int x, double linesPosition[], int l, int k, Uint32 color);

void drawFriendlyCars(SDL_Surface* screen, objects friendlyCar[], double playerDistance, int time, SDL_Surface* npc);

void drawEnemyCars(SDL_Surface* screen, objects enemyCar[], double playerDistance, int time, SDL_Surface* enemy);

void setColors(SDL_Surface* screen, int& black, int& gray);

void drawTexts(SDL_Surface* screen, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Surface* charset, double worldTime, int points, char* text);

void drawSpyCars(SDL_Surface* screen, objects spyCar[], double playerDistance, int time, SDL_Surface* spy);

void drawTrees(SDL_Surface* screen, SDL_Surface* tree, int x, double playerDistance);

void showStats(SDL_Surface* screen, SDL_Surface* charset, int xPlayerSpeed, int xPlayerPosition, double playerSpeed, double playerDistance, char* text);
