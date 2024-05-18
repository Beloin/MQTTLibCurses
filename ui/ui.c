#include "ui.h"
#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

WINDOW *mainw;
WINDOW *menuw;
WINDOW *debugbw;

int termx, termy;

int selection_pair = 1;

void initialize_colors_pair();
MenuCommand get_command();
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

bool debug_enabled = false;
void ui_debug() {
  debug_enabled = true;
  debugbw = newwin(15, 60, .74 * termy, .75 * termx);
  box(debugbw, 0, 0);
  wrefresh(debugbw);
  wrefresh(mainw);
}

void ui_end() { endwin(); }

int current_menu_op = 0;
enum Option ui_main_menu(MenuCallback callback) {
  menuw = newwin(5, 30, 1, 0);
  box(menuw, 0, 0);

  wmove(menuw, 1, 1);
  print_rows(current_menu_op, 1, 1);
  wrefresh(menuw);

  while (1) {
    MenuCommand command = NONE;
    while (command == NONE) {
      command = get_command();
    }

    switch (command) {
    case ARROW_DOWN:
      if (current_menu_op >= 2)
        current_menu_op = 0;
      else
        current_menu_op++;
      break;
    case ARROW_UP:
      if (current_menu_op == 0)
        current_menu_op = 2;
      else
        current_menu_op--;
      break;
    default:
      break;
    }

    callback(current_menu_op, command);

    wmove(menuw, 1, 1);
    print_rows(current_menu_op, 1, 1);
    wrefresh(menuw);
  }

  return CHOICE_1;
}

int curr_debug = 0;
void debug_box(const char *format, ...) {
  if (!debug_enabled)
    return;

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
    char *format = "%s";
    if (i == selected) {
      format = "* %s";
      attron(COLOR_PAIR(selection_pair));
    }

    // Clear the menu with the * included
    for (int j = 0; j < strlen(rows[i]) + 2; j++) {
      waddch(menuw, ' ');
    }

    wmove(menuw, starty, startx);
    wprintw(menuw, format, rows[i]);

    attroff(COLOR_PAIR(selection_pair));
    wmove(menuw, ++starty, startx);
  }

  wrefresh(menuw);
}

MenuCommand get_command() {
  int c = getch();
  if (c == '\033') {   // if the first value is esc
    getch();           // skip the [
    switch (getch()) { // the real value
    case 'A':
      return ARROW_UP;
      break;
    case 'B':
      return ARROW_DOWN;
      break;
    case 'C':
      return ARROW_RIGHT;
      break;
    case 'D':
      return ARROW_LEFT;
      break;
    }
  }

  if (c == '\n')
    return ENTER;

  return NONE;
}

const char *MenuCommand_to_string(MenuCommand command) {
  switch (command) {
  case ARROW_DOWN:
    return "ARROW_DOWN";
  case ARROW_UP:
    return "ARROW_UP";
  case ARROW_LEFT:
    return "ARROW_LEFT";
  case ARROW_RIGHT:
    return "ARROW_RIGHT";
  case ENTER:
    return "ENTER";
  default:
    return "NONE";
  }
}
