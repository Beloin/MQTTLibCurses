#include "mysensors.h"
#include "utils/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *sensors_thread();
void *readch_thread();

STCallback temp_callback;
STCallback hum_callback;
STCallback speed_callback;

float temp_t = -1;
float hum_t = -1;
float speed_t = -1;

static float temp = -1;
static float hum = -1;
static float speed = -1;

void sensors_set_threshold(char s, float threshold) {
  switch (s) {
  case 't':
    printf("New temperature threshold %.2f°C\n", threshold);
    temp_t = threshold;
    break;
  case 'h':
    printf("New Humidity threshold %.2f%%\n", threshold);
    hum_t = threshold;
    break;
  case 's':
    printf("New speed threshold %.2fKm/h\n", threshold);
    speed_t = threshold;
    break;
  default:
    break;
  }
}

void sensors_threshold_callback(char s, STCallback callback) {
  switch (s) {
  case 't':
    temp_callback = callback;
    break;
  case 'h':
    hum_callback = callback;
    break;
  case 's':
    speed_callback = callback;
    break;
  default:
    break;
  }
}

static pthread_t sensors_t;
static pthread_t readch_t;
static int should_run = 1;
static int t_pause = 0;
static int _ms_sleep;

void sensors_start_randomizer(int seed, int ms_sleep) {
  _ms_sleep = ms_sleep;
  srand(seed);
  pthread_create(&sensors_t, NULL, sensors_thread, NULL);
  pthread_create(&readch_t, NULL, readch_thread, NULL);
}

void play() { t_pause = !t_pause; }
void *readch_thread() {
    char c;
    while (should_run) {
        c = getchar();

        if (c == 'p') {
            play();
        }
    }

    return NULL;
}

void *sensors_thread() {
  while (should_run) {
    if (t_pause) {
      continue;
    }

    float temperature = sensors_read_temperature();
    if (temperature >= temp_t) {
      temp_callback(temperature);
    }

    float humidity = sensors_read_humidity();
    if (humidity >= hum_t) {
      hum_callback(humidity);
    }

    float speed = sensors_read_speed();
    if (speed >= speed_t) {
      speed_callback(speed);
    }

    mssleep(_ms_sleep);
  }

  return NULL;
}

float sensors_read_temperature() {
  int integral_part = rand() % 100; // Limiting to 100°C
  int decimal_part = rand() % 100;

  float sensor_result = integral_part + (float)decimal_part / 100;
  return sensor_result;
}

float sensors_read_humidity() {
  int integral_part = rand() % 100; // Limiting to 100%
  int decimal_part = rand() % 100;

  float sensor_result = integral_part + (float)decimal_part / 100;
  return sensor_result;
}

float sensors_read_speed() {
  int integral_part = rand() % 201; // Limiting to 200Km/h
  int decimal_part = rand() % 100;

  float sensor_result = integral_part + (float)decimal_part / 100;
  return sensor_result;
}
