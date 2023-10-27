#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include <fstream>
#include <tuple>

class Maze;

class Cell {
public:
	Maze* maze;

	//index
	int i;
	int j;
	
	//walls
	bool top = true;
	bool left = true;
	bool bottom = true;
	bool right = true;
	
	//special 
	bool elev = false;
	bool lad = false;
	bool lab = false;
	bool carg = false;
	bool com = false;

	Cell() = default;
	Cell(int p_i, int p_j);
	Cell* getNext(std::vector<Cell>& p_grid);

	void null();
};


class Maze {
private:
	
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int cols;
	int rows;
	
	std::string mazePath;
	
	//vectors for calcs
	std::vector<bool> visited; //+
	
	std::vector<int> distance; //+
	std::vector<std::vector<int>> adjList; //+

	//output vectros
	std::vector<std::vector<std::string>>ColMatr; //+
	std::vector<std::vector<char>>TileMatr; //+
	std::vector<int> check;
	std::ofstream mazeOut;

public:

	int level;

	int startNode;
	int elev_ind;
	int lad_ind;

	int carg_ind;
	int com_ind;
	int lab_ind;


	std::vector<Cell> grid; //+
	std::vector<std::vector<int>> escape; //+

	Maze() {};
	Maze(const int n, const int m, std::string path, int p_level);
	~Maze();

	int getRows() { return rows; };
	int getCols() { return cols; };

	int getUID();
	int getInd(int i, int j);
	static int statGetInd(int i, int j, int n, int m);
	bool visCheck(int i, int j);
	void remWalls(Cell* prev, Cell* next);
	void generation();
	void BFS(int start);
	void findPath();
	void generateMatrix();
	void createFile();
	void clear();
	void renderMaze();
};

