// GameServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "App.h"
#include <iomanip>

int main()
{
	App app;

	std::thread logicThread([&]() {
		app.Init();
		app.Run(); }
	);

	std::cout << "press any key to exit..." << std::endl;;
	getchar();

	app.Stop();
	logicThread.join();

	return 0;
}