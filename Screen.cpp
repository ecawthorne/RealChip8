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

// Quit = -1 | Reset = 1 | Pause = 2
int Screen::parseInput(uint_least8_t* controller)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						return -1;
					case SDLK_F1:
						return 1;
					case SDLK_F2:
						return 2;
					case SDLK_1: controller[0] = 1; break;
					case SDLK_2: controller[1] = 1; break;
					case SDLK_3: controller[2] = 1; break;
					case SDLK_4: controller[3] = 1; break;
					case SDLK_q: controller[4] = 1; break;
					case SDLK_w: controller[5] = 1; break;
					case SDLK_e: controller[6] = 1; break;
					case SDLK_r: controller[7] = 1; break;
					case SDLK_a: controller[8] = 1; break;
					case SDLK_s: controller[9] = 1; break;
					case SDLK_d: controller[0xA] = 1; break;
					case SDLK_f: controller[0xB] = 1; break;
					case SDLK_z: controller[0xC] = 1; break;
					case SDLK_x: controller[0xD] = 1; break;
					case SDLK_c: controller[0xE] = 1; break;
					case SDLK_v: controller[0xF] = 1; break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					case SDLK_1: controller[0] = 0; break;
					case SDLK_2: controller[1] = 0; break;
					case SDLK_3: controller[2] = 0; break;
					case SDLK_4: controller[3] = 0; break;
					case SDLK_q: controller[4] = 0; break;
					case SDLK_w: controller[5] = 0; break;
					case SDLK_e: controller[6] = 0; break;
					case SDLK_r: controller[7] = 0; break;
					case SDLK_a: controller[8] = 0; break;
					case SDLK_s: controller[9] = 0; break;
					case SDLK_d: controller[0xA] = 0; break;
					case SDLK_f: controller[0xB] = 0; break;
					case SDLK_z: controller[0xC] = 0; break;
					case SDLK_x: controller[0xD] = 0; break;
					case SDLK_c: controller[0xE] = 0; break;
					case SDLK_v: controller[0xF] = 0; break;
				}
				break;
		}
	}
	return 0;
}
