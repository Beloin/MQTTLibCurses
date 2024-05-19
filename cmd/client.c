#include "mqtt.h"
#include "ui.h"
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void wait_sigint();

void handle_signal(int signal) { printf("Signal %d received.\n", signal); }

char *temperature_tp = "sensors/temperature";
char *humidity_tp = "sensors/humidity";
char *speed_tp = "sensors/speed";

bool is_temp_sub = false;
void on_temperature(char *message) {
  debug_box("Temperature: %s\n", message);
  ui_sensor_add(0, message);
}

bool is_hum_sub = false;
void on_humidity(char *message) {
  debug_box("Humidity: %s\n", message);
  ui_sensor_add(1, message);
}

bool is_speed_sub = false;
void on_speed(char *message) {
  debug_box("Speed: %s\n", message);
  ui_sensor_add(2, message);
}

void menuCallback(int r, MenuCommand command) {
  if (command == ENTER) {
    switch (r) {
    case 0:
      if (!is_temp_sub) {
        subscribe(on_temperature, temperature_tp);
        debug_box("Subscribed to: %s\n", temperature_tp);

        ui_sensors_initialize(0);
      } else {
        unsubscribe(temperature_tp);
        debug_box("Unsubscribed to: %s\n", temperature_tp);

        ui_sensors_remove(0);
      }

      is_temp_sub = !is_temp_sub;
      break;
    case 1:
      if (!is_hum_sub) {
        subscribe(on_humidity, humidity_tp);
        debug_box("Subscribed to: %s\n", humidity_tp);

        ui_sensors_initialize(1);
      } else {
        unsubscribe(humidity_tp);
        debug_box("Unsubscribed to: %s\n", humidity_tp);

        ui_sensors_remove(1);
      }
      is_hum_sub = !is_hum_sub;

      break;
    case 2:
      if (!is_speed_sub) {
        subscribe(on_speed, speed_tp);
        debug_box("Subscribed to: %s\n", speed_tp);

        ui_sensors_initialize(2);
      } else {
        unsubscribe(speed_tp);
        debug_box("Unsubscribed to: %s\n", speed_tp);

        ui_sensors_remove(2);
      }
      is_speed_sub = !is_speed_sub;

      break;
    }
  }
}

pthread_t uit;
void *ui_thread() {
  ui_initialize();
#ifdef NDEBUG
  ui_debug();
#endif
  ui_main_menu(menuCallback);
  return NULL;
}

int main(int argc, char *argv[]) {
#ifdef NDEBUG
  setbuf(stdout, NULL);
#endif

  int pid = getpid();
  printf("starting client (%d)\n", pid);
  int r = mqtt_connect();
  if (r) {
    exit(r);
  }

  // subscribe(on_temperature, "sensors/temperature");

  printf("waiting sigint\n");

  // Start UI Thread
  pthread_create(&uit, NULL, ui_thread, NULL);

  wait_sigint();
  ui_end();


  // Unsubscribing from all to don't need to add 3 more if's
  unsubscribe("sensors/temperature");
  unsubscribe("sensors/humidity");
  unsubscribe("sensors/speed");

  return EXIT_SUCCESS;
}

void wait_sigint() {
  sigset_t set;
  int sig = -1;
  struct sigaction sa;
  sa.sa_handler = handle_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);
  sigemptyset(&set);
  sigaddset(&set, SIGINT);

  while (sig != SIGINT) {
    sigwait(&set, &sig);

    if (sig != SIGINT)
      printf("press ^C to exit\n");
  }

  printf("\n");
}
