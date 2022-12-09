const int light1 = A5;
const int light2 = A4;
const int fan1 = A3;
const int fan2 = A2;
int i = 0;
int l1 = 2, l2 = 2, f1 = 2, f2 = 2;
const int relay1 = 9, relay2 = 10, relay3 = 11, relay4 = 12;
int f11 = 0, f20 = 0, l10 = 0, l20 = 0;
int f10 = 0, f21 = 0, l11 = 0, l21 = 0;
int fan1_cs = 2 , fan1_ss = 2, fan2_cs = 2, fan2_ss = 2;
int light1_cs = 2, light2_cs = 2 , light1_ss = 2 , light2_ss = 2;
String str, receivedData;
String value[5], post_wifi;
char msg1[20], combinedData[20];
long lastMsg = 0; // an array to store the received data
boolean newData = false;

void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  Serial.begin(115200);
}

void upload(char msg[20]) {
  char* token = strtok(msg, "/");
  int i = 0;
  while (token != NULL) {
    value[i] = token;
    token = strtok(NULL, "/");
    i++;
    if (value[0] == "home") {
      if (value[1] == "Lgt_1") {
        if (value[2] == "onn") {
          digitalWrite(relay1, HIGH);
     //     Serial.print("atm1/Lgt_1/onn/");
        } else if (value[2] == "off") {
          digitalWrite(relay1, LOW);
     //     Serial.print("atm1/Lgt_1/off/");
        }
      } else if (value[1] == "Lgt_2") {
        if (value[2] == "onn") {
          digitalWrite(relay2, HIGH);
    //      Serial.print("atm1/Lgt_2/onn/");
        } else if (value[2] == "off") {
          digitalWrite(relay2, LOW);
      //     Serial.print("atm1/Lgt_2/off/");
        }
      } else if (value[1] == "fan_1") {
        if (value[2] == "onn") {
     //      Serial.print("atm1/fan_1/onn/");
          digitalWrite(relay3, HIGH);
        } else if (value[2] == "off") {
      //     Serial.print("atm1/fan_1/off/");
          digitalWrite(relay3, LOW);
        }
      } else if (value[1] == "fan_2") {
        if (value[2] == "onn") {
          digitalWrite(relay4, HIGH);
        //  Serial.print("atm1/fan_2/onn/");
        } else if (value[2] == "off") {
          digitalWrite(relay4, LOW);
         // Serial.print("atm1/fan_2/off/");
        }
      }
    }
  }
}
void buttonState() {

  if (l11 > l10) {
    if (l1 == 1) {
      digitalWrite(relay1, HIGH);
      Serial.print("atm/Lgt_1/onn/");
      l11 = 0;
      l10 = 0;
      l1 = 2;
    }
  } else if (l11 < l10) {
    if (l1 == 2) {
      digitalWrite(relay1, LOW);
      Serial.print("atm/Lgt_1/off/");
      l11 = 0;
      l10 = 0;
      l1 = 1;
    }
  }
  if (l21 > l20) {
    if (l2 == 1) {
      digitalWrite(relay2, HIGH);
      Serial.print("atm/Lgt_2/onn/");
      l21 = 0;
      l20 = 0;
      l2 = 2;
    }
  } else if (l21 < l20) {
    if (l2 == 2) {
      digitalWrite(relay2, LOW);
      Serial.print("atm/Lgt_2/off/");
      l21 = 0;
      l20 = 0;
      l2 = 1;
    }
  }
  if (f11 > f10) {
    if (f1 == 1) {
      digitalWrite(relay3, HIGH);
      Serial.print("atm/fan_1/onn/");
      f11 = 0;
      f10 = 0;
      f1 = 2;
    }
  } else if (f11 < f10) {
    if (f1 == 2) {
      digitalWrite(relay3, LOW);
      Serial.print("atm/fan_1/off/");
      f11 = 0;
      f10 = 0;
      f1 = 1;
    }
  }
  if (f21 > f20) {
    if (f2 == 1) {
      digitalWrite(relay4, HIGH);
      Serial.print("atm/fan_2/onn/");
      f21 = 0;
      f20 = 0;
      f2 = 2;
    }
  } else if (f21 < f20) {
    if (f1 == 2) {
      digitalWrite(relay4, LOW);
      Serial.print("atm/fan_2/off/");
      f21 = 0;
      f20 = 0;
      f1 = 1;
    }
  }

}
void  buttoncheck() {
  if (light1_ss == 0) {
    l10 = l10 + 1;
  } else if (light1_ss == 1) {
    l11 = l11 + 1;
  }
  if (light2_ss == 0) {
    l20 = l20 + 1;
  } else if (light2_ss == 1) {
    l21 = l21 + 1;
  }
  if (fan1_ss == 0) {
    f10 = f10 + 1;
  } else if (fan1_ss == 1) {
    f11 = f11 + 1;
  }
  if (fan2_ss == 0) {
    f20 = f20 + 1;
  } else if (fan2_ss == 1) {
    f21 = f21 + 1;
  }
}
void loop() {
  while (i != 600) {
    light1_ss = analogRead(light1);
    light1_ss = light1_ss / 1023;
    delay(1);
    light2_ss = analogRead(light2);
    light2_ss = light2_ss / 1023;
    delay(1);
    fan1_ss = analogRead(fan1);
    fan1_ss = fan1_ss / 1023;
    delay(1);
    fan2_ss = analogRead(fan2);
    delay(1);
    fan2_ss = fan2_ss / 1023;
    buttoncheck();
    delay(1);
    i = i + 1;
  }
  buttonState();
  while (Serial.available()) {
    receivedData = Serial.readString();
    receivedData.toCharArray(combinedData, 20);
    upload(combinedData);
  }
  receivedData = "";
  i = 0;
}
