#include "Application.h"
#include "imgui/imgui.h"
#include "classes/TicTacToe.h"

namespace ClassGame {
        //
        // our global variables
        //
        TicTacToe *game = nullptr;
        bool gameOver = false;
        int gameWinner = -1;
        
        //  variables to keep track of wins, losses, and draws
        int player0Wins = 0;
        int player0Losses = 0;
        int player0Draws = 0;
        int player1Wins = 0;
        int player1Losses = 0;
        int player1Draws = 0;

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            game = new TicTacToe();
            game->setUpBoard();
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        // 
        //changed the UI to display player statistics and improved
        void RenderGame() 
        {
                ImGui::DockSpaceOverViewport();

                ImGui::ShowDemoWindow();

                if (!game) return;
                if (!game->getCurrentPlayer()) return;
                
                ImGui::Begin("Settings");
                ImGui::Text("Current Player Number: %d", game->getCurrentPlayer()->playerNumber());
                ImGui::Text("Current Board State: %s", game->stateString().c_str());
                
                //PLAYER 0 STATS
                ImGui::Separator();
                ImGui::Text("Player 0 (X) Stats:");
                ImGui::Text("  Wins: %d", player0Wins);
                ImGui::Text("  Losses: %d", player0Losses);
                ImGui::Text("  Draws: %d", player0Draws);
                
                ImGui::Separator();
                //PLAYER 1 STATS
                ImGui::Text("Player 1 (O) Stats:");
                ImGui::Text("  Wins: %d", player1Wins);
                ImGui::Text("  Losses: %d", player1Losses);
                ImGui::Text("  Draws: %d", player1Draws);
                
                ImGui::Separator();

                if (gameOver) {
                    ImGui::Text("Game Over!");
                    if (gameWinner == -1) {
                        // Draw
                        ImGui::Text("Result: DRAW!");
                    } else {
                        // Win case
                        ImGui::Text("Winner: Player %d", gameWinner);
                    }
                    if (ImGui::Button("Reset Game")) {
                        game->stopGame();
                        game->setUpBoard();
                        gameOver = false;
                        gameWinner = -1;
                    }
                }
                ImGui::End();

                ImGui::Begin("GameWindow");
                game->drawFrame();
                ImGui::End();
        }

        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        // added logic to track player statistics (wins/losses/draws) and update the statistics counters when a game ends.

        void EndOfTurn() 
        {
            Player *winner = game->checkForWinner();
            if (winner)
            {
                gameOver = true;
                gameWinner = winner->playerNumber();
                
                // Update stats based on which player won
                if (gameWinner == 0) {
                    player0Wins++;
                    player1Losses++;
                } else if (gameWinner == 1) {
                    player1Wins++;
                    player0Losses++;
                }
            }
            if (game->checkForDraw()) {
                gameOver = true;
                gameWinner = -1;
                
                // Both players get a draw recorded
                player0Draws++;
                player1Draws++;
            }
        }
}
