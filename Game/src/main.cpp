#include "Abstractions/Environment.hpp"
#include "Application.hpp"

int main(int argc, char **argv) {

	Environment::init();

	Application app {};

	try {
		app.run();
	} catch (const std::exception& ex) {
		LOGE(__FUNCTION__, "%s", ex.what())
		Environment::destroy(EXIT_FAILURE);
	}

	Environment::destroy();

	return EXIT_SUCCESS;
}
