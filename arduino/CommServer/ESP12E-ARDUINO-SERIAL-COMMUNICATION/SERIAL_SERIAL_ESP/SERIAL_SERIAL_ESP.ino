#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<String.h>

const char* ssid = "Aashay";
const char* password = "aashay123";
const char* mqtt_server = "broker.mqtt-dashboard.com";

String receivedData;
char combinedData[50];

WiFiClient espClient;
PubSubClient client(espClient);
const char* f1pub = "home/fan_1/state";
const char* f2pub = "home/fan_2/state";

const char* disp = "home/display";
const char* l1sub = "pub/room1/light_1";
char msg[50];
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  randomSeed(micros());
}


//Function to read data received on the same topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {

    msg[i] = (char)payload[i];
  }
  Serial.println(msg);
  // upload(msg);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(f1pub);
      client.subscribe(f2pub);
      client.subscribe(l1sub);
      client.subscribe(disp);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  while (Serial.available()) {
    receivedData = Serial.readString();
    receivedData.toCharArray(combinedData, 50);
    client.publish(disp, combinedData);
  }
}
