//
// Created by nunol on 12/24/2021.
//

#include "MainGame.hpp"
#include "Abstractions/Environment.hpp"

#include "GL/glew.h"
#include "Engine/Graphics/ImageLoader.hpp"

MainGame::MainGame() {
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
	_state = GameState::PLAY;
	_time = 0;
	_maxFPS = 60.0f;

	// Init SDL
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_EVERYTHING);
}

MainGame::~MainGame() {
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void MainGame::run() {
	initSystems();

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Assets/PNG/CharacterRight_Standing.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Assets/PNG/CharacterRight_Standing.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "Assets/PNG/CharacterRight_Standing.png");

	loop();
}

void MainGame::initSystems() {

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	_window = SDL_CreateWindow(
		"Game Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_screenWidth,
		_screenHeight,
		SDL_WINDOW_OPENGL
	);

	if (_window == nullptr) {
		LOGE(__FUNCTION__, "Could not initialize SDL Window")
		Environment::destroy(-1);
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		LOGE(__FUNCTION__, "Could not initialize SDL GL Context")
		Environment::destroy(-1);
	}

	if (glewInit() != GLEW_OK) {
		LOGE(__FUNCTION__, "Could not initialize OpenGL ES")
		Environment::destroy(-1);
	};

	LOGD(__FUNCTION__, "OpenGL %s", glGetString(GL_VERSION))

	glClearColor(0, 0, 0.9f, 1.0f);

	SDL_GL_SetSwapInterval(0);

	initShaders();
}

void MainGame::initShaders() {
	_colorProgram.compileShaders(
		"Shaders/ColorShading.vert",
		"Shaders/ColorShading.frag"
	);
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::processInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {

			case SDL_EventType::SDL_QUIT:
				_state = GameState::EXIT;
				break;

			case SDL_EventType::SDL_MOUSEMOTION:
				// LOGD(__FUNCTION__, "Mouse at (%d, %d)", event.motion.x, event.motion.y )
				break;

		}

	}

}

void MainGame::loop() {
	while (_state != GameState::EXIT) {
		// Used for frame time measuring
		R32 startTicks = SDL_GetTicks();

		processInput();
		_time += 0.01;
		draw();
		calculateFPS();

		static S32 frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			LOGD(__FUNCTION__, "%.2f", _fps);
			frameCounter = 0;
		}

		R32 frameTicks = SDL_GetTicks() - startTicks;
		// Limit the fps to the max fps
		if (1000.0f / _maxFPS > frameTicks) {
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}

	}
}

void MainGame::draw() {
	// Set the base depth to 1.0
	glClearDepth(1.0f);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	S32 textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	S32 timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	for (auto& sprite : _sprites) {
		sprite->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	// Swap the buffer and draw everything to the screen
	SDL_GL_SwapWindow(_window);
}

void MainGame::calculateFPS() {

	static const S32 NUM_SAMPLES = 10;
	static R32 frameTimes[NUM_SAMPLES];
	static S32 currentFrame = 0;

	static R32 prevTicks = SDL_GetTicks();
	R32 currentTicks;

	currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	S32 count;

	currentFrame++;
	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	} else {
		count = NUM_SAMPLES;
	}

	R32 frameTimeAverage = 0;

	for(S32 i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0) {
		_fps = 1000.0f / frameTimeAverage;
	} else {
		_fps = 60.0f;
	}
}
