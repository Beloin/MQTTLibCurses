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
static void add_row_limited_window(WINDOW *w, int limit, int *curr,
                                   char *format, ...);

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
int debug_line_limit = 12;
void debug_box(const char *format, ...) {
  if (!debug_enabled)
    return;
  if (curr_debug > debug_line_limit) {
    wclear(debugbw);
    box(debugbw, 0, 0);

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
    // Clear the menu with the * included
    for (int j = 0; j < strlen(rows[i]) + 2; j++) {
      waddch(menuw, ' ');
    }

    char *format = "%s";
    if (i == selected) {
      format = "* %s";
      wattron(menuw, COLOR_PAIR(selection_pair));
    }

    wmove(menuw, starty, startx);
    wprintw(menuw, format, rows[i]);

    wattroff(menuw, COLOR_PAIR(selection_pair));
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

WINDOW *sensors_01;
WINDOW *sensors_02;
WINDOW *sensors_03;
void ui_sensors_initialize(int id) {
  // TODO: Make this windows appear and disapear with given order
  switch (id) {
  case 0:
    sensors_01 = newwin(10, 30, 1, 31);
    box(sensors_01, 0, 0);
    wmove(sensors_01, 0, 1);
    wprintw(sensors_01, "Temperature");
    wrefresh(sensors_01);
    break;
  case 1:
    sensors_02 = newwin(10, 30, 11, 31);
    box(sensors_02, 0, 0);
    wmove(sensors_02, 0, 1);
    wprintw(sensors_02, "Humidity");
    wrefresh(sensors_02);
    break;
  case 2:
    sensors_03 = newwin(10, 30, 21, 31);
    box(sensors_03, 0, 0);
    wmove(sensors_03, 0, 1);
    wprintw(sensors_03, "Speed");
    wrefresh(sensors_03);
    break;
  }
}

void ui_sensors_remove(int id) {
  switch (id) {
  case 0:
    wclear(sensors_01);
    wrefresh(sensors_01);
    delwin(sensors_01);

    sensors_01 = NULL;
    break;
  case 1:
    wclear(sensors_02);
    wrefresh(sensors_02);
    delwin(sensors_02);

    sensors_02 = NULL;
    break;
  case 2:
    wclear(sensors_03);
    wrefresh(sensors_03);
    delwin(sensors_03);

    sensors_03 = NULL;
    break;
  }
}

void ui_sensor_add(int id, char *message) {
  static int row_limit = 8;
  static int current_sensor01 = 0;
  static int current_sensor02 = 0;
  static int current_sensor03 = 0;
  switch (id) {
  case 0:
    if (sensors_01) {
      add_row_limited_window(sensors_01, row_limit, &current_sensor01,
                             "Temperature Update: %sC", message);
    }

    break;
  case 1:
    if (sensors_02) {
      add_row_limited_window(sensors_02, row_limit, &current_sensor02,
                             "Humidity Update: %s%%", message);
    }

    break;
  case 2:
    if (sensors_03) {
      add_row_limited_window(sensors_03, row_limit, &current_sensor03,
                             "Speed Update: %sKm/h", message);
    }

    break;
  }
}

static void add_row_limited_window(WINDOW *w, int limit, int *curr,
                                   char *format, ...) {
  if ((*curr) >= limit) {
    wclear(w);
    box(w, 0, 0);

    (*curr) = 0;
  }

  va_list args;
  va_start(args, format);

  wmove(w, 1 * ((*curr) + 1), 1);
  vw_printw(w, format, args);

  wrefresh(w);
  *curr = *curr + 1;

  va_end(args);
}
