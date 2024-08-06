#include "tests.h"

START_TEST(init_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  ck_assert_int_eq(state->score, 0);
  game_over();
  terminate_game();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(init_2) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  ck_assert_int_eq(state->level, 0);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(init_3) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  ck_assert_int_eq(state->pause, 0);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(init_4) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  ck_assert_int_eq(state->coord_x, -1);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(init_5) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  ck_assert_int_eq(state->coord_y, 4);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(shift) {
  initialize_state();
  shifting_block();
  GameInfo_t *state = get_current_state();
  ck_assert_int_eq(state->coord_x, 0);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(spawn_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  int block_size = state->next_block_size;
  spawn_block();
  ck_assert_int_eq(state->block_size, block_size);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

// START_TEST(spawn_2) {
//   int i = 700;
//   while (i != 0) {
//     initialize_state();
//     GameInfo_t *state = get_current_state();
//     spawn_block();
//     if (state->block != NULL) free_matrix(state->block, state->block_size);
//     if (state->next_block != NULL)
//       free_matrix(state->next_block, state->next_block_size);
//     if (state->field != NULL) free_matrix(state->field, FIELD_H);
//     set_current_state_null();
//     i--;
//   }
// }
// END_TEST

START_TEST(spawn_and_rotate) {
  int i = 1000;
  while (i != 0) {
    initialize_state();
    GameInfo_t *state = get_current_state();
    spawn_block();
    state->coord_x = 5;
    rotate_block();
    rotate_block();
    if (state->block != NULL) free_matrix(state->block, state->block_size);
    if (state->next_block != NULL)
      free_matrix(state->next_block, state->next_block_size);
    if (state->field != NULL) free_matrix(state->field, FIELD_H);
    set_current_state_null();
    i--;
  }
}
END_TEST

START_TEST(right_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  int coord_y = state->coord_y;
  userInput(UserStart);
  state->coord_x += 5;
  userInput(Right);
  ck_assert_int_eq(state->coord_y, coord_y);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(left_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  int coord_y = state->coord_y;
  userInput(UserStart);
  state->coord_x += 5;
  userInput(Right);
  ck_assert_int_eq(state->coord_y, coord_y);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(input_start) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  userInput(UserStart);
  ck_assert_int_eq(state->status, Spawn);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(consume_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  int score = state->score;
  state->status = Moving;
  attaching_block();
  ck_assert_int_eq(state->score, score);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(pause_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->status = Moving;
  pause_game();
  ck_assert_int_eq(state->pause, 1);
  pause_game();
  ck_assert_int_eq(state->pause, 0);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(timer_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  process_timer();
  get_current_state();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(input_left) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->status = Moving;
  userInput(Left);
  ck_assert_int_eq(state->status, Moving);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(input_right) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->status = Moving;
  userInput(Right);
  ck_assert_int_eq(state->status, Moving);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(consume_2) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->score = 10000;
  attaching_block();
  ck_assert_int_eq(state->score, 10000);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(input_down) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->status = Moving;
  int coord_x = state->coord_x;
  userInput(Down);
  ck_assert_int_eq(coord_x + 1, state->coord_x);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(can_rotate_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->status = Moving;
  rotate_block();
  ck_assert_int_eq(state->status, Moving);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(can_rotate_3) {
  int i = 30;
  while (i != 0) {
    initialize_state();
    GameInfo_t *state = get_current_state();
    state->status = Moving;
    rotate_block();
    if (state->block != NULL) free_matrix(state->block, state->block_size);
    if (state->next_block != NULL)
      free_matrix(state->next_block, state->next_block_size);
    if (state->field != NULL) free_matrix(state->field, FIELD_H);
    set_current_state_null();
    i--;
  }
}
END_TEST

// START_TEST(rotate_1) {
//   initialize_state();
//   GameInfo_t *state = get_current_state();
//   int block_size;
//   int **new_block = generate_new_block(&block_size);
//   int can_rotate = can_rotate_block(new_block);
//   ck_assert_int_eq(can_rotate, 1);
//   get_current_state();
//   if (state->block != NULL) free_matrix(state->block, state->block_size);
//   if (state->next_block != NULL)
//     free_matrix(state->next_block, state->next_block_size);
//   if (state->field != NULL) free_matrix(state->field, FIELD_H);
//   set_current_state_null();
// }
// END_TEST

START_TEST(attach_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  attaching_block();
  ck_assert_int_eq(state->status, Spawn);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(input_action) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->status = Moving;
  userInput(Action);
  ck_assert_int_eq(state->status, Moving);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(consume_3) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  for (int j = 0; j < FIELD_W; j++) {
    state->field[FIELD_H - 1][j] = 1;
  }
  attaching_block();
  ck_assert_int_eq(state->score, 100);
  if (state->block != NULL) free_matrix(state->block, state->block_size);

  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);

  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(consume_4) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  for (int i = FIELD_H - 1; i > FIELD_H - 3; i--) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  attaching_block();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(consume_5) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  for (int i = FIELD_H - 1; i > FIELD_H - 4; i--) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  attaching_block();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(consume_6) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  for (int i = FIELD_H - 1; i > FIELD_H - 5; i--) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  attaching_block();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

// START_TEST(input_terminate) {
//   initialize_state();
//   GameInfo_t *state = get_current_state();
//   userInput(Terminate);
//   ck_assert_int_eq(state->status, Start);
//   if (state->block != NULL) free_matrix(state->block, state->block_size);
//   if (state->next_block != NULL)
//     free_matrix(state->next_block, state->next_block_size);
//   if (state->field != NULL) free_matrix(state->field, FIELD_H);
//   set_current_state_null();
//   set_current_state_null();
// }
// END_TEST

START_TEST(timer_2) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->status = Spawn;
  process_timer();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(attach_2) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  for (int i = 0; i < state->block_size; i++) {
    for (int j = 0; j < state->block_size; j++) {
      state->block[i][j] = 0;
    }
  }
  attaching_block();
  ck_assert_int_eq(state->status, Spawn);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(right_2) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  int coord_y = state->coord_y;
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  state->status = Moving;
  userInput(Right);
  ck_assert_int_eq(state->coord_y, coord_y + 1);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(left_2) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  int coord_y = state->coord_y;
  state->coord_y += 2;
  state->coord_x += 4;
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      state->field[i][j] = 1;
    }
  }
  state->status = Moving;
  userInput(Left);
  ck_assert_int_eq(state->coord_y, coord_y + 1);
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(can_move_right_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  spawn_block();
  state->coord_y = FIELD_W;
  state->coord_x += 4;
  can_move_right();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(can_move_left_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  spawn_block();
  state->coord_y = 0;
  state->coord_x += 4;
  can_move_left();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST
START_TEST(can_move_down_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  spawn_block();
  state->coord_y = 4;
  state->coord_x = FIELD_H;
  can_move_down();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

// START_TEST(can_rotate_2) {
//   initialize_state();
//   int block_size;
//   generate_new_block(&block_size);
//   GameInfo_t *state = get_current_state();
//   state->coord_x = 5;
//   bool can_rotate = can_rotate_block();
//   ck_assert_int_eq((int)can_rotate, 1);
//   if (state->block != NULL) free_matrix(state->block, state->block_size);
//   if (state->next_block != NULL)
//     free_matrix(state->next_block, state->next_block_size);
//   if (state->field != NULL) free_matrix(state->field, FIELD_H);
//   set_current_state_null();
// }
// END_TEST

START_TEST(can_rotate_5) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  spawn_block();
  state->coord_y = 0;
  state->coord_x += 4;
  can_rotate_block();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(can_rotate_6) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  spawn_block();
  state->coord_y = FIELD_W;
  state->coord_x += 4;
  can_rotate_block();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(updateCurrentState_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  GameInfo_t Game_state = updateCurrentState();
  free_matrix(Game_state.field, FIELD_H);
  game_over();
  terminate_game();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

START_TEST(cleanup_game_1) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  cleanup_game();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  set_current_state_null();
}
END_TEST

START_TEST(timer_3) {
  initialize_state();
  GameInfo_t *state = get_current_state();
  state->time_left = 0;
  process_timer();
  get_current_state();
  if (state->block != NULL) free_matrix(state->block, state->block_size);
  if (state->next_block != NULL)
    free_matrix(state->next_block, state->next_block_size);
  if (state->field != NULL) free_matrix(state->field, FIELD_H);
  set_current_state_null();
}
END_TEST

Suite *lib_suite(void) {
  Suite *s;
  s = suite_create("Check");

  TCase *tc_tetris;

  tc_tetris = tcase_create("OK");
  suite_add_tcase(s, tc_tetris);
  tcase_add_test(tc_tetris, init_1);
  tcase_add_test(tc_tetris, init_2);
  tcase_add_test(tc_tetris, init_3);
  tcase_add_test(tc_tetris, init_4);
  tcase_add_test(tc_tetris, init_5);
  tcase_add_test(tc_tetris, shift);
  tcase_add_test(tc_tetris, spawn_1);
  tcase_add_test(tc_tetris, spawn_and_rotate);
  // tcase_add_test(tc_tetris, spawn_2);
  tcase_add_test(tc_tetris, right_1);
  tcase_add_test(tc_tetris, left_1);
  tcase_add_test(tc_tetris, input_start);
  tcase_add_test(tc_tetris, consume_1);
  tcase_add_test(tc_tetris, pause_1);
  tcase_add_test(tc_tetris, timer_1);
  tcase_add_test(tc_tetris, input_left);
  tcase_add_test(tc_tetris, input_right);
  tcase_add_test(tc_tetris, consume_2);
  tcase_add_test(tc_tetris, input_down);
  tcase_add_test(tc_tetris, can_rotate_1);
  tcase_add_test(tc_tetris, can_rotate_3);
  // tcase_add_test(tc_tetris, rotate_1);

  tcase_add_test(tc_tetris, attach_1);
  tcase_add_test(tc_tetris, input_action);
  tcase_add_test(tc_tetris, consume_3);
  tcase_add_test(tc_tetris, consume_4);
  tcase_add_test(tc_tetris, consume_5);
  tcase_add_test(tc_tetris, consume_6);
  // tcase_add_test(tc_tetris, input_terminate);
  tcase_add_test(tc_tetris, timer_2);
  tcase_add_test(tc_tetris, attach_2);
  tcase_add_test(tc_tetris, right_2);
  tcase_add_test(tc_tetris, left_2);
  tcase_add_test(tc_tetris, can_move_right_1);
  tcase_add_test(tc_tetris, can_move_left_1);
  tcase_add_test(tc_tetris, can_move_down_1);

  // tcase_add_test(tc_tetris, can_rotate_2);
  // tcase_add_test(tc_tetris, can_rotate_4);
  tcase_add_test(tc_tetris, can_rotate_5);
  tcase_add_test(tc_tetris, can_rotate_6);
  tcase_add_test(tc_tetris, updateCurrentState_1);
  tcase_add_test(tc_tetris, cleanup_game_1);
  tcase_add_test(tc_tetris, timer_3);

  return s;
}

int main(void) {
  Suite *s;
  SRunner *sr;
  int nf = 0;

  s = lib_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}