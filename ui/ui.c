#include "ui.h"
#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>

WINDOW *mainw;
WINDOW *menuw;
WINDOW *debugbw;

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

  debugbw = newwin(15, 60, .74 * termy, .75 * termx);
  box(debugbw, 0, 0);
  wrefresh(debugbw);
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

  char buf[255];
  while (1) {
    char c = wgetch(menuw);
    // sprintf(buf, "Selected char: %c", c);
    debug_box("Selected char: %c", c);
  }

  return CHOICE_1;
}

int curr_debug = 0;
void debug_box(const char *format, ...) {
  int debug_line_limit = 12;
  if (curr_debug == debug_line_limit) {
    for (int i = 1; i <= debug_line_limit; i++) {
      wmove(debugbw, 1 * i, 1);
      wclrtoeol(debugbw);
    }

    curr_debug = 0;
  }

  va_list args;
  va_start(args, format);

  wmove(debugbw, 1 * (curr_debug + 1), 1);
  vw_printw(debugbw, format, args);

  wrefresh(debugbw);
  curr_debug++;

  va_end(args);
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

  wrefresh(menuw);
}
