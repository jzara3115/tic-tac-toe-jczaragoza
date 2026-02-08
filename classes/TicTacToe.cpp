#include "TicTacToe.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file is intentionally *full of comments* and gentle TODOs that guide you
// through wiring up a complete Tic‑Tac‑Toe implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
// -----------------------------------------------------------------------------

const int AI_PLAYER   = 1;      // index of the AI player (O)
const int HUMAN_PLAYER= 0;      // index of the human player (X)

TicTacToe::TicTacToe()
{
    _aiMoved = false;
    _aiEnabled = true;  // AI is enabled by default
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    // Set up 2 players
    setNumberOfPlayers(2);
    
    // Reset AI flag
    _aiMoved = false;
    
    // grid options
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;
    
    // Initialize each square
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            ImVec2 position((float)(x * 100 + 50), (float)(y * 100 + 50));
            _grid[y][x].initHolder(position, "square.png", x, y);
        }
    }
    
    startGame();
}

bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    if (!holder) return false;

    if (!holder->empty()) return false;
    
    // Check if game is over
    if (checkForWinner() != nullptr) return false;
    if (checkForDraw()) return false;

    //put the player's piece on the holder
    Player *currentPlayer = getCurrentPlayer();
    if (!currentPlayer) return false;
    
    int playerNum = currentPlayer->playerNumber();
    Bit *newBit = PieceForPlayer(playerNum);
    newBit->setPosition(holder->getPosition());
    holder->setBit(newBit);
    
    // Reset AI flag when human makes a move
    if (playerNum == HUMAN_PLAYER) {
        _aiMoved = false;
    }

    return true;
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    // clear out the board
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            _grid[y][x].destroyBit();
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{
    // index is 0..8, convert to x,y using:
    int y = index / 3;
    int x = index % 3;
    
    Bit *bit = _grid[y][x].bit();
    
    if (!bit) return nullptr;

    return bit->getOwner();
}

Player* TicTacToe::checkForWinner()
{
    // All winning triples
    int winningCombinations[8][3] = {
        {0, 1, 2},  
        {3, 4, 5},  
        {6, 7, 8}, 
        {0, 3, 6}, 
        {1, 4, 7}, 
        {2, 5, 8},  
        {0, 4, 8},  
        {2, 4, 6}  
    };
    
    // check each winning combination
    for (int i = 0; i < 8; i++) {
        Player *owner0 = ownerAt(winningCombinations[i][0]);
        Player *owner1 = ownerAt(winningCombinations[i][1]);
        Player *owner2 = ownerAt(winningCombinations[i][2]);
        
        // if all three positions have the same owner and it's not nullptr
        if (owner0 != nullptr && owner0 == owner1 && owner1 == owner2) {
            return owner0;
        }
    }
    
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    // Check if all squares are filled
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (_grid[y][x].empty()) {
                return false;
            }
        }
    }
    return true;
}

// state strings
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
std::string TicTacToe::stateString() const
{
    std::string result = "";
    
    // Iterate through the board left-to-right, top-to-bottom
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            Bit *bit = _grid[y][x].bit();
            if (bit == nullptr) {
                result += '0';
            } else {
                // player numbers are 0-based so add 1
                int playerNum = bit->getOwner()->playerNumber() + 1;
                result += std::to_string(playerNum);
            }
        }
    }
    
    return result;
}

void TicTacToe::setStateString(const std::string &s)
{
    int index = 0;
    
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (index >= s.length()) break;
            
            int playerNumber = s[index] - '0';
            
            if (playerNumber == 0) {
                _grid[y][x].setBit(nullptr);
            } else if (playerNumber == 1 || playerNumber == 2) {
                int playerIdx = playerNumber - 1;
                Bit *bit = PieceForPlayer(playerIdx);
                bit->setPosition(_grid[y][x].getPosition());
                _grid[y][x].setBit(bit);
            }
            
            index++;
        }
    }
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    // Check if AI is enabled
    if (!_aiEnabled) return;
    
    // AI plays as Player 1 (O)
    if (getCurrentPlayer()->playerNumber() == AI_PLAYER && !_aiMoved) {
        // Check if game over
        if (checkForWinner() || checkForDraw()) {
            return;
        }
        
        _aiMoved = true;

        makeAIMove(AI_PLAYER);
    }
}

//
// Get the current board state as an integer array
// 0 = empty, 1 = AI player, 2 = human player
//
void TicTacToe::getBoardState(int board[9])
{
    for (int i = 0; i < 9; i++) {
        int y = i / 3;
        int x = i % 3;
        Bit* bit = _grid[y][x].bit();
        if (!bit) {
            board[i] = 0;
        } else {
            board[i] = bit->getOwner()->playerNumber() + 1;
        }
    }
}


//positive if AI is winning, negative if human is winning, 0 for neutral/draw
int TicTacToe::evaluateBoard(int board[9])
{
    // Check all winning combinations
    int winningCombinations[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // columns
        {0, 4, 8}, {2, 4, 6}              // diagonals
    };
    
    for (int i = 0; i < 8; i++) {
        int a = board[winningCombinations[i][0]];
        int b = board[winningCombinations[i][1]];
        int c = board[winningCombinations[i][2]];
        
        if (a != 0 && a == b && b == c) {
            // Someone won
            return (a == 2) ? 10 : -10;
        }
    }
    
    return 0;  // No winner yet
}

// Negamax algorithm using integer board representation
int TicTacToe::negamax(int board[9], int depth, int color)
{
    // Check if game is over
    int boardScore = evaluateBoard(board);
    if (boardScore != 0) {
        return color * boardScore;
    }
    
    // Check for draw
    bool hasEmpty = false;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            hasEmpty = true;
            break;
        }
    }
    if (!hasEmpty) return 0;
    
    // Safety: max depth
    if (depth > 9) return 0;
    
    int maxScore = -1000;
    int currentPlayer = (color == 1) ? 2 : 1;  // 2 = AI, 1 = Human
    
    // Try all possible moves
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            // Make move
            board[i] = currentPlayer;
            
            // Recursive call
            int score = -negamax(board, depth + 1, -color);
            
            // Undo move
            board[i] = 0;
            
            if (score > maxScore) {
                maxScore = score;
            }
        }
    }
    
    return maxScore;
}

// Make the best move for the AI using negamax
bool TicTacToe::makeAIMove(int playerNum)
{
    int board[9];
    getBoardState(board);
    
    int bestScore = -1000;
    int bestMove = -1;
    
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            // Try this move
            board[i] = 2;  // AI is player 1, which is represented as 2 in the board array
            
            // Evaluate
            int score = -negamax(board, 1, -1);
            
            // Undo
            board[i] = 0;
            
            // Track best
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    
    // Make the best move on the actual board
    if (bestMove != -1) {
        int y = bestMove / 3;
        int x = bestMove % 3;
        
        if (_grid[y][x].empty()) {
            Bit* bit = PieceForPlayer(playerNum);
            if (bit) {
                bit->setPosition(_grid[y][x].getPosition());
                _grid[y][x].setBit(bit);
                endTurn();
                return true;
            }
        }
    }
    
    return false;
}


