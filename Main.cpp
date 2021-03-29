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
enum class State { kEmpty, kObstacle, kClosed, kPath, kStart, kFinish };

// directional deltas
const int delta[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

//Read cell state and return character 
string CellString(State cell)
{
	switch (cell)
	{
		case State::kObstacle: 
			return "X"; //Obstacle
			break;
        case State::kStart:
            return "S"; //Start Cell
            break;
        case State::kFinish:
            return "F"; //Finish Cell
            break;
        case State::kPath:
            return "P"; //Path
            break;
		default:
			return "O"; //Empty Cell
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
    //Compare function will sort this list descendingly.
	sort(v->begin(), v->end(), Compare);
}

/**
 * Check validity of cells on the grid
 */
bool CheckValidCell(int x, int y, const vector<vector<State>> &grid)
{
    if ((x >= 0 && x < grid.size())
        && (y >= 0 && y < grid[0].size())
        && grid[x][y] == State::kEmpty
        )
    {
        return true;
    }
        return false;
}

/**
 * Function to expand neighbors
 */
void ExpandNeighbors(const vector<int>& current, int goal[2], vector<vector<int>>& openlist, vector<vector<State>>& grid)
{

    // TODO: Get current node's data.
    int x1 = current[0];
    int y1 = current[1];
    int g1 = current[2];

    //Loop through current node's potential neighbors.
    for (int i = 0; i<4; i++)
    {
        int x2 = x1 + delta[i][0]; // 0,0 1,0 2,0 3,0
        int y2 = y1 + delta[i][1]; // 0,1 1,1 2,1 3,1
    
        //Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
        if (CheckValidCell(x2, y2, grid))
        {
            //Increment g value, compute h value, and add neighbor to open list.
            int g2 = g1 + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, openlist, grid);
        }
    }
}

/**
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2])
{
	//Create a vector with solution of A* Search
	vector<vector<State>> solution{};

	//Create vector of open nodes. Node formate {x,y,g,h}
	vector<vector<int>> open{};

	//Initialize the starting node.
	int x = init[0];
	int y = init[1];
	int g = 0;
	int h = Heuristic(x, y, goal[0], goal[1]);

	//List of open nodes and grid are passed by reference to be changed by AddToOpen function
	AddToOpen(x, y, g, h, open, grid);

	//while open vector is non empty 
	while (open.empty() == false)
	{
		//Sort the open list descendingly using `CellSort`, and get the current node.
		CellSort(&open);
		//Get the x and y values from the current node,
		auto current_node = open.back();
		open.pop_back();
		x = current_node[0];
		y = current_node[1];

        //Check start node
        if (x == init[0] && y == init[1])
        {
            //Set first node start position to kStart
            grid[x][y] = State::kStart;
        }
        else
        {
            // and set grid[x][y] to kPath.
            grid[x][y] = State::kPath;
        }

		//Check if you've reached the goal. If so, return grid.
		if (x == goal[0] && y == goal[1])
		{
            grid[x][y] = State::kFinish;
			return grid;
		}
		else
		{
			// ExpandNeighbors
            ExpandNeighbors(current_node, goal, open, grid);
		}
	}
	cout << "No path found! \n";
	return solution;
}


/*START TEST*/
void PrintVector(vector<int> v) {
    cout << "{ ";
    for (auto item : v) {
        cout << item << " ";
    }
    cout << "}" << "\n";
}
void PrintVectorOfVectors(vector<vector<int>> v) {
    for (auto row : v) {
        cout << "{ ";
        for (auto col : row) {
            cout << col << " ";
        }
        cout << "}" << "\n";
    }
}
void PrintVectorOfVectors(vector<vector<State>> v) {
    for (auto row : v) {
        cout << "{ ";
        for (auto col : row) {
            cout << CellString(col) << " ";
        }
        cout << "}" << "\n";
    }
}
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
    return;
}
void TestAddToOpen() {
    cout << "----------------------------------------------------------" << "\n";
    cout << "AddToOpen Function Test: ";
    int x = 3;
    int y = 0;
    int g = 5;
    int h = 7;
    vector<vector<int>> open{ {0, 0, 2, 9}, {1, 0, 2, 2}, {2, 0, 2, 4} };
    vector<vector<int>> solution_open = open;
    solution_open.push_back(vector<int>{3, 0, 5, 7});
    vector<vector<State>> grid{ {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kEmpty, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty, State::kObstacle, State::kEmpty} };
    vector<vector<State>> solution_grid = grid;
    solution_grid[3][0] = State::kClosed;
    AddToOpen(x, y, g, h, open, grid);
    if (open != solution_open) {
        cout << "failed" << "\n";
        cout << "\n";
        cout << "Your open list is: " << "\n";
        PrintVectorOfVectors(open);
        cout << "Solution open list is: " << "\n";
        PrintVectorOfVectors(solution_open);
        cout << "\n";
    }
    else if (grid != solution_grid) {
        cout << "failed" << "\n";
        cout << "\n";
        cout << "Your grid is: " << "\n";
        PrintVectorOfVectors(grid);
        cout << "\n";
        cout << "Solution grid is: " << "\n";
        PrintVectorOfVectors(solution_grid);
        cout << "\n";
    }
    else {
        cout << "passed" << "\n";
    }
    return;
}
void TestCompare() {
    cout << "----------------------------------------------------------" << "\n";
    cout << "Compare Function Test: ";
    vector<int> test_1{ 1, 2, 5, 6 };
    vector<int> test_2{ 1, 3, 5, 7 };
    vector<int> test_3{ 1, 2, 5, 8 };
    vector<int> test_4{ 1, 3, 5, 7 };
    if (Compare(test_1, test_2)) {
        cout << "failed" << "\n";
        cout << "\n" << "a = ";
        PrintVector(test_1);
        cout << "b = ";
        PrintVector(test_2);
        cout << "Compare(a, b): " << Compare(test_1, test_2) << "\n";
        cout << "Correct answer: 0" << "\n";
        cout << "\n";
    }
    else if (!Compare(test_3, test_4)) {
        cout << "failed" << "\n";
        cout << "\n" << "a = ";
        PrintVector(test_3);
        cout << "b = ";
        PrintVector(test_4);
        cout << "Compare(a, b): " << Compare(test_3, test_4) << "\n";
        cout << "Correct answer: 1" << "\n";
        cout << "\n";
    }
    else {
        cout << "passed" << "\n";
    }
    return;
}
void TestSearch() {
    cout << "----------------------------------------------------------" << "\n";
    cout << "Search Function Test (Partial): ";
    int goal[2]{ 4, 5 };
    //Path to input file
    const string filePath = "C:/Users/oelhalawany/source/repos/Udacity-L1/InputFiles/InputBoard.txt";
    auto board = ReadBoardFile(filePath);

    std::cout.setstate(std::ios_base::failbit); // Disable cout
    auto output = Search(board, goal, goal);
    std::cout.clear(); // Enable cout

    vector<vector<State>> solution{ {State::kEmpty, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kEmpty, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kEmpty, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kEmpty, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty, State::kObstacle, State::kPath} };

    if (output != solution) {
        cout << "failed" << "\n";
        cout << "Search(board, {4,5}, {4,5})" << "\n";
        cout << "Solution board: " << "\n";
        PrintVectorOfVectors(solution);
        cout << "Your board: " << "\n";
        PrintVectorOfVectors(output);
        cout << "\n";
    }
    else {
        cout << "passed" << "\n";
    }
    cout << "----------------------------------------------------------" << "\n";
}
void TestCheckValidCell() {
    cout << "----------------------------------------------------------" << "\n";
    cout << "CheckValidCell Function Test: ";
    vector<vector<State>> grid{ {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kClosed, State::kEmpty, State::kEmpty, State::kObstacle, State::kEmpty} };

    if (CheckValidCell(0, 0, grid)) {
        cout << "failed" << "\n";
        cout << "\n" << "Test grid is: " << "\n";
        PrintVectorOfVectors(grid);
        cout << "Cell checked: (0, 0)" << "\n";
        cout << "\n";
    }
    else if (!CheckValidCell(4, 2, grid)) {
        cout << "failed" << "\n";
        cout << "\n" << "Test grid is: " << "\n";
        PrintVectorOfVectors(grid);
        cout << "Cell checked: (4, 2)" << "\n";
        cout << "\n";
    }
    else {
        cout << "passed" << "\n";
    }
    cout << "----------------------------------------------------------" << "\n";
}
void TestExpandNeighbors() {
    cout << "----------------------------------------------------------" << "\n";
    cout << "ExpandNeighbors Function Test: ";
    vector<int> current{ 4, 2, 7, 3 };
    int goal[2]{ 4, 5 };
    vector<vector<int>> open{ {4, 2, 7, 3} };
    vector<vector<int>> solution_open = open;
    solution_open.push_back(vector<int>{3, 2, 8, 4});
    solution_open.push_back(vector<int>{4, 3, 8, 2});
    vector<vector<State>> grid{ {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kObstacle, State::kEmpty, State::kEmpty, State::kEmpty, State::kEmpty},
                              {State::kClosed, State::kClosed, State::kEmpty, State::kEmpty, State::kObstacle, State::kEmpty} };
    vector<vector<State>> solution_grid = grid;
    solution_grid[3][2] = State::kClosed;
    solution_grid[4][3] = State::kClosed;
    ExpandNeighbors(current, goal, open, grid);
    CellSort(&open);
    CellSort(&solution_open);
    if (open != solution_open) {
        cout << "failed" << "\n";
        cout << "\n";
        cout << "Your open list is: " << "\n";
        PrintVectorOfVectors(open);
        cout << "Solution open list is: " << "\n";
        PrintVectorOfVectors(solution_open);
        cout << "\n";
    }
    else if (grid != solution_grid) {
        cout << "failed" << "\n";
        cout << "\n";
        cout << "Your grid is: " << "\n";
        PrintVectorOfVectors(grid);
        cout << "\n";
        cout << "Solution grid is: " << "\n";
        PrintVectorOfVectors(solution_grid);
        cout << "\n";
    }
    else {
        cout << "passed" << "\n";
    }
    cout << "----------------------------------------------------------" << "\n";
    return;
}
/*END TEST*/

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
	
    //Print Borad
    PrintBoard(board);
    cout << "---------------------------------------------\n";

	//Call A* Serach function
	auto solution = Search(board, start, goal);

	//Print Path from start to goal
	PrintBoard(solution);

	/*** TESTS ***/
	//TestHeuristic();
	//TestAddToOpen();
	//TestCompare();
	//TestSearch();
    //TestCheckValidCell();
    //TestExpandNeighbors();
}