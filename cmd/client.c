#include "mqtt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pid = getpid();
  printf("starting client (%d)\n", pid);
  int r = mqtt_connect();
  if (r) {
    printf("Error %d", r);
    exit(r);
  }
  printf("connected with status %d\n", r);

  while (1) {
  }

  return EXIT_SUCCESS;
}
