#include "mqtt.h"
#include <stdio.h>

#include <MQTTAsync.h>
#include <MQTTClient.h>
#include <string.h>

#include "ds/hashmap.h"

MQTTClient client;
struct hashmap *function_map;

int mapcmp(const void *a, const void *b, void *udata) {
  const struct mappedFunction *mfA = a;
  const struct mappedFunction *mfB = b;
  return strcmp(mfA->topic, mfB->topic);
}

uint64_t maphash(const void *item, uint64_t seed0, uint64_t seed1) {
  const struct mappedFunction *user = item;
  return hashmap_sip(user->topic, strlen(user->topic), seed0, seed1);
}

void print_version() { printf("No. 1.0.0\n"); }

int on_message(void *context, char *topicName, int topicLen,
               MQTTClient_message *message) {
  char *payload = message->payload;

  /* Mostra a mensagem recebida */
  printf("Mensagem recebida! \n\rTopico: %s Mensagem: %s\n", topicName,
         payload);

  const struct mappedFunction *mp =
      hashmap_get(function_map, &(struct mappedFunction){.topic = topicName});

  if (mp) {
    mp->fun(payload); // TODO: Make this works
  }

  /* Faz echo da mensagem recebida */
  // publish(client, MQTT_PUBLISH_TOPIC, payload);

  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

int mqtt_connect() {
  int status =
      MQTTClient_create(&client, "tcp://localhost:1883", "ExampleClientSub",
                        MQTTCLIENT_PERSISTENCE_NONE, NULL);

  if (status != MQTTCLIENT_SUCCESS) {
    printf("could not create client to MQTT broker Error: %d\n", status);
    return status;
  }

  status = MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);
  if (status != MQTTCLIENT_SUCCESS) {
    printf("faled to set callbacks - Error: %d\n", status);
    return status;
  }

  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  conn_opts.username = "mqtt_example";
  conn_opts.password = "2001";
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;

  status = MQTTClient_connect(client, &conn_opts);

  if (status != MQTTCLIENT_SUCCESS) {
    printf("could not connect to MQTT broker Error: %d\n", status);
    return status;
  }

  MQTTClient_subscribe(client, "example/topic", 0); // TODO: Add a way to call functions from subscribe

  function_map = hashmap_new(sizeof(struct mappedFunction), 0, 0, 0, maphash,
                             mapcmp, NULL, NULL);

  return 0;
}

int mqtt_disconnect() {
  hashmap_free(function_map);
  return 0;
}

void handle_sub(){
 // TODO: Choose a way to implement the subscribe
}

void subscribe(callback f, char *topic) {
  printf("subscribing to %s\n", topic);
  hashmap_set(function_map, &(struct mappedFunction){.topic = topic, .fun = f});
}

void unsubscribe(char *topic) {
  printf("unsubscribing from %s\n", topic);
  hashmap_delete(function_map, &(struct mappedFunction){.topic = topic});
}
