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
int mvaddnwstr();

/**
  * Allocate and fill in the board.
  */
int create_board(wchar_t* board[], int height, int width) {

    //Build the sky.
    for (int r = 0; r < height; r++) {
        //Allocate memory for one row. Add one to account for null-terminator.
        board[r] = (wchar_t *)malloc((width + 1) * sizeof(wchar_t));

        //Initialize all characters to be empty spaces.
        for (int c = 0; c < width; c++) {
            if (r == height - 3) {
                board[r][c] = L'-';
            } else {
                board[r][c] = L' ';
            }
        }

        //End string
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

/**
 * Prints the board.
 */
void print_board(wchar_t* board[], short height) {
    for (int r = 0; r < height; r++) {
        mvaddwstr(r, 0, board[r]);
    }
    refresh();
}

/**
 * Prints the dino onto the board based on the height it is at.
 */
void print_dino(wchar_t* dino[], short dino_height, short term_height) {
    int len = wcslen(dino[0]);
    
    for (int r = 0; r < SPRITE_HEIGHT; r++) {
        
        for (int c = 0; c < len; c++) {
            if (dino[r][c] != L' ') {
                mvaddnwstr(term_height - SPRITE_HEIGHT - dino_height + r, c, &dino[r][c], 1);
            }
        }
    }
}

//Update board with next line.
void update_board(wchar_t* board[], wchar_t new_col[], short height) {

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
        exit(1);
    }
    
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    //Get dino
    wchar_t* dino[SPRITE_HEIGHT];
    get_dino(dino);

    //Height tracker for dino.
    short dino_height = 0;

    //Create board
    wchar_t *board[t_height];
    create_board(board, t_height, t_width);
    set_rand_gen();

    char c;
    int obst_len = -1;
    int obst_index = -1;
    wchar_t* obst[SPRITE_HEIGHT];

    // while ((c = getch()) != 'q') {
    //     //Print the board
    //     print_board(board, t_height);

    //     //Getting obstacle
    //     if (obst_index >= obst_len) {
    //         get_obstacle(obst);
    //         obst_len = wcslen(obst[0]);
    //         obst_index = 0;
    //     } else {
    //         obst_index++;
    //     }

    //     //Translate one sprite column into an array
    //     wchar_t obst_col[SPRITE_HEIGHT];
    //     for (int row = 0; row < SPRITE_HEIGHT; row++) {
    //         obst_col[row] = obst[row][obst_index];
    //     }

    //     //update_board(board, obst_col, t_height);
    // }
    
    print_board(board, t_height);
    print_dino(dino, dino_height, t_height);
    refresh();
    getch();
    free_board(board, t_height);
    
    endwin();
}