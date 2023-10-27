#include "Maze.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stack>
#include <queue>
#include <algorithm>
#include <ctime>

Maze::Maze(const int n, const int m, std::string path, int p_level) {

	window = SDL_CreateWindow("Expert System", 0, 50, 800, 952, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Init(SDL_INIT_VIDEO);
	level = p_level;
	rows = n;
	cols = m;
	mazePath = path;
	visited.resize(rows * cols);
	fill(visited.begin(), visited.end(), false);

	adjList.resize(rows * cols);
	grid.resize(rows * cols);

	escape.resize(2);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid[this->getInd(i, j)].maze = this;
			grid[this->getInd(i, j)].i = i;
			grid[this->getInd(i, j)].j = j;
		}
	}
}

Maze::~Maze() {}

int Maze::getUID() {
	int res = rand() % (rows * cols);
	while (find(check.begin(), check.end(), res) != check.end()) {
		res = rand() % (rows * cols);
	}
	check.push_back(res);
	return res;
}

int Maze::getInd(int i, int j) {
	if (i != -1 && i != rows + 1 && j != -1 && j != cols + 1) {
		return i * cols + j;
	}
	else {
		return -1;
	}
}

int Maze::statGetInd(int i, int j, int n, int m) {
	if (i != -1 && i != n + 1 && j != -1 && j != m + 1) {
		return i * m + j;
	}
	else {
		return -1;
	}
}

bool Maze::visCheck(int i, int j) {
	return visited[this->getInd(i, j)];
}

void Maze::remWalls(Cell* cur, Cell* next) {
	int ind_a = getInd(cur->i, cur->j);
	int ind_b = getInd(next->i, next->j);

	adjList[ind_a].push_back(ind_b);
	adjList[ind_b].push_back(ind_a);

	if (ind_b - ind_a == 1) {
		next->left = false;
		cur->right = false;
	}
	else if (ind_b - ind_a == -1) {
		cur->left = false;
		next->right = false;
	}
	else if (ind_b - ind_a > 1) {
		cur->bottom = false;
		next->top = false;
	}
	else if (ind_b - ind_a < -1) {
		cur->top = false;
		next->bottom = false;
	}
}

void Maze::generation() {
	srand(time(NULL));
	// maze-generating algorithm
	Cell* cur = &grid[0];
	visited[0] = true;
	std::stack<Cell*> stack_cell;
	stack_cell.push(cur);
	while (!stack_cell.empty()) {
		cur = stack_cell.top();
		stack_cell.pop();
		Cell* next = cur->getNext(grid);
		if (next != nullptr) {
			stack_cell.push(cur);
			remWalls(cur, next);
			visited[getInd(next->i, next->j)] = true;

			stack_cell.push(next);
		}
	}

	elev_ind = getUID();
	lad_ind = getUID();
	carg_ind = getUID();
	com_ind = getUID();
	lab_ind = getUID();

	grid[elev_ind].elev = true;
	grid[lad_ind].lad = true;
	grid[lab_ind].lab = true;
	grid[com_ind].com = true;
	grid[carg_ind].carg = true;

}

void Maze::BFS(int start) {
	fill(visited.begin(), visited.end() - 1, false);
	std::queue<int> order;
	distance.resize(rows * cols);
	visited[start] = true;
	distance[start] = 0;
	order.push(start);

	while (!order.empty()) {
		int cur = order.front();
		order.pop();
		for (std::vector<int>::iterator it = adjList[cur].begin(); it != adjList[cur].end(); it++) {
			if (visited[*it]) continue;
			visited[*it] = true;
			distance[*it] = distance[cur] + 1;
			order.push(*it);
		}
	}
}

void Maze::findPath() {

	int cur = elev_ind;
	while (cur != startNode) {
		for (std::vector<int>::iterator it = adjList[cur].begin(); it != adjList[cur].end(); it++) {
			if (distance[*it] - distance[cur] == -1) {
				escape[0].push_back(cur);
				cur = *it;
				break;
			}
		}
	}
	escape[0].push_back(cur);
	reverse(escape[0].begin(), escape[0].end());

	cur = lad_ind;
	while (cur != startNode) {
		for (std::vector<int>::iterator it = adjList[cur].begin(); it != adjList[cur].end(); it++) {
			if (distance[*it] - distance[cur] == -1) {
				escape[1].push_back(cur);
				cur = *it;
				break;
			}
		}
	}
	escape[1].push_back(cur);
	reverse(escape[1].begin(), escape[1].end());
}

void Maze::generateMatrix() {
	ColMatr.resize(rows + 1);
	for (int i = 0; i < rows + 1; i++) {
		ColMatr[i].resize(cols + 1);
		ColMatr[i][0] = "10";
		ColMatr[0][i] = "01";
	}
	ColMatr[0][0] = "00";
	for (int i = 1; i < rows + 1; i++) {
		for (int j = 1; j < cols + 1; j++) {
			std::string type;
			int rw = grid[getInd(i - 1, j - 1)].right;
			int bw = grid[getInd(i - 1, j - 1)].bottom;
			type = std::to_string(rw) + std::to_string(bw);
			ColMatr[i][j] = type;
		}
	}

	TileMatr.resize(rows);

	for (int i = 0; i < rows; i++) {
		TileMatr[i].resize(cols);
		for (int j = 0; j < cols; j++) {
			if (grid[getInd(i, j)].elev)
				TileMatr[i][j] = '1';
			else if (grid[getInd(i, j)].lad)
				TileMatr[i][j] = '2';
			else if (grid[getInd(i, j)].lab)
				TileMatr[i][j] = '3';
			else if (grid[getInd(i, j)].carg)
				TileMatr[i][j] = '4';
			else if (grid[getInd(i, j)].com)
				TileMatr[i][j] = '5';
			else
				TileMatr[i][j] = '0';
		}
	}
}

