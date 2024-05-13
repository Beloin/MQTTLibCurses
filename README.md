# Message oriented Middleware  -  Project

Implement a network of sensors which each sensor has the following characteristics:

1. Should monitor only one parameter
2. Modify the current parameter value
    - Maybe use a random generator to this.
3. Max and Min limits for parameter (Maybe use a CLI arg)
4. When reaching those limits, should send a message to broker, to the topic the sensor is related.

Should also instantiate clients that follows this rules:

1. Read topics from the sesnor from the Broker
2. Choose which topics (Again maybe from a CLI arg) - Maybe is better to create a UI
3. Show to user the values recieved from the broker

Use UI or CLI?

Maybe is better to use UI instead of Cli

# UI
CLI or Visual?

Visual: QT  
CLI: ncurses

# Make or Cmake 

In this project we will use make to facilitate the multiple builds and run

# Mosquitto

## Broker

To configure and run:

```
docker-compose up mqtt5
docker exec -it <container-id> sh
mosquitto_passwd -c /mosquitto/config/pwfile mqtt_example

# If necessary can delete
mosquitto_passwd -D /mosquitto/config/pwfile <user-name-to-delete>
exit

# Now outside of container:
sudo docker restart <container-id>
```

Current user and password in `./config/pwfile`: `mqtt_example:2001`

## Client

Client api will be created from `paho.mqtt.c`

### Necessary libs to build

```
sudo pacman -S base-devel gcc make cmake
apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
```  

### Compiling and Installing

```
git clone git@github.com:eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make && make html
sudo make install  
```

If there's any doubt, follow [this](https://embarcados.com.br/paho-mqtt-em-c-no-linux-embarcado/) tutorial 

## Webclient

> ```docker run -d --name mqttx-web -p 80:80 emqx/mqttx-web```

# Resources

1. https://github.com/eclipse/paho.mqtt.c
2. https://eclipse.github.io/paho.mqtt.c/MQTTClient/html/pubasync.html
3. https://embarcados.com.br/paho-mqtt-em-c-no-linux-embarcado/
