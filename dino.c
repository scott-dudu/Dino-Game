#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <wchar.h>

#define MIN_T_HEIGHT 25
#define MIN_T_WIDTH 85

/**
    Dino:
    printf("          ▄▄▄▄▄   \n");  
    printf("         ██▄██████\n");
    printf("         ██████▄▄▄\n");
    printf("        ▄█████    \n");
    printf("   █▄▄████████▀▀█ \n");
    printf("    ▀▀██████▀     \n");
    printf("      █▀  ▀█      \n");
    printf("      █▄   █▄     \n");

    Cactus:
    printf("         ▄▄    \n");
    printf("        ████ ▄▄\n");
    printf("     ██ ████▄█▀\n");
    printf("      ▀█████▀  \n");
    printf("        ████   \n");

    Cacti
    printf("         ▄▄               ▄▄    \n");
    printf("        ████ ▄▄       ██ ████ ▄▄\n");
    printf("     ██ ████▄█▀ ▄  ▄█▄ ▀█████▄█▀\n");
    printf("      ▀█████▀  ███ ███   ████▀  \n");
    printf("        ████   ███ ███   ████   \n");

    Bird:
    printf("         ▄█▄           \n");  
    printf("       ▄██▄█▄ ▄▄██▀▀▀  \n");
    printf("           ▀████████▀▀▀\n");
    printf("              ▀▀▀▀     \n");
 */

 /**
  * Allocate and fill in the board.
  */
int create_board(wchar_t* board[], int height, int width) {

    //Build the sky.
    for (int r = 0; r < height - 8; r++) {
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

    wchar_t *dino[8] = {L"          ▄▄▄▄▄   ",  
                        L"         ██▄██████",
                        L"         ██████▄▄▄",
                        L"        ▄█████    ",
                        L"   █▄▄████████▀▀█ ",
                        L"    ▀▀██████▀     ",
                        L"      █▀  ▀█      ",
                        L"      █▄   █▄     "};

    int len = wcslen(dino[0]);
    int size = sizeof(dino) / sizeof(wchar_t*);

    for (int r = height - 8; r < height; r++) {
        board[r] = (wchar_t *)malloc((width + 1) * sizeof(wchar_t));

        for (int c = 0; c < width; c++) {
            if (c < len) {
                if (r == height - 3 && dino[r - height + 8][c] == ' ') {
                    board[r][c] = L'-';
                } else {
                    board[r][c] = dino[r - height + 8][c];
                }
            } else {
                if (r == height - 3) {
                    board[r][c] = L'-';
                } else {
                    board[r][c] = L' ';
                }
            }
        }
    }

    for(int i = 0; i < height; i++) {
        printf("%ls", board[i]);
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

int main() {    
    // Set the locale for wide-character support
    setlocale(LC_ALL, "");

    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        exit(1);
    }

    unsigned short t_height = ws.ws_row;
    unsigned short t_width = ws.ws_col;

    if (t_height < MIN_T_HEIGHT) t_height = MIN_T_HEIGHT;
    if (t_width < MIN_T_WIDTH) t_width = MIN_T_WIDTH;

    wchar_t *board[t_height];
    create_board(board, t_height, t_width);
    
    free_board(board, t_height);
    return 0;
}