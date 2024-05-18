#ifndef UI_H
#define UI_H

#include <ncurses.h>
typedef struct {
  WINDOW *win;
} Screen;

enum Option { CHOICE_1, CHOOSE_2 };
typedef enum {
  ARROW_DOWN,
  ARROW_UP,
  ARROW_LEFT,
  ARROW_RIGHT,
  ENTER,
  NONE
} MenuCommand;

typedef void (*MenuCallback)(int row, MenuCommand command);

const char *MenuCommand_to_string(MenuCommand command);

void ui_initialize();
void ui_end();

enum Option ui_main_menu(MenuCallback callback);

// TODO: Create an UI for status update

void ui_debug();
void debug_box(const char *format, ...);

#endif // !UI_H
