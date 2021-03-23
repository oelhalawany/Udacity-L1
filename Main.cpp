#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
using std::vector;
using std::cout;

//Define ENUM class for states of the board cells
enum class State { kEmpty, kObstacle };

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

/**
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> grid, int start[2], int goal[2])
{

	vector<vector<State>> solution{};

	cout << "No path found! \n";

	return solution;
}

// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
	return (abs(x2 - x1) + abs(y2 - y1));
}

//Test Heuristic function
void TestHeuristic() {
	cout << "----------------------------------------------------------" << "\n";
	cout << "Heuristic Function Test: ";
	if (Heuristic(1, 2, 3, 4) != 4) {
		cout << "failed" << "\n";
		cout << "\n" << "Heuristic(1, 2, 3, 4) = " << Heuristic(1, 2, 3, 4) << "\n";
		cout << "Correct result: 4" << "\n";
		cout << "\n";
	}
	else if (Heuristic(2, -1, 4, -7) != 8) {
		cout << "TestHeuristic Failed" << "\n";
		cout << "\n" << "Heuristic(2, -1, 4, -7) = " << Heuristic(2, -1, 4, -7) << "\n";
		cout << "Correct result: 8" << "\n";
		cout << "\n";
	}
	else {
		cout << "passed" << "\n";
	}
	cout << "----------------------------------------------------------" << "\n";
}

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

}