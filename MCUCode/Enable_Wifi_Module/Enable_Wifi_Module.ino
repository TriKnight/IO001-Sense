

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(46, OUTPUT);
pinMode(33, OUTPUT);
digitalWrite(46, HIGH);
digitalWrite(33, HIGH);
  Serial.begin(74880);
  Serial2.begin(74880);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(46, HIGH);//Enable ESP 8266
digitalWrite(33, HIGH); // Running mode
  if(Serial2.available())
    Serial.write(Serial2.read());
  if(Serial.available())
    Serial2.write(Serial.read());
}
