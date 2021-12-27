#include <Windows.h>

#define SDL_MAIN_HANDLED

#include "Abstractions/Environment.hpp"
#include "MainGame.hpp"

int main(int argc, char **argv) {

	Environment::init();

	auto game = new MainGame();
	game->run();

	Environment::destroy();

	return EXIT_SUCCESS;
}
