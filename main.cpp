#define SDL_MAIN_HANDLED
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <SDL.h>
#include "Chip8.h"
#include "Screen.h"

int main(int argc, char* argv[])
{
	Chip8 myChip8;
	Screen chip8Display(argv[1], 64, 32, 15);
	if (!myChip8.loadRom(argv[1])) return 1;

	bool quit = false;
	SDL_Event event;

	while(!quit)
	{

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_F1:
							myChip8.reset();
							break;
						case SDLK_F2:
							myChip8.paused = !myChip8.paused;
						case SDLK_1: myChip8.keys[0] = 1; break;
						case SDLK_2: myChip8.keys[1] = 1; break;
						case SDLK_3: myChip8.keys[2] = 1; break;
						case SDLK_4: myChip8.keys[3] = 1; break;
						case SDLK_q: myChip8.keys[4] = 1; break;
						case SDLK_w: myChip8.keys[5] = 1; break;
						case SDLK_e: myChip8.keys[6] = 1; break;
						case SDLK_r: myChip8.keys[7] = 1; break;
						case SDLK_a: myChip8.keys[8] = 1; break;
						case SDLK_s: myChip8.keys[9] = 1; break;
						case SDLK_d: myChip8.keys[0xA] = 1; break;
						case SDLK_f: myChip8.keys[0xB] = 1; break;
						case SDLK_z: myChip8.keys[0xC] = 1; break;
						case SDLK_x: myChip8.keys[0xD] = 1; break;
						case SDLK_c: myChip8.keys[0xE] = 1; break;
						case SDLK_v: myChip8.keys[0xF] = 1; break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
						case SDLK_1: myChip8.keys[0] = 0; break;
						case SDLK_2: myChip8.keys[1] = 0; break;
						case SDLK_3: myChip8.keys[2] = 0; break;
						case SDLK_4: myChip8.keys[3] = 0; break;
						case SDLK_q: myChip8.keys[4] = 0; break;
						case SDLK_w: myChip8.keys[5] = 0; break;
						case SDLK_e: myChip8.keys[6] = 0; break;
						case SDLK_r: myChip8.keys[7] = 0; break;
						case SDLK_a: myChip8.keys[8] = 0; break;
						case SDLK_s: myChip8.keys[9] = 0; break;
						case SDLK_d: myChip8.keys[0xA] = 0; break;
						case SDLK_f: myChip8.keys[0xB] = 0; break;
						case SDLK_z: myChip8.keys[0xC] = 0; break;
						case SDLK_x: myChip8.keys[0xD] = 0; break;
						case SDLK_c: myChip8.keys[0xE] = 0; break;
						case SDLK_v: myChip8.keys[0xF] = 0; break;
					}
					break;
			}
		}
		if (!myChip8.paused)
		{
			myChip8.emulateCycles(9);
			if (myChip8.drawFlag)
			{
				chip8Display.updateScreen(myChip8.screen);
				myChip8.drawFlag = false;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
	}
	return 0;
}
