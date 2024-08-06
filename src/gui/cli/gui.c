#include "gui.h"

void render(GameInfo_t info) {
  WINDOW *game_window = newwin(FIELD_H + 2, 2 * FIELD_W + 2, 0, 0);
  box(game_window, 0, 0);
  refresh();
  wrefresh(game_window);

  // Render game field
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      if (info.field[i][j]) {
        mvwprintw(game_window, i + 1, 2 * j + 1, "[]");
      } else {
        mvwprintw(game_window, i + 1, 2 * j + 1, "  ");
      }
    }
  }
  wrefresh(game_window);

  // Create the side panel
  WINDOW *side_panel = newwin(FIELD_H + 2, 20, 0, 2 * FIELD_W + 2);
  refresh();
  box(side_panel, 0, 0);

  // Render side panel
  mvwprintw(side_panel, 1, 1, "Pause: %s", info.pause ? "Yes" : "NO");
  mvwprintw(side_panel, 2, 1, "Time left: %lld", info.time_left);
  if (info.status == Start)
    mvwprintw(side_panel, 15, 1, "Press start button");
  else if (info.status == PostGame) {
    mvwprintw(side_panel, 14, 1, "Game over! Press");
    mvwprintw(side_panel, 15, 1, "start button or");
    mvwprintw(side_panel, 16, 1, "Exit button");
  }
  mvwprintw(side_panel, 4, 1, "Score: %d", info.score);
  mvwprintw(side_panel, 5, 1, "Level: %d", info.level);
  mvwprintw(side_panel, 6, 1, "High Score: %d", info.high_score);
  mvwprintw(side_panel, 7, 1, "Next Block:");

  // Render next block
  for (int i = 0; i < info.next_block_size; i++) {
    for (int j = 0; j < info.next_block_size; j++) {
      if (info.next_block[i][j]) {
        mvwprintw(side_panel, 8 + i, 2 + 2 * j, "[]");
      }
    }
  }

  mvwprintw(side_panel, 20, 1, "Arlindad");
  wrefresh(side_panel);

  delwin(side_panel);
  delwin(game_window);
}

/**
 * @brief Отлавливает нажатие клавиатуры и в зависимости от нажатой клавиши
 * передает action в userInput.
 */
void updateUserAction() {
  GameInfo_t *Game = get_current_state();
  // проверка на действия
  if (Game->status == Moving) {
    timeout(Game->time_left);
  }
  int ch = getch();
  switch (ch) {
    case 'n':
      userInput(UserStart);
      break;
    case KEY_HOME:
      userInput(Pause);
      break;
    case KEY_END:
      userInput(Terminate);
      break;
    case KEY_LEFT:
      userInput(Left);
      break;
    case KEY_RIGHT:
      userInput(Right);
      break;
    case KEY_DOWN:
      userInput(Down);
      break;
    case KEY_UP:
      userInput(Action);
      break;
    case ERR:
      break;
    default:
      break;
  }
  // }
}