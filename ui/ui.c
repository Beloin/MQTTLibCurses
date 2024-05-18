#include "ui.h"
#include <ncurses.h>

WINDOW *mainw;
WINDOW *menuw;

int termx, termy;

int selection_pair = 1;

void initialize_colors_pair();
void print_rows(int selected, int startx, int starty);

void ui_initialize() {
  mainw = initscr();
  cbreak();
  noecho();

  start_color();
  initialize_colors_pair();

  // Could also use stdscr
  getmaxyx(mainw, termy, termx);

  wmove(mainw, (int).1 * termy, (int).1 * termx);
  attron(A_STANDOUT);
  wprintw(mainw, "Welcome to MQTT Project!");
  attroff(A_STANDOUT);
  wrefresh(mainw);
}

void initialize_colors_pair() {
  init_pair(selection_pair, COLOR_RED, COLOR_WHITE);
}

void ui_end() { endwin(); }

enum Option ui_main_menu() {
  // menuw = newwin((int).3*termy, (int).4*termx, 0, .02*termx);
  menuw = newwin(50, 100, 1, 0);
  box(menuw, 0, 0);

  wmove(menuw, 1, 1);
  print_rows(1, 1, 1);
  wrefresh(menuw);

  return CHOICE_1;
}

char *rows[] = {"Temperature", "Humidity", "Speed"};
void print_rows(int selected, int startx, int starty) {
  for (int i = 0; i < 3; i++) {
    if (i == selected) {
      attron(COLOR_PAIR(selection_pair));
    }
    wprintw(menuw, "%s", rows[i]);
    
    attroff(COLOR_PAIR(selection_pair));
    wmove(menuw, ++starty, startx);
  }
}
