String x="  ";
void setup() {
Serial.begin(115200);
}

void loop() {
  if(Serial.available())
    x=Serial.readString();
    Serial.println(x);

}
