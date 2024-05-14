#ifndef SRC_MQTT_HEADER
#define SRC_MQTT_HEADER

typedef void(*callback)(char *message) ;
struct mappedFunction {
  char *topic;
  callback fun;
};

void print_version();

int mqtt_connect();
int mqtt_disconnect();

void subscribe(callback f, char* topic);
void unsubscribe(char* topic);

#endif // !SRC_MQTT_HEADER

