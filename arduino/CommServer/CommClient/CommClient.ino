/*  Connects to the NodeMCU Hotspot  
 *  IP Address 192,168,1,80
 *  Sends and receives message from the server in every 2 seconds
  */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

char ssid[] = "Emtron";           // SSID of your hotspot WiFi
char pass[] = "12345678";            // password of your hotspot WiFi


unsigned long askTimer = 0;

IPAddress server(192,168,1,80);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop () {
  client.connect(server, 80);   // Connection to the server
  digitalWrite(LED_BUILTIN, LOW);    // to show the communication only (inverted logic)
  Serial.println(".");
  client.println("Hello server! Are you sleeping?\r");  // sends the message to the server
  String answer = client.readStringUntil('\r');   // receives the answer from the sever
  Serial.println("from server: " + answer);
  client.flush();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);                  // client will trigger the communication after two seconds
}
