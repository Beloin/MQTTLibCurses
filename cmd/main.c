#include "mqtt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  srand(time(NULL));

  mqtt_connect();
  // if (strcmp(argv[1], "temperature"))
  int status = mqtt_send_message("sensors/temperature", "48°C");
  if (!status)
    printf("Temperature sent\n");
  mqtt_send_message("sensors/humidity", "78%");
  if (!status)
    printf("Humidity Sent\n");
  mqtt_send_message("sensors/speed", "22Km/h");
  if (!status)
    printf("Speed sent\n");

  mqtt_disconnect();

  return EXIT_SUCCESS;
}
