#include "mqtt.h"
#include "mysensors.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void handle_signal(int signal) { printf("Signal %d received.\n", signal); }

void on_temp_threshold(float temp) {
  printf("----------------------------------\n");
  printf("Temperature hit Threshold %.2f\n", temp);
  printf("----------------------------------\n\n");

  char buffer[10];
  sprintf(buffer, "%.2f", temp);
  mqtt_send_message("sensors/temperature", buffer);
}

void on_humidity_threshold(float humidity) {
  printf("----------------------------------\n");
  printf("Humidity hit Threshold %.2f\n", humidity);
  printf("----------------------------------\n\n");

  char buffer[10];
  sprintf(buffer, "%.2f", humidity);
  mqtt_send_message("sensors/humidity", buffer);
}

void on_speed_threshold(float speed) {
  printf("----------------------------------\n");
  printf("Speed hit Threshold %.2f\n", speed);
  printf("----------------------------------\n\n");

  char buffer[10];
  sprintf(buffer, "%.2f", speed);
  mqtt_send_message("sensors/speed", buffer);
}

int main(int argc, char *argv[]) {
  float temp_threshold, hum_threshold, speed_threshold;
  if (argc > 3) {
    temp_threshold = atof(argv[1]);
    hum_threshold = atof(argv[2]);
    speed_threshold = atof(argv[3]);
  }

  mqtt_connect("Sensors");

  sensors_threshold_callback('t', on_temp_threshold);
  sensors_threshold_callback('h', on_humidity_threshold);
  sensors_threshold_callback('s', on_speed_threshold);

  sensors_set_threshold('t', temp_threshold);
  sensors_set_threshold('h', hum_threshold);
  sensors_set_threshold('s', speed_threshold);

  // Use time(NULL) in first arg to change seed
  sensors_start_randomizer(1, 2000);

  wait_sigint(handle_signal);
  mqtt_disconnect();

  return EXIT_SUCCESS;
}
