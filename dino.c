#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

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

int create_board(char **board, int height, int width) {
    
    board = (char **)malloc(height * sizeof(char *));
    if (board == NULL) return -1;

    return 0;
}

void free_board(char **board, short height) {
    for (int r = 0; r < height; r++) {
        free(board[r]);
    }

    free(board);
}

int main() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        exit(1);
    }

    unsigned short t_height = ws.ws_row;
    unsigned short t_width = ws.ws_col;

    if (t_height < MIN_T_HEIGHT) t_height = MIN_T_HEIGHT;
    if (t_width < MIN_T_WIDTH) t_width = MIN_T_WIDTH;

    char **board = (char **)malloc(10 * sizeof(char*));
    create_board(board, t_height, t_width);
    
    free_board(board, t_height);
    return 0;
}