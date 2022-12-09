const int light1 = A5;
const int light2 = A4;
const int fan1 = A3;
const int fan2 = A2;
const int relay1 = 9;
const int relay2 = 10;
const int relay3 = 11;
const int relay4 = 12;

int fan1_cs , fan1_ss = 0, fan2_cs = 0, fan2_ss = 0;
int light1_cs, light2_cs , light1_ss = 0 , light2_ss = 0;
String str, receivedData;
String value[5], post_wifi;
char combinedData[50];
char msg1[50];
long lastMsg = 0;

void setup() {
  pinMode(light1, INPUT);
  pinMode(light2, INPUT);
  pinMode(fan1, INPUT);
  pinMode(fan2, INPUT);

  Serial.begin(115200);
}

void upload(char msg[50]) {
  char* token = strtok(msg, "/");
  int i = 0;
  while (token != NULL) {
    value[i] = token;
    token = strtok(NULL, "/");
    i++;

    if (value[1] == "Lgt_1") {
      if (value[2] == "onn") {
        digitalWrite(relay1, HIGH);
        light1_cs = 1;
      } else if (value[2] == "off") {
        light1_cs = 0;
        digitalWrite(relay1, LOW);
      }
    } else if (value[1] == "Lgt_2") {
      if (value[2] == "onn") {
        light2_cs = 1;
        digitalWrite(relay2, HIGH);
      } else if (value[2] == "off") {
        light2_cs = 0;
        digitalWrite(relay2, LOW);
      }
    } else if (value[1] == "fan_1") {
      if (value[2] == "onn") {
        digitalWrite(relay3, HIGH);      
        fan1_cs = 1;
      } else if (value[2] == "off") {
        fan1_cs = 0;
        digitalWrite(relay3, LOW);
      }
    } else if (value[1] == "fan_2") {
      if (value[2] == "onn") {
        fan2_cs = 1;
        digitalWrite(relay4, HIGH);

      } else if (value[2] == "off") {
        fan2_cs = 0;
        digitalWrite(relay4, LOW);
      }
    }
  }
}
void check() {
  if (light1_cs == 1 || light1_ss == 1) {
    post_wifi = "home/lgt_1/onn ";
    Serial.print(post_wifi);
    digitalWrite(relay1, HIGH);
  } else if (light1_cs == 0 && light1_ss == 0) {
    post_wifi = "home/lgt_1/off ";
    Serial.print(post_wifi);
    digitalWrite(relay1, LOW);
  }
  else  if (light2_cs == 1 || light2_ss == 1) {
    post_wifi = "home/lgt_2/onn ";
    Serial.print(post_wifi);

    digitalWrite(relay2, HIGH);
  } else if (light2_cs == 0 && light2_ss == 0)  {
    post_wifi = "home/lgt_2/off ";
    Serial.print(post_wifi);
    digitalWrite(relay2, LOW);


  }
  else  if (fan1_cs == 1 || fan1_ss == 1) {
    post_wifi = "home/fan_1/onn ";
    Serial.print(post_wifi);
    digitalWrite(relay3, HIGH);
  } else  if (fan1_cs == 0 && fan1_ss == 0) {
    post_wifi = "home/fan_1/off ";
    Serial.print(post_wifi);
    digitalWrite(relay3, LOW);
  }
  else if (fan2_cs == 1 || fan2_ss == 1) {
    post_wifi = "home/fan_2/onn ";
    Serial.print(post_wifi);
    digitalWrite(relay4, HIGH);
  } else if (fan2_cs == 0 && fan2_ss == 0) {
    post_wifi = "home/fan_2/off ";
    Serial.print(post_wifi);
    digitalWrite(relay4, LOW);
  }
}
void loop() {
  int light1_ss = analogRead(light1);
  int light2_ss = analogRead(light2);
  int fan1_ss = analogRead(fan1);
  int fan2_ss = analogRead(fan2);

  while (Serial.available()) {
    receivedData = Serial.readString();
    receivedData.toCharArray(combinedData, 50);
    upload(combinedData);

  }
  receivedData = "";

}
