#ifndef FUNCS_H
#define FUNCS_H

#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define FIELD_W 10
#define FIELD_H 20
#define UNPAUSE 0
#define PAUSE 1
#define RESET_TIME 650

#define BACK_INIT() \
  { srand(time(NULL)); }

typedef enum {
  Start,
  Spawn,
  Moving,
  Shifting,
  Attaching,
  GameOver,
  PostGame,
  ExitGame
} GameStatus_t;

typedef enum { None, GameStart, GamePause } Banner;

typedef struct {
  int **field;
  int **block;
  int **next_block;
  int score;
  int high_score;
  int level;
  int speed;
  bool pause;
  int status;
  int block_size;
  int next_block_size;
  int coord_x;
  int coord_y;
  unsigned long long start_time;
  unsigned long long time_left;
} GameInfo_t;

typedef enum {
  UserStart,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

int **create_matrix(int H, int W);
void free_matrix(int **matrix, int H);
GameInfo_t *get_current_state();
void set_current_state_null();
GameInfo_t updateCurrentState();
void userInput(UserAction_t action);
void updateUserAction();
void initialize_state();
int **generate_new_block(int *block_size);
void spawn_block();
void rotate_block();
bool can_rotate_block();
bool can_move_left();
bool can_move_right();
bool can_move_down();
void shifting_block();
void clear_full_lines();
void attach_block_to_field();
void attaching_block();
void pause_game();
unsigned long long current_time_millis();
unsigned long long process_timer();
bool is_game_over();
void game_over();
void terminate_game();
void cleanup_game();

#endif