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

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
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
    // we will implement the AI in the next assignment!
}

