#include "mqtt.h"
#include "ui.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void wait_sigint();

void handle_signal(int signal) { printf("Signal %d received.\n", signal); }

void on_temperature(char *message) { printf("Result %s", message); }

pthread_t uit;
void *ui_thread() {
  ui_initialize();
  ui_main_menu();
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

  subscribe(on_temperature, "sensors/temperature");

  printf("waiting sigint\n");

  // TODO: Start on another thread to waitsig
  pthread_create(&uit, NULL, ui_thread, NULL);

  wait_sigint();
  ui_end();

  unsubscribe("sensors/temperature");

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
