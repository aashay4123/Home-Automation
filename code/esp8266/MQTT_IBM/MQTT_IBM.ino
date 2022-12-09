#include <aJSON.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <WiFi.h>
#include <WifiIPStack.h>
#include <Countdown.h>
#include <MQTTClient.h>

#define USE_USCI_B1
#define MQTT_MAX_PACKET_SIZE 100
#define IBMSERVERURLLEN  64
#define IBMIOTFSERVERSUFFIX "messaging.internetofthings.ibmcloud.com"
#define DHT11_PIN 9
DHT dht(DHT11_PIN, DHT11);
WifiIPStack ipstack;
MQTT::Client<WifiIPStack, Countdown, MQTT_MAX_PACKET_SIZE> client(ipstack);

char ssid[] = "Aashay";
char password[] = "aashay123";
char organization[] = "mwxcba";
char typeId[] = "wifi_module";
char pubtopic[] = "iot-2/evt/status/fmt/json";
char subTopic[] = "iot-2/cmd/+/fmt/json";
char deviceId[] = "cc3200";
char clientId[64];
char mqttAddr[IBMSERVERURLLEN];
char authMethod[] = "use-token-auth";
char authToken[] = "aashay123";

//const int dht = 9;
const int mqttPort = 1883;
//const int currentPin = 10;
const int moisture_pin = 33;
const int tec1 = 2, tec2 = 3;
const int fan1 = 4, fan2 = 5;
const int led1 = 14, led2 = 15;
const int cled = 39, cfan = 40;
const int pump1 = 7, pump2 = 6;

int moisture , innertemp, humidity, lux;
int  changeit, changem, changemt, changeh, changei, changel;
int value;


void callback(char* topic, byte* payload, unsigned int length);
void messageArrived(MQTT::MessageData& md);
void heating();
void cooling();
void watering();
void exhaust();
void printFloat(float value, int places) ; //uncommemnt if tmp006 doesnt work
void light();

void setup() {

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
  sprintf(mqttAddr, "%s.%s", organization, IBMIOTFSERVERSUFFIX);
  //

  //  pinMode(currentPin, OUTPUT);
  pinMode(moisture_pin, INPUT);
  pinMode(tec1, OUTPUT);
  pinMode(tec2, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);

}

void heating() {

  digitalWrite(tec1 , HIGH);
  digitalWrite(tec2 , LOW);
  Serial.print("heating started");
}

void cooling() {
  digitalWrite(tec1, LOW);
  digitalWrite(tec2, HIGH);
  Serial.print("cooling started");
}
void watering() {
  digitalWrite(pump1, HIGH);
  digitalWrite(pump2, LOW);
  delay(3000);
  digitalWrite(pump1, LOW);
  Serial.print("done wateringing");
}
void light(int var) {
  digitalWrite(led1 , HIGH);
  digitalWrite(led2 , LOW);
  analogWrite(cled , var);
  delay(1000);
  Serial.print("luxon");
}

