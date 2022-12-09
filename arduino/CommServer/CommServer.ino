/*  Creates Hotspot 
 *  Server IP address: 192,168,1,80
 *  Sends and receives messages from client
 */
#include <SPI.h>
#include <ESP8266WiFi.h>

char AP_ssid[] = "Emtron";           
char AP_password[] = "12345678";     
WiFiServer server(80);                    

String Message = "";

IPAddress ip(192, 168, 1, 80);            // IP address of the server
IPAddress gateway(192,168,1,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_ssid, AP_password);
    WiFi.softAPConfig(ip, gateway, subnet);
    Serial.println("");
    Serial.println("AP ip");
    Serial.println(WiFi.softAPIP());
    server.begin();                         // starts the server
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop () {
  WiFiClient client = server.available();
  
  if (client) {
    if (client.connected()) {
      digitalWrite(LED_BUILTIN, LOW);  // to show the communication only (inverted logic)
      Serial.println(".");
      String request = client.readStringUntil('\r');    // receives the message from the client
      Serial.print("From client: ");  
      Serial.println(request);
      client.flush();
      client.println("No I am Up\r");  // sends the message to the server
      digitalWrite(LED_BUILTIN, HIGH);
    }
    client.stop();                // tarminates the connection with the client
  }
}
