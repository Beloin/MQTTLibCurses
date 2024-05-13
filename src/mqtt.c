#include "mqtt.h"
#include <stdio.h>

#include <MQTTAsync.h>
#include <MQTTClient.h>

void print_version() { printf("No. 1.0.0\n"); }

int on_message(void *context, char *topicName, int topicLen,
               MQTTClient_message *message) {
  char *payload = message->payload;

  /* Mostra a mensagem recebida */
  printf("Mensagem recebida! \n\rTopico: %s Mensagem: %s\n", topicName,
         payload);

  /* Faz echo da mensagem recebida */
  // publish(client, MQTT_PUBLISH_TOPIC, payload);

  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

// MQTTClient client;
int connect() {
  MQTTClient client;

  int status =
      MQTTClient_create(&client, "tcp://localhost:1883", "ExampleClientSub",
                        MQTTCLIENT_PERSISTENCE_NONE, NULL);

  // if (status != MQTTCLIENT_SUCCESS) {
  //   printf("could not create client to MQTT broker Error: %d\n", status);
  //   return status;
  // }

  // status = MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);
  // if (status != MQTTCLIENT_SUCCESS) {
  //   printf("faled to set callbacks - Error: %d\n", status);
  //   return status;
  // }

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

  // MQTTClient_subscribe(client, "example/topic", 0);

  return 0;
}
