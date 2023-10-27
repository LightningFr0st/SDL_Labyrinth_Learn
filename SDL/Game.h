#pragma once
#include "SDL.h"
#include "Maze.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class ColliderComponent;

class Game {

public:
	std::vector<std::vector<int>> curPath;
	std::vector<Cell> curGrid;
	static bool isRunning; // variable for main game loop
	Game(); // constructor (undefined)
	~Game(); // destructor (undefined)
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen); // method for game initialization
	void handleEvents(); // method for handling events
	void update(); // method for updating all entities
	void render(); // method for rendering all entities
	void clean(); // method for cleanzing all entities

	bool running() { return isRunning; } // method for checking is game currently running
	static SDL_Renderer* renderer; // renderer 
	static SDL_Event event; // events
	static SDL_Rect camera; // rectangle for camera

	enum groupLabel : std::size_t { // Enumeration for different game objects for properly rendering
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
	};

private:
	int startingNode;
	int CurLevel;
	Maze* maze;
	SDL_Window* window; // SDL_Window
};

