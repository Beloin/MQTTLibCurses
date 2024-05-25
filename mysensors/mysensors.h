#ifndef SENSORS_H
#define SENSORS_H

typedef void (*STCallback)(float v);

float sensors_read_temperature();

float sensors_read_humidity();

float sensors_read_speed();

/* Set sensors threshold
 *
 *  t -> Temperature
 *  h -> humidity
 *  s -> speed
 */
void sensors_set_threshold(char s, float threshold);

/* Set on threshold callback
 *
 *  t -> Temperature
 *  h -> humidity
 *  s -> speed
 */
void sensors_threshold_callback(char s, STCallback callback);

/* Start randomizer thread.
 *
 * Use the sensors_set_threshold(char, float) to set the thresold values for
 * sensors.
 */
void sensors_start_randomizer(int seed, int ms_sleep);

void play();

#endif // !SENSORS_H
