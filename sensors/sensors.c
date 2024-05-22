#include "sensors.h"

STCallback temp_callback;
STCallback hum_callback;
STCallback speed_callback;
STCallback all_callback;

float temp_t = -1;
float hum_t = -1;
float speed_t = -1;

static float temp = -1;
static float hum = -1;
static float speed = -1;

void sensors_set_threshold(char s, float threshold) {
  switch (s) {
  case 't':
    temp_t = threshold;
    break;
  case 'h':
    hum_t = threshold;
    break;
  case 's':
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
    all_callback = callback;
    break;
  }
}
