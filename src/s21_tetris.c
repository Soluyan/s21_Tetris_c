#include "gui/cli/gui.h"

int main() {
  initscr();
  curs_set(0);
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  initialize_state();

  GameInfo_t Game_state = updateCurrentState();
  render(Game_state);
  free_matrix(Game_state.field, FIELD_H);

  bool status_game = true;
  GameInfo_t *Game;
  while (status_game) {
    Game = get_current_state();
    switch (Game->status) {
      case Start:
        updateUserAction();
        break;
      case Spawn:
        spawn_block();
        break;
      case Moving:
        Game->time_left = process_timer();
        updateUserAction();
        break;
      case Attaching:
        attaching_block();
        break;
      case GameOver:
        game_over();
        break;
      case PostGame:
        updateUserAction();
        break;
      case ExitGame:
        status_game = false;
        break;
    }
    Game_state = updateCurrentState();
    render(Game_state);
    free_matrix(Game_state.field, FIELD_H);
  }
  cleanup_game();
  endwin();
  return 0;
}