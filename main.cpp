#include <Windows.h>

#define SDL_MAIN_HANDLED

#include "Core/Env/Environment.hpp"
#include "Game/MainGame.hpp"
#include <rabbitmq-c/amqp.h>

int main(int argc, char **argv) {

	Environment::init();

	auto game = new MainGame();
	game->run();

	Environment::destroy();

	return EXIT_SUCCESS;
}
