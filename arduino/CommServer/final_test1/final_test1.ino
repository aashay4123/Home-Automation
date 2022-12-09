
#include <MQTTClient.h>

#include <aJSON.h>
#include <SPI.h>
#include <WiFi.h>
#include <WifiIPStack.h>
#include <Countdown.h>
#include <ESP8266WiFi.h>

#define USE_USCI_B1
#define MQTT_MAX_PACKET_SIZE 100
#define IBMSERVERURLLEN  64
#define IBMIOTFSERVERSUFFIX "messaging.internetofthings.ibmcloud.com"
//MACAddress mac;
WifiIPStack ipstack;
MQTT::Client<WifiIPStack, Countdown, MQTT_MAX_PACKET_SIZE> client(ipstack);

const int mqttPort = 1883;
char ssid[] = "Aashay";
char password[] = "aashay123";
char organization[] = "f38wjj";
char typeId[] = "wifi_module";
char pubtopic[] = "iot-2/evt/status/fmt/json";
char subTopic[] = "iot-2/cmd/+/fmt/json";
char deviceId[] = "nodemc";
char clientId[64];
char mqttAddr[IBMSERVERURLLEN];
char authMethod[] = "use-token-auth";
char authToken[] = "aashay123";


void callback(char* topic, byte* payload, unsigned int length);
void messageArrived(MQTT::MessageData& md);

void setup() {

  uint8_t macOctets[6];
  Serial.begin(115200);
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  while (WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    delay(300);
  }
  Serial.print("\nIP Address obtained: ");
  Serial.println(WiFi.localIP());
  Serial.println(deviceId);
  sprintf(clientId, "d:%s:%s:%s", organization, typeId, deviceId);
  sprintf(mqttAddr, "%s.%s", organization, IBMIOTFSERVERSUFFIX);}

void loop() {

  int rc = -1;
  if (!client.isConnected()) {
    Serial.print("Connecting to ");
    Serial.print(mqttAddr);
    Serial.print(":");
    Serial.println(mqttPort);
    Serial.print("With client id: ");
    Serial.println(clientId);

    while (rc != 0) {
      rc = ipstack.connect(mqttAddr, mqttPort);
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = clientId;
    connectData.username.cstring = authMethod;
    connectData.password.cstring = authToken;
    connectData.keepAliveInterval = 10;

    rc = -1;
    while ((rc = client.connect(connectData)) != 0)
      ;
    Serial.println("Connected\n");

    Serial.print("Subscribing to topic: ");
    Serial.println(subTopic);
    client.unsubscribe(subTopic);
    // Try to subscribe for commands
    if ((rc = client.subscribe(subTopic, MQTT::QOS0, messageArrived)) != 0) {
      Serial.print("Subscribe failed with return code : ");
      Serial.println(rc);
    } else {
      Serial.println("Subscribe success\n");
    }
  }
}
/*
  if (true)
  {
    char msg1[56] = "{\"d\":{\"cc3200\":\"incubator\",\"temperature\":";
    dtostrf(innertemp, 1, 2, &msg1[43]);
    msg1[48] = '}';
    msg1[49] = '}';
    msg1[50] = '\0';
    Serial.print("Publishing: ");
    Serial.println(msg1);
    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.payload = msg1;
    message.payloadlen = strlen(msg1);
    rc = client.publish(pubtopic, message);
    if (rc != 0) {
      Serial.print("Message publish failed with return code : 1 ");
    }
    client.yield(1000);
  }

 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message has arrived");

  char * msg = (char *)malloc(length * sizeof(char));
  int count = 0;
  for (count = 0 ; count < length ; count++) {
    msg[count] = payload[count];
  }
  msg[count] = '\0';
  Serial.println(msg);

  if (length > 0) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
  }


  free(msg);
}

void messageArrived(MQTT::MessageData& md) {

  // Static JsonBuffer<50> jsonBuffer;
  Serial.print("Message Received\t");
  MQTT::Message &message = md.message;
  int topicLen = strlen(md.topicName.lenstring.data) + 1;
  //    char* topic = new char[topicLen];
  char * topic = (char *)malloc(topicLen * sizeof(char));
  topic = md.topicName.lenstring.data;
  topic[topicLen] = '\0';

  int payloadLen = message.payloadlen + 1;
  //   char* payload = new char[payloadLen];
  char * payload = (char*)message.payload;
  payload[payloadLen] = '\0';

  String topicStr = topic;
  String payloadStr = payload;

  //Command topic: iot-2/cmd/blink/fmt/json

  if (strstr(topic, "/cmd/blink") != NULL) {
    Serial.print("Command IS Supported : ");
    Serial.print(payload);
    Serial.println("\t.....");


    //Blink twice
    for (int i = 0 ; i < 2 ; i++ ) {
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
    }
  }
  if (strstr(topic, "/cmd/settemp") != NULL) {
    Serial.print("temperature set to:");
    aJsonObject* root = aJson.parse(payload);
    value = root.temperature;
    String string = aJson.print(root);
    if (string != NULL) {
      Serial.println(string);
      Serial.println(value);
    } else {
      Serial.print("error");
    }
    if (strstr(topic, "/cmd/exhaust") != NULL) {
    Serial.print("fans on");
    Serial.print(payload);
    exhaust();
  } else {
    Serial.println("Command Not Supported:");
  }
  delay(10000);
}*/
