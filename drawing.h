#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720
#define ROAD_DOWN_BORDER 650
#define ROAD_TOP_BORDER 100
#define NAME_X_POSITION	10
#define NAME_Y_POSITION	10
#define POINTS_Y_POSITION 26
#define X_VERTICAL_LINE 0
#define Y_VERTICAL_LINE 1
#define ROAD_LINE_SHIFT 120


void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset);

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor);

double getRoadLinesPosition(double speed, int lineNum, double& currentLinePosition, int lineHeight);

void drawRoadlines(SDL_Surface* screen, int x, double linesPosition[], int l, int k, Uint32 color);
