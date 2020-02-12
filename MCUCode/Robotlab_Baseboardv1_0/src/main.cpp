
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
#include <RTCTimer.h>
#include <Sodaq_DS3231.h>
#include <Sodaq_PcInt_PCINT0.h>
#include <GPRSbee.h>

//--------------------------------
// 2.Device Registration and sampling feature
//--------------------------------
// Data header, for data log file on SD card
const char *SKETCH_NAME ="RoboIot001_Sketch";
const char *LOGGERNAME = "RoboIoT001";
const char *FILE_NAME = "SENSE001";
const char *DATA_HEADER = "JSON Formatted Data";
const int TIME_ZONE = +0;
// Register your site and get these tokens 
const char *REGISTRATION_TOKEN = "3522beb5-f508-458f-b3f2-f37b00068bc7";
const char *SAMPLING_FEATURE = "5bdba7fd-d356-410b-b75f-a44c2a4eea77";
const char *ONBOARD_TEMPERATURE_UUID = "ad6b2392-1e32-4f95-b8f7-394f98957b8a";
const char *ONBOARD_BATTERY_UUID = "443f94f4-9e1e-4068-bdc6-ba0496bfbcbe";
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

Sodaq_DS3231 sodaq;   // Controls the Real Time Clock Chip
RTCTimer timer;  // The timer functions for the RTC
SdFat SD;  // The SD initialization
String fileName = String(FILE_NAME);  // For the file name
TinyGsm modem(SerialAT);
BlynkTimer timer_blynk;
//--------------------------------
// 8. Working Function
//--------------------------------

// Communication Function
// Timmer Event connect to the Blynk App
void Blynk_app()
{ 
  rtc.convertTemperature();  //convert current temperature into registers
  float tempVal = rtc.getTemperature();
  #define V5  5
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, tempVal);

}

void Blynk_sleep(){
  //Setup Sleep Mode for Modulde
  digitalWrite(BEE_DTR_PIN,HIGH); // Enable SLEEP Mode
  DBG("SetDTR-HIGH Enable Sleep Mode");
  modem.sleepEnable();
  DBG("Sleep Mode");
  modem.radioOff();
  DBG("Radiooff.");
  modem.poweroff();
  DBG("Poweroff.");
  
  delay(50);
}

