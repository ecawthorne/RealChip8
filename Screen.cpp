#include "Screen.h"


Screen::Screen(const char* romName, int width, int height, int scale)
{
	screenWidth = width;
	screenHeight = height;
	screenScale = scale;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		romName,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width * scale,
		height * scale,
		SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width,
		height
	);
}

Screen::~Screen()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Screen::updateScreen(const uint_least8_t* display)
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int rownum = 0;
	SDL_Rect pixel;
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64; ++x)
		{

			pixel.x = x * screenScale;
			pixel.y = y * screenScale;
			pixel.w = screenScale;
			pixel.h = screenScale;
			rownum = y * 64;
			if (display[x + rownum] == 1)
			{
				SDL_RenderFillRect(renderer, &pixel);
			}
		}
	}
	SDL_RenderPresent(renderer);
}
