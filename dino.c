#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>

#include "sprites.h"

//Minimum terminal size.
#define MIN_T_HEIGHT 25
#define MIN_T_WIDTH 85

//Arrow key inputs.
#define UP_ARROW 3
#define DOWN_ARROW 2
#define LEFT_ARROW 4
#define RIGHT_ARROW 5
#define ENTER_KEY 10

//Dino statistics.
#define JUMP_HEIGHT 10
#define MAX_AIR_TIME 35
#define MS_PER_DINO_FRAME 100

//Score statistics
#define MS_PER_SCORE 100
#define MAX_SCORE 999999
#define SCORE_HEIGHT 10
#define SCORE_WIDTH 14

//Game statistics
#define GROUND_LEVEL 3
#define MIN_OBST_DIST 30
#define MAX_OBST_DIST 65
#define MIN_HOLD_MS 140
#define MAX_HOLD_MS 500

//States of the dino in the air, jumping, falling, and ducking.
typedef enum {
    DINO_UP,
    DINO_DOWN,
    DINO_STAY,
    DINO_DUCK
} Dino_State;

//States of the game.
typedef enum {
    GAME_OVER,
    GAME_ON,
    GAME_QUIT
} Game_State;

//Removes implicit declaration
int mvaddwstr();
int mvaddnwstr();

/**
 * Fill the board with a ground and sky.
 */
void fill_board(wchar_t* board[], short height, short width) {
    for (int r = 0; r < height; r++) {
        //Initialize all characters to be empty spaces.
        for (int c = 0; c < width; c++) {
            if (r == height - 3) {
                board[r][c] = L'-';
            } else {
                board[r][c] = L' ';
            }
        }
    }
}

/**
  * Allocate and fill in the board.
  */
