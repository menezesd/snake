#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define ROWS 20
#define COLS 40

#define FOOD '$'
#define SNAKE_HEAD 'O'
#define SNAKE_BODY 'o'

#define INIT_SNAKE_LEN 3

struct snake_segment {
    int row;
    int col;
};

enum direction {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
};

void initialize_ncurses()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

void draw_grid()
{
    for (int i = 0; i <= ROWS; i++) {
	for (int j = 0; j <= COLS; j++) {
	    if (i == 0 || i == ROWS || j == 0 || j == COLS) {
		mvaddch(i, j, '#');
	    }
	}
    }
}

void draw_snake(struct snake_segment *snake, int len)
{
    mvaddch(snake[0].row, snake[0].col, SNAKE_HEAD);
    for (int i = 1; i < len; i++) {
	mvaddch(snake[i].row, snake[i].col, SNAKE_BODY);
    }
}

void clear_snake(struct snake_segment *snake, int len)
{
    for (int i = 0; i < len; i++) {
	mvaddch(snake[i].row, snake[i].col, ' ');
    }
}

void move_snake(struct snake_segment *snake, int len, enum direction dir)
{
    int dr = 0, dc = 0;
    switch (dir) {
    case DIR_LEFT:
	dc = -1;
	break;
    case DIR_RIGHT:
	dc = 1;
	break;
    case DIR_UP:
	dr = -1;
	break;
    case DIR_DOWN:
	dr = 1;
	break;
    }
    int new_row = snake[0].row + dr;
    int new_col = snake[0].col + dc;
    for (int i = len - 1; i > 0; i--) {
	snake[i].row = snake[i - 1].row;
	snake[i].col = snake[i - 1].col;
    }
    snake[0].row = new_row;
    snake[0].col = new_col;
}

int check_collision(struct snake_segment *snake, int len)
{
    int head_row = snake[0].row;
    int head_col = snake[0].col;
    if (head_row == 0 || head_row == ROWS || head_col == 0
	|| head_col == COLS) {
	return 1;
    }
    for (int i = 1; i < len; i++) {
	if (head_row == snake[i].row && head_col == snake[i].col) {
	    return 1;
	}
    }
    return 0;
}

void place_food(int *row, int *col, struct snake_segment *snake, int len)
{
    while (1) {
	*row = rand() % (ROWS - 1) + 1;
	*col = rand() % (COLS - 1) + 1;
	int collides = 0;
	for (int i = 0; i < len; i++) {
	    if (*row == snake[i].row && *col == snake[i].col) {
		collides = 1;
		break;
	    }
	}
	if (!collides) {
	    break;
	}
    }
    mvaddch(*row, *col, FOOD);
}

int main()
{
    srand(time(NULL));

    initialize_ncurses();

    draw_grid();

    struct snake_segment snake[ROWS * COLS];
    int snake_len = INIT_SNAKE_LEN;
    for (int i = 0; i < snake_len; i++) {
	snake[i].row = ROWS / 2;
	snake[i].col = COLS / 2 - i;
    }
    draw_snake(snake, snake_len);

    int food_row, food_col;
    place_food(&food_row, &food_col, snake, snake_len);

    enum direction dir = DIR_RIGHT;

    int score = 0;
    while (1) {
	int ch = getch();
	switch (ch) {
	case KEY_LEFT:
	    if (dir != DIR_RIGHT) {
		dir = DIR_LEFT;
	    }
	    break;
	case KEY_RIGHT:
	    if (dir != DIR_LEFT) {
		dir = DIR_RIGHT;
	    }
	    break;
	case KEY_UP:
	    if (dir != DIR_DOWN) {
		dir = DIR_UP;
	    }
	    break;
	case KEY_DOWN:
	    if (dir != DIR_UP) {
		dir = DIR_DOWN;
	    }
	    break;
	}

	clear_snake(snake, snake_len);
	move_snake(snake, snake_len, dir);
	if (check_collision(snake, snake_len)) {
	    break;
	}
	if (snake[0].row == food_row && snake[0].col == food_col) {
	    snake_len++;
	    score++;
	    place_food(&food_row, &food_col, snake, snake_len);
	}
	draw_snake(snake, snake_len);

	mvprintw(ROWS + 1, 0, "Score: %d", score);

	refresh();
	usleep(200000);
    }

    clear();
    mvprintw(ROWS / 2, COLS / 2 - 4, "Game Over");
    mvprintw(ROWS / 2 + 1, COLS / 2 - 8, "Final Score: %d", score);
    refresh();
    getch();

    endwin();
    return 0;
}
