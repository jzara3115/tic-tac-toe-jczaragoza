JC Zaragoza  
CMPM 123 - Game Programming  
January 2026


I did this on Windows 10 on Visual Studio 2019+

When starting this project, I needed to understand the existing game engine architecture. The framework provided abstractions that I needed to understand. I needed to research more into how Bits work (A visual sprite representing a game piece (inherits from Sprite)), and BitHolders (A container on the board that can hold a Bit (inherits from Sprite))

The code in TicTacToe.cpp had comments helping with the implementation, which was really helpful for understanding what needed to be done. I ended up creating 5 key functions in order to make this work how I wanted. First was the board setup, intializing 2 players, creating the board, and intializing all the varibales I needed to track. Next was getting the piece placements right. Stopping it when you won, and making sure they were placed where my grid and squares were. The win conditions were also important, so I made functions that helped detect that. The logic behind finding the win combinations was a fun logic game, that was able to tell the program when someone has one depending on specific patterns. Draw conditions, as well as logic to make sure the statistics were consistent as you played. 

Working with the existing framework taught me how to use abstractions and code given to me while implementing domain-specific logic. The modular design makes it easy to add new functions like AI or better statistics tracking for the future. It was a fun having to figure out how to make it all work with the skeleton code we were given, as the guided process helped a lot in the fluidity of my workflow. 

AI IMPLEMENTATION:------------------------------

For the AI implementation, I first needed to understand how turns, win conditions, and board state were already handled in the existing framework. Since the game was already made for two players, the main challenge was figuring out how to slot the AI logic into the system without breaking any of the existing rules. The basic core of the AI is based on evaluating every possible move the AI can make and simulating future outcomes to decide which move leads to the best result. I temporarily placed pieces on the board, checked the game state, then undid those moves so the real game state didnt change. 

I implemented a function that runs whenever it is the AI’s turn, which checks that the game is still active and then determines the best possible move for the AI to make. The logic behind choosing the best move came down to scoring different outcomes. Winning states are positive, losing states are negative, and draws are neutral. I also changed the scoring so that the AI prefers to win sooner rather than later, and delays losing for as long as possible. 

One of the more important parts of this process was making sure that simulating moves for the AI did not permanently alter the board. To do this, I made sure that every temporary piece placed during the AI’s search was removed afterward, keeping the actual game state clean until the AI committed to a final move. I also had to make sure that the AI properly ended its turn after making a move so that the game could continue normally for the human player.

Overall, implementing the AI helped reinforce the importance of clean state management and modular logic. Since the rest of the game was already structured around reusable components like Bits and BitHolders, I was able to plug in the AI logic without redoing most of the code. This made it much easier to test, debug, and extend later on. In the future, this structure would make it straightforward to add difficulty levels, tweak the AI behavior, or expand the game with additional features.