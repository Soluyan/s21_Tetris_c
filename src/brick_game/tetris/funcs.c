#include "funcs.h"

/**
 * @brief Создает квадратную матрицу.
 * @param H Параметр высоты матрицы.
 * @param W Параметр ширины матрицы.
 * @return Возвращает пустую матрицу с размерами заданными H и W;
 */
int **create_matrix(int H, int W) {
  int **matrix = (int **)malloc(H * sizeof(int *));
  for (int i = 0; i < H; i++) {
    int *row = (int *)malloc(W * sizeof(int));
    matrix[i] = row;
    for (int j = 0; j < W; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

/**
 * @brief Очищает квадратную матрицу.
 * @param matrix Указатель по которому находится Матрица для очищания.
 * @param H Параметр высоты матрицы.
 */
void free_matrix(int **matrix, int H) {
  for (int i = 0; i < H; i++) {
    free(matrix[i]);
  }
  free(matrix);

  matrix = NULL;
}

/**
 * @brief Получение GameInfo_t игры.
 * @return Возвращает Структуру GameInfo_t текущей игры;
 */
GameInfo_t *get_current_state() {
  static GameInfo_t info_t;
  return &info_t;
}

void set_current_state_null() {
  GameInfo_t *info_t = get_current_state();
  info_t->field = NULL;
  info_t->block = NULL;
  info_t->next_block = NULL;
  info_t->score = 0;
  info_t->high_score = 0;
  info_t->level = 0;
  info_t->speed = 0;
  info_t->pause = UNPAUSE;
  info_t->status = 0;
  info_t->block_size = 0;
  info_t->next_block_size = 0;
  info_t->coord_x = 0;
  info_t->coord_y = 0;
  info_t->start_time = 0;
  info_t->time_left = 0;
}

/**
 * @brief Накладывает Поверх игрового поля текущий блок тетриса.
 * @return Возвращает для GUI Структуру GameInfo_t;
 */
GameInfo_t updateCurrentState() {
  GameInfo_t *Game = get_current_state();
  GameInfo_t Game_state = *Game;

  int **render_field = create_matrix(FIELD_H, FIELD_W);
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      render_field[i][j] = Game->field[i][j];
    }
  }

  for (int i = 0; i < Game->block_size; i++) {
    for (int j = 0; j < Game->block_size; j++) {
      if (Game->block[i][j]) {
        // возможно нужно будет добавить отлов выхода из массива
        render_field[Game->coord_x + i][Game->coord_y + j] = Game->block[i][j];
      }
    }
  }

  Game_state.field = render_field;

  Game_state.pause = Game->pause;  // пока хз

  return Game_state;
}

/**
 * @brief В зависимости от action изменяет параметры игры.
 * @param action Состояние нажатия кнопки игроком.
 */
void userInput(UserAction_t action) {
  GameInfo_t *Game = get_current_state();

  if (action == UserStart &&
      (Game->status == Start || Game->status == PostGame)) {
    if (Game->status == Start)
      Game->status = Spawn;
    else {
      initialize_state();
      Game->status = Start;
    }
  } else if (action == Pause && Game->status != Start &&
             Game->status != GameOver) {
    pause_game();
  } else if (action == Terminate && Game->status != Start) {
    if (Game->status == PostGame)
      terminate_game();
    else
      game_over();
  } else if (action == Left && Game->status == Moving &&
             Game->pause == UNPAUSE) {
    if (can_move_left()) {
      Game->coord_y -= 1;
    }
  } else if (action == Right && Game->status == Moving &&
             Game->pause == UNPAUSE) {
    if (can_move_right()) {
      Game->coord_y += 1;
    }
  } else if (action == Down && Game->status == Moving &&
             Game->pause == UNPAUSE) {
    shifting_block();
  } else if (action == Action && Game->status == Moving &&
             Game->pause == UNPAUSE) {
    if (can_rotate_block()) rotate_block();
  }
}

/**
 * @brief Устанаввливает первичные параметры при запуске новой игры, а также
 * загружает текстовый документ с рекордом в игре
 */
void initialize_state() {
  GameInfo_t *Game = get_current_state();
  Game->status = Start;

  int **field = create_matrix(FIELD_H, FIELD_W);
  int next_temp_block_size = 0;
  int **temp_next_block = generate_new_block(&next_temp_block_size);
  Game->field = field;
  Game->score = 0;
  Game->level = 0;
  Game->speed = 1;
  Game->time_left = RESET_TIME;
  Game->pause = UNPAUSE;
  Game->coord_x = -1;
  Game->coord_y = 4;
  Game->next_block = temp_next_block;
  Game->block_size = 0;
  Game->next_block_size = next_temp_block_size;

  int high_score = 0;
  FILE *file = fopen("score.txt", "r");
  if (file == NULL) {
    file = fopen("score.txt", "w");
    fprintf(file, "0");
    fclose(file);
    file = fopen("score.txt", "r");
  }
  if (fscanf(file, "%d", &high_score) != 1) {
    perror("Failed to read");
    fclose(file);
    exit(EXIT_FAILURE);
  }
  fclose(file);
  Game->high_score = high_score;  // подгрузка из txt файла

  srand(current_time_millis());
}

/**
 * @brief Генерируе случайную игровую фигуру.
 * @param block_size Параметр сохраняющий размерновой фигуры.
 * @return Сгенерированную матрицу содержащую фигуру для тетриса;
 */
int **generate_new_block(int *block_size) {
  int **block;
  int block_type = rand() % 7;
  if (block_type == 0) {
    *block_size = 4;
    block = create_matrix(*block_size, *block_size);
  } else if (block_type == 1) {
    *block_size = 2;
    block = create_matrix(*block_size, *block_size);
  } else {
    *block_size = 3;
    block = create_matrix(*block_size, *block_size);
  }

  if (block_type == 0) {
    block[1][0] = 1;
    block[1][1] = 1;
    block[1][2] = 1;  // I
    block[1][3] = 1;
  } else if (block_type == 1) {
    block[0][0] = 1;
    block[0][1] = 1;
    block[1][0] = 1;  // O
    block[1][1] = 1;
  } else if (block_type == 2) {
    block[0][0] = 1;
    block[0][1] = 1;
    block[0][2] = 1;  // T
    block[1][1] = 1;
  } else if (block_type == 3) {
    block[0][0] = 1;
    block[1][0] = 1;
    block[1][1] = 1;  // Г
    block[1][2] = 1;
  } else if (block_type == 4) {
    block[0][2] = 1;
    block[1][0] = 1;
    block[1][1] = 1;  // Г(<-)
    block[1][2] = 1;
  } else if (block_type == 5) {
    block[0][0] = 1;
    block[0][1] = 1;
    block[1][1] = 1;  // Z
    block[1][2] = 1;
  } else if (block_type == 6) {
    block[1][0] = 1;
    block[1][1] = 1;
    block[0][1] = 1;  // Z(<-)
    block[0][2] = 1;
  }
  return block;
}

/**
 * @brief Переводит параметры координат для новой фигуры в страндартные
 * значения, очищая предыдущую матрицу с фигурой заменяя ее новой созданной
 * фигурой из next_block. Переводит игру в состояние падения фигуры на игровом
 * поле
 */
void spawn_block() {
  GameInfo_t *Game = get_current_state();
  if (Game->block != NULL) {
    free_matrix(Game->block, Game->block_size);
    // Game->block = NULL;
  }
  Game->block = Game->next_block;
  Game->block_size = Game->next_block_size;
  Game->coord_x = 0;
  Game->coord_y = (FIELD_W - Game->block_size) / 2;
  Game->next_block = generate_new_block(&Game->next_block_size);
  Game->status = Moving;
}

/**
 * @brief Разворачивает фигуру на 90 градусов вправо.
 * @param block Матрица с фигурурой.
 * @param block_size Параметр размера квадратной матрицы фигуры.
 */
void rotate_block() {
  GameInfo_t *Game = get_current_state();
  int **temp_block = create_matrix(Game->block_size, Game->block_size);

  if (Game->block_size == 4) {
    if (Game->block[1][0] == 1) {
      temp_block[0][1] = 1;
      temp_block[2][1] = 1;
      temp_block[3][1] = 1;
      temp_block[1][1] = 1;
    } else {
      temp_block[1][1] = 1;
      temp_block[1][0] = 1;
      temp_block[1][2] = 1;
      temp_block[1][3] = 1;
    }
  } else if (Game->block_size == 3) {
    for (int i = 0; i < Game->block_size; i++) {
      for (int j = 0; j < Game->block_size; j++) {
        temp_block[j][Game->block_size - 1 - i] = Game->block[i][j];
      }
    }
  }
  if (Game->block_size == 2) {
    free_matrix(temp_block, Game->block_size);
  } else {
    free_matrix(Game->block, Game->block_size);
    Game->block = temp_block;
  }
}

/**
 * @brief Проверяет возможности разворота фигуры
 * @return True - разворота фигуры допустим; \
 * @return False - разворота фигуры недопустим; \
 */
bool can_rotate_block() {
  GameInfo_t *Game = get_current_state();
  int temp_block[Game->block_size][Game->block_size];
  bool can_rotate = true;

  // Rotate
  for (int i = 0; i < Game->block_size; i++) {
    for (int j = 0; j < Game->block_size; j++) {
      temp_block[j][Game->block_size - 1 - i] = Game->block[i][j];
    }
  }

  // Check rotated block
  for (int i = 0; i < Game->block_size; i++) {
    for (int j = 0; j < Game->block_size; j++) {
      int new_x = Game->coord_x - i + 1;
      int new_y = Game->coord_y + j;
      if ((temp_block[i][j] == 1) && ((new_y < 0) || (new_y >= FIELD_W))) {
        can_rotate = false;
      } else if ((new_y >= 0) && (new_y < FIELD_W) && (new_x >= 0)) {
        if (temp_block[i][j] && Game->field[new_x][new_y]) {
          can_rotate = false;
        } else if (temp_block[i][j] && Game->field[new_x + 1][new_y]) {
          can_rotate = false;
        }
      }
    }
  }

  return can_rotate;
}

/**
 * @brief Проверяет возможности сдвига фигуры влево
 * @return True - сдвиг фигуры допустим; \
 * @return False - сдвиг фигуры недопустим; \
 */
bool can_move_left() {
  GameInfo_t *Game = get_current_state();
  for (int i = 0; i < Game->block_size; i++) {
    for (int j = 0; j < Game->block_size; j++) {
      if (Game->block[i][j] &&
          (Game->coord_y + j - 1 < 0 ||
           Game->field[Game->coord_x + i][Game->coord_y + j - 1])) {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Проверяет возможности сдвига фигуры вправо
 * @return True - сдвиг фигуры допустим; \
 * @return False - сдвиг фигуры недопустим; \
 */
bool can_move_right() {
  GameInfo_t *Game = get_current_state();
  for (int i = 0; i < Game->block_size; i++) {
    for (int j = 0; j < Game->block_size; j++) {
      if (Game->block[i][j] &&
          (Game->coord_y + j + 1 >= FIELD_W ||
           Game->field[Game->coord_x + i][Game->coord_y + j + 1])) {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Проверяет возможности сдвига фигуры вниз
 * @return True - сдвиг фигуры допустим; \
 * @return False - сдвиг фигуры недопустим; \
 */
bool can_move_down() {
  GameInfo_t *Game = get_current_state();
  for (int i = 0; i < Game->block_size; i++) {
    for (int j = 0; j < Game->block_size; j++) {
      if (Game->block[i][j] &&
          (Game->coord_x + i + 1 >= FIELD_H ||
           Game->field[Game->coord_x + i + 1][Game->coord_y + j])) {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Если это возможно сдвигает фигуру вниз по координате X иначе
 * присоединят фигуру к игровому полю;
 */
void shifting_block() {
  GameInfo_t *Game = get_current_state();
  if (can_move_down()) {
    Game->coord_x += 1;
  } else {
    attaching_block();
  }
}

/**
 * @brief Очищает полностью заполненные линии в поле для игры, начисляет
 * дополнительные очки за множественное очищение линий за один раз;
 */
void clear_full_lines() {
  GameInfo_t *Game = get_current_state();
  int combo_line = 1;
  for (int i = 0; i < FIELD_H; i++) {
    int full_line = 1;
    for (int j = 0; j < FIELD_W; j++) {
      if (!Game->field[i][j]) {
        full_line = 0;
        break;
      }
    }
    if (full_line) {
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < FIELD_W; j++) {
          Game->field[k][j] = Game->field[k - 1][j];
        }
      }
      for (int j = 0; j < FIELD_W; j++) {
        Game->field[0][j] = 0;
      }
      // сделать дополнительное начисление очков за множествое очешение линий
      Game->score += 100;
      if (combo_line == 2) {
        Game->score += 100;
      } else if (combo_line == 3) {
        Game->score += 200;
      } else if (combo_line == 4) {
        Game->score += 800;
      }
      combo_line++;
    }
    if (Game->score > Game->high_score) Game->high_score = Game->score;
  }
}

/**
 * @brief Накладывет на field матрицу с фигурой block;
 */
void attach_block_to_field() {
  GameInfo_t *Game = get_current_state();
  for (int i = 0; i < Game->block_size; i++) {
    for (int j = 0; j < Game->block_size; j++) {
      if (Game->block[i][j]) {
        Game->field[Game->coord_x + i][Game->coord_y + j] = 1;
      }
    }
  }
}

/**
 * @brief Присоединяет фигуру к игровому полю, а также очищает полностью
 * заполненные линии field, если игра не окончена переводит игру в состояние
 * Spawn, иначе в GameOver;
 */
void attaching_block() {
  GameInfo_t *Game = get_current_state();
  attach_block_to_field(Game);
  clear_full_lines(Game);
  Game->level = (int)(Game->score / 600);
  if (Game->level > 10) Game->level = 10;
  Game->speed = 1 + Game->level;
  if (is_game_over()) {
    Game->status = GameOver;
  } else {
    Game->status = Spawn;
  }
}

/**
 * @brief Замораживает игру;
 */
void pause_game() {
  GameInfo_t *Game = get_current_state();
  Game->pause = !Game->pause;
}

/**
 * @brief Подсчитывает время в игре, опускает block вниз если time_left < 0
 * @return возвращает оставшееся время до падения блока вниз на игровом поле;
 */
unsigned long long process_timer() {
  GameInfo_t *Game = get_current_state();

  unsigned long long current_time = current_time_millis();

  if (Game->pause) {
    return Game->time_left;
  }

  if (Game->start_time == 0) {
    Game->start_time = current_time;
  }

  unsigned long long elapsed_time = current_time - Game->start_time;
  unsigned long long time_left =
      Game->time_left > elapsed_time ? Game->time_left - elapsed_time : 0;

  if (time_left <= 0) {
    shifting_block();
    Game->start_time = current_time;
    if (Game->speed == 1) {
      Game->time_left = RESET_TIME;
    } else {
      Game->time_left = RESET_TIME / (Game->speed / 1.5);
    }
    time_left = Game->time_left;
  }

  return time_left;
}

unsigned long long current_time_millis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (unsigned long long)(tv.tv_sec) * 1000 +
         (unsigned long long)(tv.tv_usec) / 1000;
}

/**
 * @brief Проверяет возможности окончание игры
 * @return True - Игра окончена; \
 * @return False -  Игра неокончена; \
 */
bool is_game_over() {
  GameInfo_t *Game = get_current_state();
  for (int j = 0; j < FIELD_W; j++) {
    if (Game->field[0][j] != 0) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Если игра окончена ставит игру на паузу и сохраняет score в
 * текстовый документ, также переводит игру в состояние PostGame;
 */
void game_over() {
  GameInfo_t *Game = get_current_state();
  Game->status = PostGame;
  pause_game();
  FILE *fp = fopen("score.txt", "w");
  if (fp != NULL) {
    fprintf(fp, "%d", Game->high_score);
    fclose(fp);
  }
}

/**
 * @brief Очищает всю динамически занятую память и переводит в состояние
 * выхода из игры(ExitGame);
 */
void terminate_game() {
  GameInfo_t *Game = get_current_state();
  Game->status = ExitGame;
}

void cleanup_game() {
  GameInfo_t *Game = get_current_state();
  free_matrix(Game->field, FIELD_H);
  // if (Game->block != NULL) free_matrix(Game->block, Game->block_size);
  // if (Game->next_block != NULL) free_matrix(Game->next_block,
  // Game->next_block_size);
}