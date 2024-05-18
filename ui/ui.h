#ifndef UI_H
#define UI_H

#include <ncurses.h>
typedef struct {
  WINDOW *win;
} Screen;

enum Option{
  CHOICE_1,
  CHOOSE_2
};

void ui_initialize();
void ui_end();

enum Option ui_main_menu();

#endif // !UI_H
