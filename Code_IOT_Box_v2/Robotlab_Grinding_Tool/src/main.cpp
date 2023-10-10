
//This code for logging data 
// Reference: https://github.com/EnviroDIY/EnviroDIY_Mayfly_Logger/blob/master/examples/logging_to_EnviroDIY/logging_to_EnviroDIY.ino
// Author: Robotlab.vn

//--------------------------------
//1. Include Library
//--------------------------------
#include <Arduino.h>
#include <Wire.h>
#include <avr/sleep.h>
#include <SdFat.h>

//--------------------------------
// 2.Device Registration and sampling feature
//--------------------------------
// Data header, for data log file on SD card
const char *SKETCH_NAME ="RoboIot001_Sketch";
const char *LOGGERNAME = "RoboIoT001";
const char *FILE_NAME = "SENSE001";
const char *DATA_HEADER = "JSON Formatted Data";

//--------------------------------
// 3. Device connection option
//--------------------------------

//--------------------------------
// 4. Timming Option for Logging
//--------------------------------
int LOGGING_INTERVAL = 2;  // How frequently (in minutes) to log data
int READ_DELAY = 2;  // How often (in minutes) the timer wakes up
int UPDATE_RATE = 2000; // How frequently (in milliseconds) the logger checks if it should log
int COMMAND_TIMEOUT = 15000;  // How long (in milliseconds) to wait for a server response

//--------------------------------
// 5. Web Service and Setup
//--------------------------------
#define TINY_GSM_MODEM_SIM800
#define SerialMon Serial
#define SerialAT Serial1
#define TINY_GSM_DEBUG SerialMon
// INITIAL Parameters
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 19200
// Define how you're planning to connect to the internet
#define TINY_GSM_TEST_GPRS true
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_TEST_CALL true
#define TINY_GSM_TEST_SMS false
#define TINY_GSM_TEST_USSD true
#define TINY_GSM_TEST_BATTERY true
//--------------------------------
// 5.1 Tiny GSM Function
//--------------------------------
// powerdown modem after tests
#define TINY_GSM_POWERDOWN false
// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET  "+84356250455"
//#define CALL_TARGET "+380xxxxxxxxx"
// Your GPRS credentials, if any
const char apn[]  = "v-internet";
char user[] = "";
char pass[] = "";
// Include TinyGSM
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
// Go to the Project Settings (nut icon).
char auth[] = "6ek7cPHk1WfC1i-aFhYDcZwSqPtGPqq2";

//--------------------------------
// 6. Board setup Info
//--------------------------------
int SERIAL_BAUD = 9600;  // Serial port BAUD rate
int BEE_BAUD = 19200;  // Bee BAUD rate (9600 is default)
int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)
//int BEE_CTS_PIN = 19;   // Bee CTS Pin (Clear to Send)
int GREEN_LED = 8;  // Pin for the green LED
int RED_LED = 9;  // Pin for the red LED
int RTC_PIN = A8;  // RTC Interrupt pin
#define RTC_INT_PERIOD EveryMinute  //The interrupt period on the RTC
int BATTERY_PIN = A7;    // select the input pin for the potentiometer
int SD_SS_PIN = 12;  // SD Card Pin
//int BEECTS = 8;


//--------------------------------
// 7. Setup Variables
//--------------------------------
float ONBOARD_TEMPERATURE = 0;  // Variable to store the temperature result in
float ONBOARD_BATTERY = 0;  // variable to store the value coming from the sensor

// Variables for the timer function
int currentminute;
int testtimer = 0;
int testminute = 1;
long currentepochtime = 0;


//--------------------------------
// 8. Working Function
//--------------------------------

// Communication Function
// Timmer Event connect to the Blynk App
void Blynk_app()
{ 
 

}

void Blynk_sleep(){
  //Setup Sleep Mode for Modulde

}

// Setup the connection GSM and Blynk App
void Blynk_setup(){
  // Start GSM
    //TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);

    
}
// Flashess to Robolab_Baseboard's LED's
void greenred4flash()
{
  for (int i = 1; i <= 4; i++) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(50);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(50);
  }
  digitalWrite(RED_LED, LOW);
}
// Error Blinking
void errorBlinking_SD()
{
  for (int i = 1; i <= 5; i++) {
    digitalWrite(RED_LED, LOW);
    delay(100);
    digitalWrite(RED_LED, HIGH);
    delay(100);
  }
}


// This function returns the datetime from the realtime clock as an ISO 8601 formated string


//--------------------------------
// 9. Setup Function
//--------------------------------
// Setting Date and Time
// char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

// //writing any non-existent time-data may interfere with normal operation of the RTC.
// //Take care of week-day also.
// //year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
// DateTime dt(2020, 02, 03, 11, 19, 0, 1);


void setup() {
  // //Seting Date and Time
  //   Serial.begin(SERIAL_BAUD);
  //   Wire.begin();
  //   rtc.begin();
  //   rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
  
  // put your setup code here, to run once:
    SerialMon.begin(SERIAL_BAUD);
    // Set up pins for the LED's
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    // Blink the LEDs to show the board is on and starting up
    greenred4flash();
    
  
  
    
}

//--------------------------------
// 10. Loop function
//--------------------------------
void loop() {
  greenred4flash();
  
}