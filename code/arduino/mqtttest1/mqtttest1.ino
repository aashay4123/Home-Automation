#include <WiFi.h>
#include <SPI.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 2 // DHT Sensor connected to digital pin 2.
#define DHTTYPE DHT11 // Type of DHT sensor.
#define LIGHTPIN A0 // Analog light sensor connected to analog pin A0.

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor.

char ssid[] = "Aashay"; //  Change this to your network SSID (name).
char pass[] = "aashay123";  // Change this your network password
char mqttUserName[] = "Aashay";  // Can be any name.
char mqttPass[] = " M093QB2NGI17CCW4";  // Change this your MQTT API Key from Account > MyProfile.
char writeAPIKey[] = "HJZJZVLDLWW9OIUQ";    // Change to your channel Write API Key.
long channelID = 798763;

static const char alphanum[] ="0123456789"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz";  // For random generation of client ID.

WiFiClient client;                         // Initialize the Wifi client library.
PubSubClient mqttClient(client);           // Initialize the PuBSubClient library.
const char* server = "mqtt.thingspeak.com"; 

unsigned long lastConnectionTime = 0; 
const unsigned long postingInterval = 20L * 1000L; // Post data every 20 seconds.

void setup() {
  
  Serial.begin(9600);
  int status = WL_IDLE_STATUS;  // Set a temporary WiFi status.

  // Attempt to connect to WiFi network
  while (status != WL_CONNECTED) 
  {
    status = WiFi.begin(ssid, pass); // Connect to WPA/WPA2 Wi-Fi network.
    delay(5000);
  }

  Serial.println("Connected to wifi");
  mqttClient.setServer(server, 1883);   // Set the MQTT broker details.
}

void loop() {

 // Reconnect if MQTT client is not connected.
  if (!mqttClient.connected()) 
  {
    reconnect();
  }

  mqttClient.loop();   // Call the loop continuously to establish connection to the server.

  // If interval time has passed since the last connection, publish data to ThingSpeak.
  if (millis() - lastConnectionTime > postingInterval) 
  {
    mqttpublish();
  }
}

void reconnect() 
{
  char clientID[9];

  // Loop until reconnected.
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Generate ClientID
    for (int i = 0; i < 8; i++) {
        clientID[i] = alphanum[random(51)];
    }
    clientID[8]='\0';

    // Connect to the MQTT broker
    if (mqttClient.connect(clientID,mqttUserName,mqttPass)) 
    {
      Serial.print("Connected with Client ID:  ");
      Serial.print(String(clientID));
      Serial.print(", Username: ");
      Serial.print(mqttUserName);
      Serial.print(" , Passwword: ");
      Serial.println(mqttPass);
    } else 
    {
      Serial.print("failed, rc=");
      // Print to know why the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void mqttpublish() {
  
  float t = dht.readTemperature(true); // Read temperature from DHT sensor.
  float h = dht.readHumidity();  // Read humidity from DHT sensor.
  int lightLevel = analogRead(LIGHTPIN); // Read from light sensor
  
  // Create data string to send to ThingSpeak
  String data = String("field1=" + String(t, DEC) + "&field2=" + String(h, DEC) + "&field3=" + String(lightLevel, DEC));
  int length = data.length();
  char msgBuffer[length];
  data.toCharArray(msgBuffer,length+1);
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  String topicString ="channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
  length=topicString.length();
  char topicBuffer[length];
  topicString.toCharArray(topicBuffer,length+1);
 
  mqttClient.publish( topicBuffer, msgBuffer );

  lastConnectionTime = millis();
}

// Use this function instead to publish to a single field directly.  Don't forget to comment out the above version.
/*
void mqttpublish() {
  
  float t = dht.readTemperature(true); // Read temperature from DHT sensor.

  String data = String(t, DEC);
  int length = data.length();
  char msgBuffer[length];
  data.toCharArray(msgBuffer,length+1);
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  String topicString ="channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
  length=topicString.length();
  char topicBuffer[length];
  topicString.toCharArray(topicBuffer,length+1);
 
  mqttClient.publish( topicBuffer, msgBuffer );

  lastConnectionTime = millis();
}
*/
