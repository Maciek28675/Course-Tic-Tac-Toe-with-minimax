#ifndef MINI_MAX_HPP
#define MINI_MAX_HPP

#include "Board.hpp";
#include <limits>

int minimax(Board& board, int depth, int maxDepth, bool isMaximizing, int alpha, int beta);
int evaluate(Board& board);

std::pair<int, int> findBestMove(Board& board);

///////////////////////////////////////////////////////////////////////

int evaluate(Board& board)
{
	char gameState = board.checkGameState();

	if (gameState == 'o')
		return 10;

	else if (gameState == 'x')
		return -10;

	else
		return 0;
}

int minimax(Board& board, int depth, int maxDepth, bool isMaximizing, int alpha, int beta)
{
	int score = evaluate(board);

	if (depth >= maxDepth)
		return score;

	// Prefer quick wins and slow loses
	if (score == 10)
		return score - depth;

	if (score == -10)
		return score + depth;

	if (board.isGameFinished())
		return 0;

	if (isMaximizing)
	{
		int best = std::numeric_limits<int>::min();

		for (int i = 0; i < board.size_; i++)
		{
			for (int j = 0; j < board.size_; j++)
			{
				if (board.makeMove(i, j, 'o'))
				{
					// Recursivly call for child
					best = std::max(best, minimax(board, depth + 1, maxDepth, !isMaximizing, alpha, beta));
					alpha = std::max(alpha, best);

					// Cancel move
					board.board_[i][j] = '0';

					// Edge pruning
					if (beta <= alpha)
						break;
				}
			}
		}

		return best;
	}

	else
	{
		int best = std::numeric_limits<int>::max();

		for (int i = 0; i < board.size_; i++)
		{
			for (int j = 0; j < board.size_; j++)
			{
				if (board.makeMove(i, j, 'x'))
				{
					// Recursivly call for child
					best = std::min(best, minimax(board, depth + 1, maxDepth, !isMaximizing, alpha, beta));
					beta = std::min(beta, best);

					// Cancel move
					board.board_[i][j] = '0';

					// Edge pruning
					if (beta <= alpha)
						break;
				}
			}
		}

		return best;
	}
}


std::pair<int, int> findBestMove(Board& board)
{
	int bestValue = std::numeric_limits<int>::min();
	int moveValue = 0;
	int maxDepth = 0;

	std::pair<int, int> move;
	move.first = -1;
	move.second = -1;

	// Calculate max depth based on number of empty squares
	// The more empty squares - the simpler are bot's moves
	if (board.size_ == 3)
		maxDepth = 9;
	else if (board.size_ > 3)
		maxDepth = 100 / board.emptyCells();

	for (int i = 0; i < board.size_; i++)
	{
		for (int j = 0; j < board.size_; j++)
		{
			if (board.makeMove(i, j, 'o'))
			{
				moveValue = minimax(board, 0, maxDepth, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

				board.board_[i][j] = '0';

				if (moveValue > bestValue)
				{
					move.first = i;
					move.second = j;

					bestValue = moveValue;
				}
			}
		}
	}

	return move;
}

#endif