#include "mqtt.h"
#include "mysensors.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void handle_signal(int signal) { printf("Signal %d received.\n", signal); }

void on_temp_threshold(float temp) {}

void on_humidity_threshold(float humidity) {}

void on_speed_threshold(float speed) {}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  mqtt_connect("Sensors");

  sensors_threshold_callback('t', on_temp_threshold);

  wait_sigint(handle_signal);

  mqtt_disconnect();

  return EXIT_SUCCESS;
}
