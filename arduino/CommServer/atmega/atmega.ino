String str;
void setup(){
 Serial.begin(115200);
 Serial1.begin(115200);
 Serial.print(char(169)); // Copyright Symbol
 Serial.println(" Myengineeringstuffs.com");
 delay(2000);
}
void loop()
{
  str =String('H')+String("hello")+String('T')+String("tom");
  Serial1.println(str);
  delay(500);
}
