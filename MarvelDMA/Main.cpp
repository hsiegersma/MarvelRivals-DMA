#include <iostream>
#include "../DMALibrary/Memory/Memory.h"
#include "Globals.h"
#include <Game.h>
#include <thread>
#include <Render.h>
#include <Aimbot.h>

void refreshMemory()
{
	while (true)
	{
		mem.MemoryPartialRefresh();
		mem.TLBRPartialefresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

int main()
{
	if (!mem.Init(Globals.processName, true, true))
	{
		std::cout << "Failed to initilize DMA" << std::endl;
		system("pause");
		return 1;
	}

	std::cout << "DMA initialized" << std::endl;

	if (!mem.GetKeyboard()->InitKeyboard())
	{
		std::cout << "Failed to initialize keyboard hotkeys through kernel." << std::endl;
		system("pause");
		return 1;
	}

	std::cout << "Keyboard initialized" << std::endl;

	if (!Aimbot::init())
	{
		std::cout << "Failed to initialize kmbox" << std::endl;
		system("pause");
		return 1;
	}

	std::cout << "Kmbox initialized" << std::endl;

	Globals.scatterHandle = mem.CreateScatterHandle();

	mem.FullRefresh();
	std::thread(refreshMemory).detach();

	std::thread(Game::Loop).detach();

	Render render = Render();
	render.Init();
	render.Loop();

	return 0;
}
