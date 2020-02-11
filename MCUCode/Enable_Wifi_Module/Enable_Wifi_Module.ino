 

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(46, OUTPUT);
pinMode(33, OUTPUT);
digitalWrite(46, LOW); // This wiring to the pin 3(CH_PD) Enable ESP
//pinMode(49, OUTPUT);  This not wiring by export Gerber Error
digitalWrite(33, LOW);   // This is the GPIO0 of ESP
  Serial.begin(57600);
  Serial2.begin(74880);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(46, LOW);//Enable ESP 8266
digitalWrite(33, LOW); // Running mode
  if(Serial2.available())
    Serial.write(Serial2.read());
  if(Serial.available())
    Serial2.write(Serial.read());
}