// Setup the connection GSM and Blynk App
void Blynk_setup(){
  // Start GSM
    //TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);
    digitalWrite(BEE_DTR_PIN,LOW); // Diable SLEEP Mode
    SerialAT.begin(BEE_BAUD);
    delay(1000);
    SerialMon.println("Setting modem...");
    modem.restart();
    //Start Blink App
    Blynk.begin(auth, modem, apn, user, pass);
    
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
uint32_t getNow()
{
  currentepochtime = rtc.now().getEpoch();
  currentepochtime += TIME_ZONE*3600;
  return currentepochtime;
}

// This function returns the datetime from the realtime clock as an ISO 8601 formated string
String getDateTime_ISO8601(void)
{
  String dateTimeStr;
  //Create a DateTime object from the current time
  DateTime dt(rtc.makeDateTime(getNow()));
  //Convert it to a String
  dt.addToString(dateTimeStr);
  dateTimeStr.replace(F(" "), F("T"));
  String tzString = String(TIME_ZONE);
  if (-24 <= TIME_ZONE && TIME_ZONE <= -10)
  {
      tzString += F(":00");
  }
  else if (-10 < TIME_ZONE && TIME_ZONE < 0)
  {
      tzString = tzString.substring(0,1) + F("0") + tzString.substring(1,2) + F(":00");
  }
  else if (TIME_ZONE == 0)
  {
      tzString = F("Z");
  }
  else if (0 < TIME_ZONE && TIME_ZONE < 10)
  {
      tzString = "+0" + tzString + F(":00");
  }
  else if (10 <= TIME_ZONE && TIME_ZONE <= 24)
  {
      tzString = "+" + tzString + F(":00");
  }
  dateTimeStr += tzString;
  return dateTimeStr;
}

// This sets up the function to be called by the timer with no return of its own.
// This structure is required by the timer library.
// See http://support.sodaq.com/sodaq-one/adding-a-timer-to-schedule-readings/
void showTime(uint32_t ts)
{
  // Retrieve the current date/time
  String dateTime = getDateTime_ISO8601();
  return;
}

// Set-up the RTC Timer events
void setupTimer()
{
  // This tells the timer how often (READ_DELAY) it will repeat some function (showTime)
  timer.every(READ_DELAY, showTime);

  // Instruct the RTCTimer how to get the current time reading (ie, what function to use)
  timer.setNowCallback(getNow);
}
void wakeISR()
{
  //Leave this blank
}

// Sets up the sleep mode (used on device wake-up)
void setupSleep()
{
  pinMode(RTC_PIN, INPUT_PULLUP);
  PcInt::attachInterrupt(RTC_PIN, wakeISR);

  //Setup the RTC in interrupt mode
  rtc.enableInterrupts(RTC_INT_PERIOD);

  //Set the sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void sensorsSleep()
{
  // Add any code which your sensors require before sleep
}

void sensorsWake()
{
  // Add any code which your sensors require after waking
}

// Puts the system to sleep to conserve battery life.

void systemSleep()
{
    // Sleep Blynk App
 // Blynk_sleep();
  // This method handles any sensor specific sleep setup
  sensorsSleep();

  // Wait until the serial ports have finished transmitting
  Serial.flush();
  Serial1.flush();
  
  // The next timed interrupt will not be sent until this is cleared
  rtc.clearINTStatus();

  // Disable ADC
  ADCSRA &= ~_BV(ADEN);


  // Sleep time
  noInterrupts();
  sleep_enable();
  interrupts();
  sleep_cpu();
  sleep_disable();

  // Enable ADC
  ADCSRA |= _BV(ADEN);

  // This method handles any sensor specific wake setup
  sensorsWake();
}
// Initializes the SDcard and prints a header to it
void setupLogFile()
{
  // Initialise the SD card
  if (!SD.begin(SD_SS_PIN))
  {
    Serial.println(F("Error: SD card failed to initialise or is missing."));
    errorBlinking_SD();
  }

  fileName += "_" + getDateTime_ISO8601().substring(0,10) + ".csv";

  // Open the file in write mode
  File logFile = SD.open(fileName, FILE_WRITE);

  logFile.println(LOGGERNAME);
  logFile.println(DATA_HEADER);

  //Close the file to save it
  logFile.close();
}


// This function updates the values for any connected sensors. Need to add code for
// Any sensors connected - this example only uses temperature and battery voltage.
bool updateAllSensors()
{
    // Get the temperature from the Mayfly's real time clock
    rtc.convertTemperature();  //convert current temperature into registers
    float tempVal = rtc.getTemperature();
    ONBOARD_TEMPERATURE = tempVal;

    // Get the battery voltage
    float rawBattery = analogRead(BATTERY_PIN);
    ONBOARD_BATTERY = (3.3 / 1023.) * 1.47 * rawBattery;

    // Return true when finished
    return true;
}
// This function generates the JSON data string that becomes the body of the POST request
// TODO:  Figure out how to not use a string here
String generateSensorDataJSON(void)
{
    String jsonString = "{";
    jsonString += "\"sampling_feature\": \"" + String(SAMPLING_FEATURE) + "\", ";
    jsonString += "\"timestamp\": \"" + getDateTime_ISO8601() + "\", ";
    jsonString += "\"" + String(ONBOARD_TEMPERATURE_UUID) + "\": " + String(ONBOARD_TEMPERATURE) + ", ";
    jsonString += "\"" + String(ONBOARD_BATTERY_UUID) + "\": " + String(ONBOARD_BATTERY);
    jsonString += "}";
    return jsonString;
}
// Writes a string to a text file on the SD Card
void logData(String rec)
{
  // Re-open the file
  File logFile = SD.open(fileName, FILE_WRITE);

  // Write the CSV data
  logFile.println(rec);

  // Close the file to save it
  logFile.close();
}



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
  // Setup Blynk App
    Blynk_setup();

    Wire.begin();
    rtc.begin();
    delay(100);

    // Set up pins for the LED's
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
 
    // Blink the LEDs to show the board is on and starting up
    greenred4flash();
    
     // Set up the log file
    setupLogFile();

    // Setup timer events
    setupTimer();
    
    // Setup sleep mode
    setupSleep();
     
    Serial.print(F("Now running "));
    Serial.println(SKETCH_NAME);
    Serial.print(F("Time: "));
    Serial.println(getDateTime_ISO8601());
  
    
}

//--------------------------------
// 10. Loop function
//--------------------------------
void loop() {
  
      timer.update();
      if (currentminute % testminute == 0)
    {
        // Turn on the LED
        digitalWrite(GREEN_LED, HIGH);
        
        // Print a few blank lines to show new reading
        Serial.println(F("\n---\n---\n"));
        // Get the sensor value(s), store as string
        updateAllSensors();
        //Save the data record to the log file
        logData(generateSensorDataJSON());
      
        // Turn off the LED
        digitalWrite(GREEN_LED, LOW);
        // Advance the timer
         // Void Blynk app
        Blynk.run();
        Blynk_app();
        testtimer++;
    }

    if (testtimer >= LOGGING_INTERVAL)
    {
       testminute = LOGGING_INTERVAL;
    }

     // Sleep
    delay(UPDATE_RATE);
    systemSleep();

}