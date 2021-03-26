#pragma once
#include <SDL.h>
#include <cstdint>

class Screen
{
public:
	Screen(const char* romName, int width, int height, int scale);
	~Screen();
	void updateScreen(const uint_least8_t* display);
	int parseInput(uint_least8_t* controller);
private:
	int screenWidth;
	int screenHeight;
	int screenScale;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};