void create_board(wchar_t* board[], int height, int width) {

    for (int r = 0; r < height; r++) {
        //Allocate memory for one row. Add one to account for null-terminator.
        board[r] = (wchar_t *)malloc((width + 1) * sizeof(wchar_t));

        //End string
        board[r][width] = '\0';
    }

    fill_board(board, height, width);
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
 * Function to calculate the current millisecond. Used to handle input buffering.
 */
long now_ms(void){
    struct timespec ts; 
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec*1000L + ts.tv_nsec/1000000L;
}

/**
 * Check if the character c is a valid input. Return true if it is.
 */
short is_input(char c) {
    switch (c) {
        case 'w':
        case ' ':
        case UP_ARROW:
        case 's':
        case DOWN_ARROW:
            return 1; //true
        default:
            return 0; //false
    }
}

/**
 * Prints the board.
 */
void print_board(wchar_t* board[], short height) {
    for (int r = 0; r < height; r++) {
        mvaddwstr(r, 0, board[r]);
    }
}

/**
 * Prints the dino onto the board based on the height it is at.
 */
void print_dino(wchar_t* dino[], short dino_height, short term_height, wchar_t *board[], Game_State *game_state) {
    int len = wcslen(dino[0]);
    
    for (int r = 0; r < SPRITE_HEIGHT; r++) {
        
        for (int c = 0; c < len; c++) {
            int board_row = term_height - SPRITE_HEIGHT - dino_height + r;
            wchar_t board_square = board[board_row][c];

            //Do not over ground with spaces.
            if (board_square != L'-' || dino[r][c] != L' ') {

                //Avoid covering obstacle with empty space.
                if (board_square != L' ' && dino[r][c] == L' ') continue;
                mvaddnwstr(board_row, c, &dino[r][c], 1);
                
                //Collision detection.
                if (dino[r][c] != L' ') {
                    if (board_square != L' ' && board_square != L'-') {
                        *game_state = GAME_OVER;
                    }
                }
            }

        }
    }
}

/**
 * Print the score on the top right of the screen.
 */
void print_score(int score, short t_height, short t_width) {
    wchar_t score_text[SCORE_WIDTH];
    swprintf(score_text, SCORE_WIDTH, L"Score: %06d", score);
    
    attron(A_BOLD);
    mvaddwstr(t_height - SPRITE_HEIGHT - SCORE_HEIGHT, t_width - SCORE_WIDTH - 5, score_text);
    attroff(A_BOLD);
}

/**
 * Update board with next line.
 */
void update_board(wchar_t* board[], wchar_t new_col[], short height, short width) {
    for (int r = 0; r < height; r++) {

        for (int c = 0; c < width - 1; c++) {
            if (board[r][c + 1] != L' ') {
                board[r][c] = board[r][c + 1];
            } else if (board[r][c] != L' ' && board[r][c + 1] == L' ') {
                board[r][c] = L' ';
            }
        }

        board[r][width - 1] = new_col[r];
    }
}

/**
 * Helper function to determine the next stance the dino should be based on the state.
 * Creates the running animation of the dino.
 */
void update_stance(Dino_State state, Stance* stance, long* prev_switch) {
    //If the time between stances is too short, do not change stance.
    long time = now_ms();
    if (time - *prev_switch < MS_PER_DINO_FRAME) {
        return;
    } else {
        *prev_switch = time;
    }

    //Determine the stance.
    if (*stance == STAND) { //Left foot after starting position.

        *stance = STAND_LEFT;

    } else if (state == DINO_DUCK) { //Go into duck.

        if (*stance == STAND_LEFT || *stance == DUCK_LEFT) { //If previous foot was left.
            *stance = DUCK_RIGHT;
        } else if (*stance == STAND_RIGHT || *stance == DUCK_RIGHT) { //If previous foot was right.
            *stance = DUCK_LEFT;
        }

    } else { //The dino is upright.

        if (*stance == STAND_LEFT || *stance == DUCK_LEFT) { //If previous foot was left.
            *stance = STAND_RIGHT;
        } else if (*stance == STAND_RIGHT || *stance == DUCK_RIGHT) { //If previous foot was right.
            *stance = STAND_LEFT;
        }

    }
}

/**
 * Update the dino height based on the state it is in.
 */
void update_dino(wchar_t* dino[], Dino_State* state, Stance* stance, short *dino_height, short *air_time, char input, long* prev_switch) {
    //Process character input.
    if (*dino_height == 0 && (input == 'w' || input == ' ' || input == UP_ARROW)) {
        *state = DINO_UP;
    } else if (input == 's' || input == DOWN_ARROW) {
        if (*state == DINO_UP || *dino_height != 0) {
            *state = DINO_DOWN;
        } else if (*state != DINO_DUCK) {
            *state = DINO_DUCK;
        }
    } else if (*dino_height == 0) {
        *state = DINO_STAY;
    }

    update_stance(*state, stance, prev_switch);
    get_dino(dino, *stance);

    //Update height depending if still jumping, in air, or falling.
    if (*state == DINO_UP) {
        *dino_height += 1;

        if (*dino_height >= JUMP_HEIGHT) {
            *dino_height = JUMP_HEIGHT;
            *state = DINO_STAY;
        }
    } else if (*state == DINO_DOWN) {
        *dino_height -= 1;

        if (*dino_height <= 0) {
            *dino_height = 0;
            *air_time = 0;
            *state = DINO_STAY;
        }
    } else if (*state == DINO_STAY && *dino_height != 0) {
        *air_time += 1;

        if (*air_time >= MAX_AIR_TIME) {
            *air_time = 0;
            *state = DINO_DOWN;
        }
    }

}

/**
 * Update the score.
 */
void update_score(int *score, long *last_score, long time_now) {
    if (time_now - *last_score >= MS_PER_SCORE) {
        *score += 1;
        *last_score = time_now;

        if (*score > MAX_SCORE) *score = 0;
    }
}

/**
 * Extract one column at a sspecified index from a given obstacle into an array.
 */
void translate_obstacle(wchar_t* obst[], wchar_t *obst_col, short obst_index, short obst_len, short height) {
    for (int row = 0; row < height; row++) {
            //Fill sky with spaces.
            if (row < height - SPRITE_HEIGHT) {
                obst_col[row] = L' ';
            } else {
                //Check if index accesses a sprite.
                if (obst_index < obst_len) {
                    //Prints ground or obstacle.
                    wchar_t obst_ch = obst[row - (height - SPRITE_HEIGHT)][obst_index];

                    if (obst_ch == L' ' && row == height - GROUND_LEVEL) {
                        obst_col[row] = L'-';
                    } else {
                        obst_col[row] = obst_ch;
                    }
                } else {
                    if (row == height - GROUND_LEVEL) {
                        obst_col[row] = L'-';
                    } else {
                        obst_col[row] = L' ';
                    }
                }
            }
        }
}

/**
 * Play the game.
 */
void play_game(wchar_t *board[], short t_height, short t_width, Game_State* state) {
    //Get dino
    wchar_t* dino[SPRITE_HEIGHT];
    get_dino(dino, STAND);

    //Height tracker for dino.
    short dino_height = 0;

    //Dino state tracker.
    Dino_State dino_state = DINO_STAY;
    Stance dino_stance = STAND;

    //Dino air time tracker.
    short air_time = 0;

    //Input.
    char c;
    int held = 0;
    int hold_time = 0;
    long last_seen = 0;
    long time_now;

    //Obstacle statistics:
    //the length of obstacle, which column of obstacle to print, and distance until next obstacle.
    short obst_len = -1;
    short obst_index = -1;
    short obst_diff = MIN_OBST_DIST;
    wchar_t* obst[SPRITE_HEIGHT];

    //Score holder.
    int score = 0;
    long last_score_frame = now_ms();
    long time_of_stance = now_ms();


    while (*state == GAME_ON) {
        //Get char input.
        c = getch();

        //Get time for input buffer and score.
        time_now = now_ms();

        //Input buffer
        if (is_input(c)) {
            if (held != c) {
                hold_time = MAX_HOLD_MS;
            } else {
                hold_time = MIN_HOLD_MS;
            }
            held = c;
            last_seen = time_now;
        }
        if (held && (time_now - last_seen) > hold_time) {
            held = 0;
        } else if (held > 0) {
            c = held;
        }

        //Print the board.
        print_board(board, t_height);
        print_dino(dino, dino_height, t_height, board, state);
        print_score(score, t_height, t_width);
        refresh();

        //Delay board updates.
        usleep(5000);

        //Getting obstacle.
        if (obst_index >= obst_len + obst_diff) {
            get_obstacle(obst);
            obst_len = wcslen(obst[0]);
            obst_index = 0;
            obst_diff = (rand() % MAX_OBST_DIST) + MIN_OBST_DIST;
        } else {
            obst_index++;
        }

        //Translate one sprite column into an array
        wchar_t obst_col[t_height];
        translate_obstacle(obst, obst_col, obst_index, obst_len, t_height);
        
        //Update board.
        update_dino(dino, &dino_state, &dino_stance, &dino_height, &air_time, c, &time_of_stance);
        update_board(board, obst_col, t_height, t_width);

        //Update score.
        update_score(&score, &last_score_frame, time_now);
    }
}

/**
 * Display the end screen and ask the player if they want to play again.
 */
void dead_screen(wchar_t *board[], short t_height, short t_width, Game_State *state) {
    char input;

    //Choice hover print style.
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    attron(A_BOLD);

    //Print play again message.
    mvaddwstr(t_height / 2, (t_width / 2) - 5, L"PLAY AGAIN?");
    mvaddwstr((t_height / 2) + 1, (t_width / 2) - 9, L"(USE <- and -> KEYS)");

    //Choice selector (0 == yes, 1 == no)
    short cursor = 1;

    //Continue looping until a choice has been selected.
    while (*state == GAME_OVER) {
        input = getch();

        //Display choices.
        if (cursor == 0) attron(COLOR_PAIR(1));
        mvaddwstr((t_height / 2) + 3, (t_width / 2) - 6, L" YES ");
        attroff(COLOR_PAIR(1));

        if (cursor == 1) attron(COLOR_PAIR(1));
        mvaddwstr((t_height / 2) + 3, (t_width / 2) + 3, L" NO ");
        attroff(COLOR_PAIR(1));

        //Get input.
        if (input == LEFT_ARROW || input == RIGHT_ARROW) {
            cursor = (cursor + 1) % 2;
        } else if (input == ENTER_KEY) {
            if (cursor == 0) {
                *state = GAME_ON;
            } else {
                *state = GAME_QUIT;
            }
        }

        refresh();
    }

    attroff(A_BOLD);

    if (*state == GAME_ON) {
        fill_board(board, t_height, t_width);
    }
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
    start_color();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    //Initialize game state.
    Game_State game_state = GAME_ON;

    //Create board
    wchar_t *board[t_height];
    create_board(board, t_height, t_width);
    set_rand_gen();

    while (game_state != GAME_QUIT) {
        play_game(board, t_height, t_width, &game_state);
        dead_screen(board, t_height, t_width, &game_state);
    }
    
    free_board(board, t_height);
    
    endwin();
}