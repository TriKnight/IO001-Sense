


// INITIAL Parameters
int SERIAL_BAUD = 9600;  // Serial port BAUD rate
int BEE_BAUD = 19200;  // Bee BAUD rate (9600 is default)
int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)

void setup()
{
  pinMode(BEE_DTR_PIN, OUTPUT); // . This is enable the DeepSleep Mode
  digitalWrite(BEE_DTR_PIN, LOW); // Output LOW- Disabled the Sleep Mode, Output-HIGH- Enable SLeep Mode
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(BEE_BAUD); // Enable the Serial Communication with the Sim Module
  Serial.println("Initializing...");
  delay(1000);
  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  Serial1.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  Serial1.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  Serial1.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();




}

void loop()
{
  digitalWrite(23, LOW);
  updateSerial();
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial1.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial1.available())
  {
    Serial.write(Serial1.read());//Forward what Software Serial received to Serial Port
  }
}
