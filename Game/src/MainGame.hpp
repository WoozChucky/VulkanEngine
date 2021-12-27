//
// Created by nunol on 12/24/2021.
//

#ifndef THIS_GAME_MAINGAME_HPP_
#define THIS_GAME_MAINGAME_HPP_

#include <vector>
#include "SDL.h"
#include "Abstractions/DataTypes.hpp"
#include "Engine/GUI/Sprite.hpp"
#include "Engine/Graphics/GLSLProgram.hpp"
#include "Engine/Graphics/GLTexture.hpp"

class MainGame {

 public:
	MainGame();
	~MainGame();

	void run();



 private:

	enum GameState {
		PLAY, EXIT
	};

	void initSystems();
	void initShaders();
	void processInput();
	void loop();
	void draw();
	void calculateFPS();

	SDL_Window* _window;
	S32 _screenWidth;
	S32 _screenHeight;
	GameState _state;

	R32 _time;
	R32 _fps;
	R32 _maxFPS;
	R32 _frameTime;

	std::vector<Sprite*> _sprites;

	GLSLProgram _colorProgram;

};

#endif //THIS_GAME_MAINGAME_HPP_
