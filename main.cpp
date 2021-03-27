#define SDL_MAIN_HANDLED
#include <iostream>
#include <thread>
#include <chrono>
#include "Chip8.h"
#include "Screen.h"


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Incorrect number of arguments." << '\n'
				  << "Correct syntax is \"RealChip8 <rom>\"";
		return 1;
	}

	Chip8 myChip8;
	if (!myChip8.loadRom(argv[1])) return 1;
	Screen chip8Display(argv[1], 64, 32, 15);

	int status = 0;
	while(status != -1)
	{
		status = chip8Display.parseInput(myChip8.keys);

		if (status == 1) myChip8.reset();
		if (status == 2) myChip8.paused = !myChip8.paused;
		if (myChip8.paused) continue;

		myChip8.emulateCycles(9);
		if (myChip8.drawFlag)
		{
			chip8Display.updateScreen(myChip8.screen);
			myChip8.drawFlag = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
	}

	return 0;
}
