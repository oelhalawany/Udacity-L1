#include <algorithm>  // for sort
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
using std::vector;
using std::cout;

//Define ENUM class for states of the board cells
enum class State { kEmpty, kObstacle, kClosed, kPath};

//Read cell state and return character 
string CellString(State cell)
{
	switch (cell)
	{
		case State::kObstacle: 
			return "X";
			break;
		default:
			return "0";
			break;
	}
}

//Parse line ex: 1,0,0,1,1, including int and char and store store as state
vector<State> ParseLine(string line)
{
	vector<State>row;
	//define Input stream class object to operate on strings
	istringstream my_stream1(line);

	int value, index = 0;
	char c;

	//https://www.cplusplus.com/reference/istream/istream/operator%3E%3E/
	while (my_stream1 >> value >> c) {
		switch (value)
		{
			case 1: 
				row.push_back(State::kObstacle);
				break;
			default:
				row.push_back(State::kEmpty);
				break;
		}
	}

	return row;
}

//Print out board of States in form of chars not State
void PrintBoard(vector<vector<State>>board)
{
	for (int i = 0; i< board.size(); i++)
	{
		for (int j = 0; j < board[i].size(); j++)
		{
			cout << CellString(board[i][j]) << " ";
		}
		cout << "\n";
	}
}

//Read Board from file and store it.
vector<vector<State>> ReadBoardFile(string path) {

	ifstream myfile(path);
	vector<vector<State>> board;
	string line;
	if (myfile) {
		while (getline(myfile, line)) 
		{
			board.push_back(ParseLine(line));
		}
	}
	return board;
}

// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
	return (abs(x2 - x1) + abs(y2 - y1));
}

/**
 * Add a node to the open list and mark it as open.
 */
void AddToOpen(int x, int y, int g, int h, vector<vector<int>>& openNodes, vector<vector<State>>& grid)
{
	vector<int> node = { x,y,g,h };
	openNodes.push_back(node);
	grid[x][y] = State::kClosed;
}

/**
 * Compare the F values of two cells.
 */
bool Compare(vector<int> node1, vector <int> node2)
{
	//Node formate is {x, y, g, h}

	int fvalue_node1 = node1[2] + node1[3];
	int fvalue_node2 = node2[2] + node2[3];

	return fvalue_node1 > fvalue_node2;
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>>* v) {
	sort(v->begin(), v->end(), Compare);
}

/**
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2])
{
	//Create a vector with solution of A* Search
	vector<vector<State>> solution{};

	//Create vector of open nodes
	vector<vector<int>> open{};

	// Initialize the starting node.
	int x = init[0];
	int y = init[1];
	int g = 0;
	int h = Heuristic(x, y, goal[0], goal[1]);
	//List of open nodes and grid are passed by reference to be changed by AddToOpen function
	AddToOpen(x, y, g, h, open, grid);

	// TODO: while open vector is non empty 
	while (open.empty() == false)
	{
		// TODO: Sort the open list using `CellSort`, and get the current node.
		CellSort(&open);
		// TODO: Get the x and y values from the current node,
		auto current_node = open.back();
		open.pop_back();
		x = current_node[0];
		y = current_node[1];

		// and set grid[x][y] to kPath.
		grid[x][y] = State::kPath;

		// TODO: Check if you've reached the goal. If so, return grid.
		if (x == goal[0] && y == goal[1])
		{
			return grid;
		}
		else
		{
			// If we're not done, expand search to current node's neighbors. This step will be completed in a later quiz.
			// ExpandNeighbors
		}
	}

	cout << "No path found! \n";
	return solution;
}


#include "test.cpp"

int main()
{
	//Path to input file
	const string filePath = "C:/Users/oelhalawany/source/repos/Udacity-L1/InputFiles/InputBoard.txt";

	//Define a board of ENUM State
	vector<vector<State>> board;

	//Start and goal for the grid (for A* Search)
	int start[2] = { 0,0 };
	int goal[2] = { 4,5 };

	//Read board from input file and store it as enum State
	board = ReadBoardFile(filePath);
	
	//Call A* Serach function
	auto solution = Search(board, start, goal);

	//Print board
	PrintBoard(solution);

	/*** TESTS ***/
	TestHeuristic();
	TestAddToOpen();
	TestCompare();
	TestSearch();
}