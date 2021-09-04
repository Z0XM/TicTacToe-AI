#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <windows.h>

std::string board;

void gotoxy(short x, short y) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
}

void displayBoard();
int inputMove();
int aiMove(int turn);
char checkWin(std::string& board);

int main()
{
	while (std::cout << "\n Press Enter to Play !  " && std::cin.get()) {
		board = std::string(9, '_');
		int turn = 0;
		bool over = false;

		gotoxy(0, 8); std::cout << "                          ";

		while (!over && turn < 8) {
			displayBoard();

			int index_played;

			if (turn % 2 == 0)index_played = inputMove();
			else index_played = aiMove(turn);

			board[index_played] = turn % 2 ? 'O' : 'X';

			char c = checkWin(board);
			std::string won = "";
			if (c == 'O')won = "Computer";
			else if (c == 'X')won = "Player";
			if (!won.empty()) {
				displayBoard();
				gotoxy(0, 5);
				std::cout << "\n " << won << " has Won !\n";
				over = true;
			}

			++turn;

			if (turn == 8) {
				std::cout << "\n Its a Draw !\n";
			}
		}

		std::cin.get();
	}
}

int searchMove(std::string currentBoard, int turn, std::map<std::string, int>& visitedBoards)
{
	auto it = visitedBoards.find(currentBoard);
	if (it != visitedBoards.end())return it->second;

	if (turn == 8)return 0;

	char c = checkWin(currentBoard);
	if (c == 'O')return 1;
	else if (c == 'X')return -1;

	bool arr[3] = { 0, 0, 0 };
	for (int i = 0; i < currentBoard.size(); i++) {
		if (currentBoard[i] == '_') {
			std::string s(currentBoard);
			s[i] = turn % 2 ? 'O' : 'X';
			int v = searchMove(s, turn + 1, visitedBoards);
			arr[v + 1] = true;
		}
	}


	for (int i = 0; i < 3; i++) {
		int k = (turn % 2) * (1 - i) * 2 + i;
		if (arr[k]) {
			visitedBoards[currentBoard] = k - 1;
			return k - 1;
		}
	}
}

int aiMove(int turn)
{
	srand(time(0));
	int value = -1;
	std::vector<int> positions;
	std::map<std::string, int> visitedBoards;

	for (int i = 0; i < board.size(); i++) {
		if (board[i] == '_') {
			std::string currentBoard = board;
			currentBoard[i] = turn % 2 ? 'O' : 'X';
			int v = searchMove(currentBoard, turn + 1, visitedBoards);
			if (v > value) {
				value = v;
				positions.clear();
				positions.push_back(i);
			}
			else if (v == value) {
				positions.push_back(i);
			}
		}
	}
	return positions[rand() % positions.size()];
}

char checkWin(std::string& board)
{
	bool equal = false;
	char c = '_';
	for (int i = 0; i < 3 && !equal; i++) {
		if (board[3 * i] != '_' && board[3 * i] == board[3 * i + 1] && board[3 * i + 1] == board[3 * i + 2]) {
			equal = true;
			c = board[3 * i];
		}
		else if (board[i] != '_' && board[i] == board[3 + i] && board[3 + i] == board[6 + i]) {
			equal = true;
			c = board[i];
		}
	}
	if (!equal && board[0] != '_' && board[0] == board[4] && board[4] == board[8]) {
		equal = true;
		c = board[0];
	}
	else if (!equal && board[2] != '_' && board[2] == board[4] && board[4] == board[6]) {
		equal = true;
		c = board[2];
	}

	return c;
}

int inputMove()
{
	int y = -1, x = -1;

	gotoxy(1, 5); std::cout << " Enter Y co-ordinate: ";
	gotoxy(1, 6); std::cout << " Enter X co-ordinate: ";

	bool done = false;
	while (!done) {
		while (y < 0 || y > 3) {
			gotoxy(23, 5); std::cout << "               ";
			gotoxy(23, 5);
			std::cin >> y;
		}
		while (x < 0 || x > 3) {
			gotoxy(23, 6); std::cout << "               ";
			gotoxy(23, 6);
			std::cin >> x;
		}

		if (board[(y - 1) * 3 + (x - 1)] == '_')
			done = true;
		else y = x = -1;
	}

	return (y - 1) * 3 + (x - 1);
}

void displayBoard()
{
	gotoxy(0, 1); std::cout << "      1 2 3               ";
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0)std::cout << "\n    " << i / 3 + 1 << ' ';
		std::cout << board[i] << ' ';
	}
	std::cout << '\n';

	gotoxy(1, 5); std::cout << "                             ";
	gotoxy(1, 6); std::cout << "                             ";
}