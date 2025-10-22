#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

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

int main() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        exit(1);
    }

    unsigned short len = ws.ws_col;

    
    return 0;
}