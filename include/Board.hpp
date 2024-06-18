#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <cmath>
#include <iostream>

class Board
{
	size_t size_ = 3;
	const size_t cellSize_ = 200;
	int numberToWin_ = 3;

	char** board_;
	bool** winningCells_;

	sf::Font font;

	void resetWinningCells();

	// Helper method for diagonal traversal
	int minOfThree(int a, int b, int c);

public:
	Board(size_t size, int numberToWin);
	~Board();

	void draw(sf::RenderWindow& window);
	bool makeMove(int row, int col, char player);
	bool isGameFinished();
	char checkGameState();
	void resetBoard();
	int emptyCells();

	// Give access to minimax and its helper methods
	friend int minimax(Board& board, int depth, int maxDepth, bool isMaximizing, int alpha, int beta);
	friend int evaluate(Board& board);
	friend std::pair<int, int> findBestMove(Board& board);
};

// Initialize board and winning table with default values
Board::Board(size_t size, int numberToWin): size_(size), numberToWin_(numberToWin)
{
	board_ = new char* [size_];
	winningCells_ = new bool* [size_];

	for (int i = 0 ; i < size_; i++)
	{
		board_[i] = new char[size_];
		winningCells_[i] = new bool[size_];
	}

	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			// Initialize board with empty values
			board_[i][j] = '0';
			winningCells_[i][j] = false;
		}
	}

	font.loadFromFile("C:\\Users\\macie\\Source\\Repos\\Tic-Tac-Toe-with-minimax\\assets\\ArialMdm.ttf");
}

Board::~Board()
{
	for (int i = 0; i < size_; i++)
	{
		delete [] board_[i];
		delete [] winningCells_[i];
	}

	delete [] board_;
	delete [] winningCells_;
}

// Draw the grid and current board state
void Board::draw(sf::RenderWindow& window)
{
	sf::Text figure;

	// Draw grid
	for (int i = 1; i < size_; i++)
	{
		sf::RectangleShape line(sf::Vector2f(size_ * cellSize_, 5));
		line.setPosition(0, i * cellSize_);
		window.draw(line);

		line.setSize(sf::Vector2f(5, size_ * cellSize_));
		line.setPosition(i * cellSize_, 0);
		window.draw(line);
	}

	// Draw figures according to current game state
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			figure.setFont(font);
			figure.setCharacterSize(cellSize_);

			// Change color of figures if any of the players won
			if (winningCells_[i][j])
			{
				figure.setFillColor(sf::Color::Green);
			}
			else
			{
				figure.setFillColor(sf::Color::White);
			}

			if (board_[i][j] == 'x')
			{
				figure.setString("X");
			}
			else if (board_[i][j] == 'o')
			{
				figure.setString("O");
			}
			else
			{
				continue;
			}

			// Ensure that figures are placed in the square's centre
			float x = j * cellSize_ + ((cellSize_ - figure.getLocalBounds().width) / 2) - figure.getLocalBounds().left;
			float y = i * cellSize_ + ((cellSize_ - figure.getLocalBounds().height) / 2) - figure.getLocalBounds().top;

			figure.setPosition(x, y);

			window.draw(figure);
		}
	}
}

// Place specified figure on a chosen square
bool Board::makeMove(int row, int col, char player)
{
	// Do not place the figure if the square is occupied
	if (board_[row][col] != '0')
	{
		return false;
	}

	board_[row][col] = player;
	return true;
}

