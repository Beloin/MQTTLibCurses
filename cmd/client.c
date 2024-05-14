#include "mqtt.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_signal(int signal) { printf("Signal %d received.\n", signal); }

void on_temperature(char *message) {}

int main(int argc, char *argv[]) {
  int pid = getpid();
  printf("starting client (%d)\n", pid);
  int r = mqtt_connect();
  if (r) {
    exit(r);
  }

  subscribe(on_temperature, "sensors/temperature");
  subscribe(on_temperature, "example/topic");

  sigset_t set;
  int sig;
  struct sigaction sa;
  sa.sa_handler = handle_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);
  sigemptyset(&set);
  sigaddset(&set, SIGINT);

  printf("waiting sigint\n");
  fflush(stdout);
  sigwait(&set, &sig);

  unsubscribe("sensors/temperature");

  return EXIT_SUCCESS;
}
