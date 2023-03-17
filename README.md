# snake
snake game

This implementation uses the ncurses library to draw the game grid, 
handle keyboard input, and display text on the screen. 

The game loop checks for keyboard input, moves the snake, checks for collisions
with the edges of the screen or itself, and updates the score and food placement
as necessary.

To compile and run the game, you'll need to install ncurses 
and run 

make

or

gcc -o snake snake.c -lncurses 

Then to run:

./snake