// Check for win, draw or unfinished
char Board::checkGameState()
{
	int xScoreCounter = 0;
	int oScoreCounter = 0;
	resetWinningCells();

	// Check rows
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_ - 1; j++)
		{
			if (board_[i][j] != '0' && board_[i][j + 1] == board_[i][j])
			{
				if (board_[i][j] == 'x')
					xScoreCounter++;

				else if (board_[i][j] == 'o')
					oScoreCounter++;

				winningCells_[i][j] = true;
				winningCells_[i][j + 1] = true;
			}
			else
			{
				xScoreCounter = 0;
				oScoreCounter = 0;
				resetWinningCells();
			}

			if (xScoreCounter == numberToWin_ - 1)
			{
				return 'x';
			}

			else if (oScoreCounter == numberToWin_ - 1)
			{
				return 'o';
			}
		}

		xScoreCounter = 0;
		oScoreCounter = 0;
		resetWinningCells();
	}

	// Check columns
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_ - 1; j++)
		{
			if (board_[j][i] != '0' && board_[j + 1][i] == board_[j][i])
			{
				if (board_[j][i] == 'x')
					xScoreCounter++;

				else if (board_[j][i] == 'o')
					oScoreCounter++;

				winningCells_[j][i] = true;
				winningCells_[j + 1][i] = true;
			}
			else
			{
				xScoreCounter = 0;
				oScoreCounter = 0;
				resetWinningCells();
			}

			if (xScoreCounter == numberToWin_ - 1)
			{
				return 'x';
			}

			else if (oScoreCounter == numberToWin_ - 1)
			{
				return 'o';
			}
		}

		xScoreCounter = 0;
		oScoreCounter = 0;
		resetWinningCells();
	}

	// Check diagonals
	// First from top left to bottom right
	// start from the second diagnoal beacues we don't need to check single corners
	for (int line = 2; line <= (size_ * 2) - 2; line++)
	{
		int startColumn = std::max(0, int(line - size_));
		int elementsInLine = minOfThree(line, (size_ - startColumn), size_);

		for (int j = 0; j < elementsInLine - 1; j++)
		{
			if (board_[std::min(int(size_), line) - j - 1][startColumn + j] != '0' && 
				board_[std::min(int(size_), line) - j - 1][startColumn + j] == board_[std::min(int(size_), line) - j - 2][startColumn + j + 1])
			{
				if (board_[std::min(int(size_), line) - j - 1][startColumn + j] == 'x')
					xScoreCounter++;

				else if (board_[std::min(int(size_), line) - j - 1][startColumn + j] == 'o')
					oScoreCounter++;

				winningCells_[std::min(int(size_), line) - j - 1][startColumn + j] = true;
				winningCells_[std::min(int(size_), line) - j - 2][startColumn + j + 1] = true;
			}
			else
			{
				xScoreCounter = 0;
				oScoreCounter = 0;
				resetWinningCells();
			}

			if (xScoreCounter == numberToWin_ - 1)
			{
				return 'x';
			}

			else if (oScoreCounter == numberToWin_ - 1)
			{
				return 'o';
			}
		}

		xScoreCounter = 0;
		oScoreCounter = 0;
		resetWinningCells();
	}

	int startRow = size_ - 2;

	//Now from bottom left to top right	
	for (int line = 2; line <= (size_ * 2) - 2; line++)
	{
		int startColumn = std::max(0, int(line - size_));
		int elementsInLine = minOfThree(line, (size_ - startColumn), size_);

		for (int j = 0; j < elementsInLine - 1; j++)
		{

			if (board_[startRow + j][startColumn + j] != '0' &&
				board_[startRow + j][startColumn + j] == board_[startRow + j + 1][startColumn + j + 1])
			{
				if (board_[startRow + j][startColumn + j] == 'x')
					xScoreCounter++;

				else if (board_[startRow + j][startColumn + j] == 'o')
					oScoreCounter++;

				winningCells_[startRow + j][startColumn + j] = true;
				winningCells_[startRow + j + 1][startColumn + j + 1] = true;
			}
			else
			{
				xScoreCounter = 0;
				oScoreCounter = 0;
				resetWinningCells();
			}

			if (xScoreCounter == numberToWin_ - 1)
			{
				return 'x';
			}

			else if (oScoreCounter == numberToWin_ - 1)
			{
				return 'o';
			}
		}

		xScoreCounter = 0;
		oScoreCounter = 0;
		resetWinningCells();

		if (startRow > 0)
			startRow--;
	}

	// Check if unfinished
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			// If there are still avaliable squares, then the game can continue
			if (board_[i][j] == '0')
				return 'u';
		}
	}

	// If none of other conditions were met, then it is a draw
	return 'd';

}

void Board::resetWinningCells()
{
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			winningCells_[i][j] = false;
		}
	}
}

void Board::resetBoard()
{
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			board_[i][j] = '0';
		}
	}
}

// This method is used when we just want to check if any moves are avaliable and
// don't need to check for draw or win
bool Board::isGameFinished()
{
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			// If there are still avaliable squares, then the game can continue
			if (board_[i][j] == '0')
				return false;
		}
	}

	// If an empty cell has not been found, then the game cannot continue
	return true;
}

// Helper method for diagonal traversal
int Board::minOfThree(int a, int b, int c)
{
	return std::min(std::min(a, b), c);
}

// Helper method for calculating maximum depth
int Board::emptyCells()
{
	int counter = 0;

	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size_; j++)
		{
			if (board_[i][j] == '0')
				counter++;
		}
	}

	return counter;
}
#endif