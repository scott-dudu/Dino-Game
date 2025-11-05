#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <wchar.h>

#include "sprites.h"

#define MIN_T_HEIGHT 25
#define MIN_T_WIDTH 85

//Removes implicit declaration
int mvaddwstr();

 /**
  * Allocate and fill in the board.
  */
int create_board(wchar_t* board[], int height, int width) {

    //Build the sky.
    for (int r = 0; r < height - SPRITE_HEIGHT; r++) {
        //Allocate memory for one row. Add one to account for null-terminator.
        wchar_t *sky = (wchar_t *)malloc((width + 1) * sizeof(wchar_t));

        //Initialize all characters to be empty spaces.
        for (int c = 0; c < width; c++) {
            sky[c] = ' ';
        }

        //End string
        sky[width] = '\0';

        board[r] = sky;
    }

    wchar_t *dino[8];
    get_dino(dino);

    int len = wcslen(dino[0]);

    for (int r = height - SPRITE_HEIGHT; r < height; r++) {
        board[r] = (wchar_t *)malloc((width + 1) * sizeof(wchar_t));

        for (int c = 0; c < width; c++) {
            if (c < len) {
                if (r == height - 3 && dino[r - height + SPRITE_HEIGHT][c] == ' ') {
                    board[r][c] = '-';
                } else {
                    board[r][c] = dino[r - height + SPRITE_HEIGHT][c];
                }
            } else {
                if (r == height - 3) {
                    board[r][c] = '-';
                } else {
                    board[r][c] = ' ';
                }
            }
        }

        board[r][width] = '\0';
    }

    return 0;
}

/**
 * Free the memory of the board.
 */
void free_board(wchar_t* board[], short height) {
    //Free each row of the board.
    for (int r = 0; r < height; r++) {
        free(board[r]);
    }
}

void print_board(wchar_t* board[], short height) {
    for (int r = 0; r < height; r++) {
        mvaddwstr(r, 0, board[r]);
    }
    refresh();
}

int main() {    
    // Set the locale for wide-character support
    setlocale(LC_ALL, "");

    
    //Find terminal size.
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        exit(1);
    }
    
    unsigned short t_height = ws.ws_row;
    unsigned short t_width = ws.ws_col;
    
    //Check minimum terminal size
    if (t_height < MIN_T_HEIGHT || t_width < MIN_T_WIDTH) {
        if (t_height < MIN_T_HEIGHT) {
            fprintf(stderr, "Terminal height not large enough. Enlarge terminal height.\n");
        }
        if (t_width < MIN_T_WIDTH) {
            fprintf(stderr, "Terminal width not large enough. Enlarge terminal width.\n");
        }
        endwin();
        exit(1);
    }
    
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    //Create board
    wchar_t *board[t_height];
    create_board(board, t_height, t_width);

    print_board(board, t_height);

    getch();
    
    free_board(board, t_height);
    
    endwin();
}