JC Zaragoza  
CMPM 123 - Game Programming  
January 2026


I did this on Windows 10 on Visual Studio 2019+

When starting this project, I needed to understand the existing game engine architecture. The framework provided abstractions that I needed to understand. I needed to research more into how Bits work (A visual sprite representing a game piece (inherits from Sprite)), and BitHolders (A container on the board that can hold a Bit (inherits from Sprite))

The code in TicTacToe.cpp had comments helping with the implementation, which was really helpful for understanding what needed to be done. I ended up creating 5 key functions in order to make this work how I wanted. First was the board setup, intializing 2 players, creating the board, and intializing all the varibales I needed to track. Next was getting the piece placements right. Stopping it when you won, and making sure they were placed where my grid and squares were. The win conditions were also important, so I made functions that helped detect that. The logic behind finding the win combinations was a fun logic game, that was able to tell the program when someone has one depending on specific patterns. Draw conditions, as well as logic to make sure the statistics were consistent as you played. 

Working with the existing framework taught me how to use abstractions and code given to me while implementing domain-specific logic. The modular design makes it easy to add new functions like AI or better statistics tracking for the future. It was a fun having to figure out how to make it all work with the skeleton code we were given, as the guided process helped a lot in the fluidity of my workflow. 
