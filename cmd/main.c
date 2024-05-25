#include "mqtt.h"
#include "mysensors.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void handle_signal(int signal) { printf("Signal %d received.\n", signal); }

void on_temp_threshold(float temp) {
  printf("Temperature Threshold %.2f\n", temp);
}

void on_humidity_threshold(float humidity) {
  printf("humidity Threshold %.2f\n", humidity);
}

void on_speed_threshold(float speed) {
  printf("Speed Threshold %.2f\n", speed);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  mqtt_connect("Sensors");

  sensors_threshold_callback('t', on_temp_threshold);
  sensors_threshold_callback('h', on_humidity_threshold);
  sensors_threshold_callback('s', on_speed_threshold);

  sensors_set_threshold('t', 32);
  sensors_set_threshold('h', 80);
  sensors_set_threshold('s', 100);

  sensors_start_randomizer(1, 1000);

  wait_sigint(handle_signal);
  mqtt_disconnect();

  return EXIT_SUCCESS;
}
