#include "mqtt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void noop() {}

int main(int argc, char *argv[]) {
  int pid = getpid();
  printf("starting client (%d)\n", pid);
  int r = connect();
  if (r) {
    exit(r);
  }

  while (1) {
    noop();
  }

  return EXIT_SUCCESS;
}