void Maze::createFile() {

	mazeOut.open("assets/maze.txt", std::ofstream::out, std::ofstream::trunc);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mazeOut << TileMatr[i][j];
			if (j != cols - 1)
				mazeOut << ',';
		}
		mazeOut << '\n';
	}

	mazeOut.put('\n');

	for (int i = 0; i < rows + 1; i++) {
		for (int j = 0; j < cols + 1; j++) {
			mazeOut << ColMatr[i][j];
			if (j != cols)
				mazeOut << ',';
		}
		mazeOut << '\n';
	}

	mazeOut.close();
}

void Maze::clear() {
	for (int i = 0; i < rows * cols; i++) {
		grid[i].null();
		visited[i] = false;
		distance[i] = 0;
		adjList[i].clear();
	}
	escape[0].clear();
	escape[1].clear();
	ColMatr.clear();
	TileMatr.clear();
	check.clear();
}

void Maze::renderMaze() {


	SDL_Rect frame;
	frame.x = 25;
	frame.y = 25;
	frame.w = 750;
	frame.h = 750;

	SDL_Rect cell;
	cell.w = 40;
	cell.h = 40;

	int width = 50;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	// Celar the screen

	SDL_RenderClear(renderer);

	// Draw the maze

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &frame);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int ind = getInd(i, j);

			if (grid[ind].right) {
				SDL_RenderDrawLine(renderer, (j + 1) * width + 25, i * width + 25,
					(j + 1) * width + 25, (i + 1) * width + 25);
			}

			if (grid[ind].bottom) {
				SDL_RenderDrawLine(renderer, j * width + 25, (i + 1) * width + 25,
					(j + 1) * width + 25, (i + 1) * width + 25);
			}

			if (grid[ind].elev || grid[ind].lad) {
				cell.x = (j * 50) + 30;
				cell.y = (i * 50) + 30;
				SDL_RenderDrawRect(renderer, &cell);
			}
		}
	}

	//Draw the path

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for (int i = 1; i < escape[0].size(); i++) {
		int x1 = grid[escape[0][i - 1]].j * width + 25, y1 = grid[escape[0][i - 1]].i * width + 25;
		int x2 = grid[escape[0][i]].j * width + 25, y2 = grid[escape[0][i]].i * width + 25;
		x1 = (x1 + x1 + width) / 2;
		y1 = (y1 + y1 + width) / 2;
		x2 = (x2 + x2 + width) / 2;
		y2 = (y2 + y2 + width) / 2;
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}


	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	for (int i = 1; i < escape[1].size(); i++) {
		int x1 = grid[escape[1][i - 1]].j * width + 25, y1 = grid[escape[1][i - 1]].i * width + 25;
		int x2 = grid[escape[1][i]].j * width + 25, y2 = grid[escape[1][i]].i * width + 25;
		x1 = (x1 + x1 + width) / 2 + 1;
		y1 = (y1 + y1 + width) / 2 + 1;
		x2 = (x2 + x2 + width) / 2 + 1;
		y2 = (y2 + y2 + width) / 2 + 1;
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	TTF_Init();
	char text[10] = "Level : ";
	char char_level = static_cast<char>(level + 48);
	level--;
	TTF_Font* outFont = TTF_OpenFont("C:/Users/Denis/AppData/Local/Microsoft/Windows/Fonts/8bitOperatorPlus8-Regular.ttf", 32);
	text[8] = char_level;
	SDL_Color White = { 255, 255, 255, SDL_ALPHA_OPAQUE };
	SDL_Surface* surfaceText = TTF_RenderText_Solid(outFont,text,White);
	SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	
	SDL_Rect textRect;
	textRect.x = 25;
	textRect.y = 800;
	textRect.w = 300;
	textRect.h = 100;
	
	SDL_RenderCopy(renderer, textureText, NULL, &textRect);

	SDL_RenderPresent(renderer);
}


///
/// Cell implementation
///
Cell::Cell(int p_i, int p_j) {
	i = p_i;
	j = p_j;
}

Cell* Cell::getNext(std::vector<Cell>& p_grid) {
	std::vector<Cell*> neighbours;

	// top cell
	if (i - 1 != -1 && !maze->visCheck(i - 1, j)) {
		neighbours.push_back(&p_grid[maze->getInd(i - 1, j)]);
	}

	// left cell
	if (j - 1 != -1 && !maze->visCheck(i, j - 1)) {
		neighbours.push_back(&p_grid[maze->getInd(i, j - 1)]);
	}

	// right cell
	if (j + 1 != maze->getCols() && !maze->visCheck(i, j + 1)) {
		neighbours.push_back(&p_grid[maze->getInd(i, j + 1)]);
	}

	// bottom cell
	if (i + 1 != maze->getRows() && !maze->visCheck(i + 1, j)) {
		neighbours.push_back(&p_grid[maze->getInd(i + 1, j)]);
	}

	if (neighbours.size() > 0) {
		int r = rand() % neighbours.size();
		return neighbours[r];
	}
	else {
		return nullptr;
	}
}

void Cell::null() {
	top = true;
	left = true;
	bottom = true;
	right = true;

	elev = false;
	lad = false;
	lab = false;
	carg = false;
	com = false;
}
