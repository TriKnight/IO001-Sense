

#include <GPRSbee.h>
// INITIAL Parameters
int SERIAL_BAUD = 9600;  // Serial port BAUD rate
int BEE_BAUD = 19200;  // Bee BAUD rate (9600 is default)
int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)
#define APN "v-internet"
int BEECTS = 8;
GPRSbeeClass GPRS;
void send_SMS()
{
  GPRS.sendSMS("+84356250455", "Hello anh Trí, em là Robot IoT");
}

void setup()
{
    pinMode(BEE_DTR_PIN,OUTPUT); // . This is enable the DeepSleep Mode
    digitalWrite(BEE_DTR_PIN,LOW); // Output LOW- Disabled the Sleep Mode, Output-HIGH- Enable SLeep Mode
    Serial.begin(SERIAL_BAUD);
    Serial1.begin(BEE_BAUD); // Enable the Serial Communication with the Sim Module
  
  //Serial.println("Initializing...");
  //delay(1000);
  //Serial1.println("AT"); //Once the handshake test is successful, i t will back to OK
  //updateSerial();
  //GPRS.init(Serial1, BEECTS,BEE_DTR_PIN);
  GPRS.off();
  GPRS.sendSMS("+84356250455", "Hello anh Trí, em là Robot IoT");
  updateSerial();
  Serial1.write(26); //  Call the number
  
  

 




}

void loop()
{
  //digitalWrite(23, LOW);
 // updateSerial();
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
