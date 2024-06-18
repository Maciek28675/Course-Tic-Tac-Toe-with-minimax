#include <SFML/Graphics.hpp>
#include <iostream>

#include "Board.hpp"
#include "MiniMax.hpp"

int main()
{
    int boardSize = 3;
    int numberToWin = 3;
    char newGame = 'n';

    std::cout << "Enter number of rows: ";
    std::cin >> boardSize;
    std::cout << "Enter required number of characters in a row to win: ";
    std::cin >> numberToWin;

    auto window = sf::RenderWindow{ { boardSize * 200u, boardSize * 200u }, "Tic Tac Toe" };
    window.setFramerateLimit(144);

    Board board(boardSize, numberToWin);

    char currentPlayer = 'x';
    char gameState = 'u';

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Place figure in chosen square
            if (event.type == sf::Event::MouseButtonPressed && gameState == 'u')
            {
                int row = event.mouseButton.y / 200;
                int col = event.mouseButton.x / 200;

                // If move was succesful, change players and check game state
                if (board.makeMove(row, col, currentPlayer))
                {
                    gameState = board.checkGameState();

                    if (currentPlayer == 'x')
                    {
                        currentPlayer = 'o';
                    }
                    else
                    {
                        currentPlayer = 'x';
                    }
                }
            }

        }

        // Ai move
        if (currentPlayer == 'o' && gameState == 'u')
        {
            std::pair<int, int> bestMove;
            bestMove = findBestMove(board);

            if (board.makeMove(bestMove.first, bestMove.second, currentPlayer))
            {
                gameState = board.checkGameState();
                currentPlayer = 'x';
            }

        }

        window.clear(sf::Color(48, 48, 48));

        // Draw board
        board.draw(window);
        
        window.display();

        if (gameState != 'u')
        {
            std::cout << "\nDo you want to play again? [y/n]: ";
            std::cin >> newGame;

            if (newGame == 'y')
            {
                gameState = 'u';
                board.resetBoard();
                std::cout << "New game started!\n";
            }
            else
            {
                window.close();
            }
        }
    }
}
