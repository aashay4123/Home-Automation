#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<String.h>

// Update these with values suitable for your network.

const char* ssid = "Aashay";
const char* password = "aashay123";
const char* mqtt_server = "broker.mqtt-dashboard.com";


WiFiClient espClient;
PubSubClient client(espClient);

char msg[50], combinedData[50];
char msg1[50];
long lastMsg = 0;
String value[5];
String post_wifi, post_serial, receivedData;
const char* l1pub = "home/light_1/state";
const char* l2pub = "home/light_2/state";
const char* f1pub = "home/fan_1/state";
const char* f2pub = "home/fan_2/state";

const char* disp = "home/display";
const char* l1sub = "pub/room1/light_1";
const char* l2sub = "pub/room1/light_2";
const char* f1sub = "pub/room1/fan_1";
const char* f2sub = "pub/room1/fan_2";
int relay1 = 16, relay2 = 5, relay3 = 4, relay4 = 14;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
  upload(msg);
}

void upload(char msg[50]) {
  char* token = strtok(msg, "/");
  int i = 0;
  while (token != NULL) {
    value[i] = token;
    token = strtok(NULL, "/");
    //Serial.println(value[i]);
    i++;

    if (value[1] == "Lgt_1") {
      if (value[2] == "onn") {
        post_wifi = "Light_1 ON ";
        post_wifi.toCharArray(msg1, 50);
        client.publish(l1pub, msg1);
      } else if (value[2] == "off") {
        post_wifi = "Light_1 OFF";
        post_wifi.toCharArray(msg1, 50);
        client.publish(l1pub, msg1);
      }
    } else if (value[1] == "Lgt_2") {
      if (value[2] == "onn") {
        post_wifi = "Light_2 ON";
        post_wifi.toCharArray(msg1, 50);
        client.publish(l2pub, msg1);
      } else if (value[2] == "off") {
        post_wifi = "Light_2 OFF";
        post_wifi.toCharArray(msg1, 50);
        client.publish(l2pub, msg1);
      }
    } else if (value[1] == "fan_1") {
      if (value[2] == "onn") {
        post_wifi = "Fan_1 ON";
        post_wifi.toCharArray(msg1, 50);
        client.publish(f1pub, msg1);
      } else if (value[2] == "off") {
        post_wifi = "Fan_1 OFF";
        post_wifi.toCharArray(msg1, 50);
        client.publish(f1pub, msg1);
      }
    } else if (value[1] == "fan_2") {
      if (value[2] == "onn") {
        post_wifi = "Fan_2 ON";
        post_wifi.toCharArray(msg1, 50);
        client.publish(f2pub, msg1);
      } else if (value[2] == "off") {
        post_wifi = "Fan_2 OFF";
        post_wifi.toCharArray(msg1, 50);
        client.publish(f2pub, msg1);
      }else{
        client.publish(disp,msg);        
      }
    }
  }
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
      client.subscribe(f1sub);
      client.subscribe(f2sub);
      client.subscribe(l1sub);
      client.subscribe(l2sub);
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
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
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

    long now = millis();
    if (now - lastMsg > 2000) {
      lastMsg = now;
      upload(combinedData);
    }
    receivedData = ""; // clearing buffer for new data
  }
}