void exhaust() {
  digitalWrite(fan1, HIGH);
  digitalWrite(fan2, LOW);
  analogWrite(cfan, 100);
  delay(1000);
  Serial.print("exhaust on");
  digitalWrite(fan1, LOW);
}


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
  //innertemp = dht.readTemperature();
  // humidity = dht.readHumidity();
  innertemp = 20;
  humidity = 30;

  if (changeit != innertemp)
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
  changeit = innertemp;

  if (changeh != humidity)
  {
    char msg2[56] = "{\"d\":{\"cc3200\":\"incubator\",\"humidity\":";
    dtostrf(humidity, 1, 2, &msg2[43]);
    msg2[48] = '}';
    msg2[49] = '}';
    msg2[50] = '\0';
    Serial.print("Publishing: ");
    Serial.println(msg2);
    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.payload = msg2;
    message.payloadlen = strlen(msg2);
    rc = client.publish(pubtopic, message);
    if (rc != 0) {
      Serial.print("Message publish failed with return code : 2 ");
    }
    client.yield(1000);

  }
  changeh = humidity;

  // moisture
  //moisture = analogRead(moisture_pin);
  moisture = 385;
  moisture = moisture / 5.5;
  if (changem != moisture) {
    char msg4[56] = "{\"d\":{\"cc3200\":\"incubator\",\"soilmoisture\":";
    dtostrf(humidity, 1, 2, &msg4[50]);
    msg4[48] = '}';
    msg4[49] = '}';
    msg4[50] = '\0';
    Serial.print("Publishing: ");
    Serial.println(msg4);
    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.payload = msg4;
    message.payloadlen = strlen(msg4);
    rc = client.publish(pubtopic, message);
    if (rc != 0) {
      Serial.print("Message publish failed with return code : 4 ");
    }
    client.yield(1000);
  }
  changem = moisture;


  if (changemt != "54")
  {
    Serial.print(changemt);
    char msg3[56] = "{\"d\":{\"cc3200\":\"TILaunchPad\",\"temperature\":";
      //dtostrf(getTemp(), 1, 2, &msg3[43]);
    msg3[48] = '}';
    msg3[49] = '}';
    msg3[50] = '\0';
    Serial.print("Publishing: ");
    Serial.println(msg3);
    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.payload = msg3;
    message.payloadlen = strlen(msg3);
    rc = client.publish(pubtopic, message);
    if (rc != 0) {
      Serial.print("Message publish failed with return code : 3");
    }
    client.yield(1000);
  }
  //lux
  //lux = analogRead(cled);
  lux = 200;
  lux = (int)(lux / 4.5511);
  if (changel != lux) {
    Serial.print(lux);
    char msg5[56] = "{\"d\":{\"myName\":\"TILaunchPad\",\"light\":";
    //dtostrf(getTemp(), 1, 2, &msg5[43]);
    msg5[48] = '}';
    msg5[49] = '}';
    msg5[50] = '\0';
    Serial.print("Publishing: ");
    Serial.println(msg5);
    MQTT::Message message;
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.payload = msg5;
    message.payloadlen = strlen(msg5);
    rc = client.publish(pubtopic, message);
    if (rc != 0) {
      Serial.print("Message publish failed with return code : 5 ");
    }
    client.yield(1000);
  }

  changemt = "54";
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
    //    digitalWrite(ledPin, HIGH);
    delay(1000);
//    digitalWrite(ledPin, LOW);
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
      //      digitalWrite(ledPin, HIGH);
      delay(250);
      //  digitalWrite(ledPin, LOW);
      delay(250);
    }
  }
  if (strstr(topic, "/cmd/settemp") != NULL) {
    Serial.print("temperature set to:");
 //   aJsonObject* root = aJson.parse(payload);
//    value = root.temperature;
 //   String string = aJson.print(root);
      String string ="d";
    if (string != NULL) {
      Serial.println(string);
      Serial.println(value);
    } else {
      Serial.print("error");
    }

    if (value > innertemp) {
      heating();
    } else {
      cooling();
    }

  } else if (strstr(topic, "/cmd/setlux") != NULL) {
    Serial.print("lux set to");
   // aJsonObject* root = aJson.parse(payload);
    //value = root.lux;
   // Serial.print(value);
    // light(value);
  } else if (strstr(topic, "/cmd/setmois") != NULL) {
    Serial.print("SOIL MOISTURE set to:");
 //   aJsonObject* root = aJson.parse(payload);
   // value = root.soilmois;
  //  Serial.print(value);
    if (moisture < value) {
      watering();
    }
  } else if (strstr(topic, "/cmd/exhaust") != NULL) {
    Serial.print("fans on");
    Serial.print(payload);
    exhaust();
  } else {
    Serial.println("Command Not Supported:");
  }
  delay(10000);
}
