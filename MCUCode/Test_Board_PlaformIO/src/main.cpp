#include <Arduino.h>
#include <RTCTimer.h>
#include <Sodaq_DS3231.h>
#include <Wire.h>

// Select Your Modem
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

// powerdown modem after tests
#define TINY_GSM_POWERDOWN false
// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET  "+84356250455"
//#define CALL_TARGET "+380xxxxxxxxx"
// Your GPRS credentials, if any
const char apn[]  = "v-internet";
const char gprsUser[] = "";
const char gprsPass[] = "";


// Include TinyGSM
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

int BEE_DTR_PIN = 23;  // Bee DTR Pin (Data Terminal Ready - used for sleep)
#define APN "v-internet"
char user[] = "";
char pass[] = "";

TinyGsm modem(SerialAT);
RTCTimer timer;  // The timer functions for the RTC

// Go to the Project Settings (nut icon).
char auth[] = "6ek7cPHk1WfC1i-aFhYDcZwSqPtGPqq2";

// Program with Virtual Pin
BlynkTimer timer_blynk;
void myTimerEvent()
{ 
  rtc.convertTemperature();  //convert current temperature into registers
  float tempVal = rtc.getTemperature();
  #define V5  5
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, tempVal);

}

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

//
void setup()
{

    Wire.begin();
    rtc.begin();
    delay(100);

    pinMode(BEE_DTR_PIN,OUTPUT); // . This is enable the DeepSleep Mode
    digitalWrite(BEE_DTR_PIN,LOW); // Output LOW- Disabled the Sleep Mode, Output-HIGH- Enable SLeep Mode
    Serial.begin(GSM_AUTOBAUD_MIN );
    Serial1.begin(GSM_AUTOBAUD_MAX); // Enable the Serial Communication with the Sim Module
    delay(20);
    TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);
   //SerialAT.begin(9600);
    delay(3000);
    //check Status Module sim Connected
    bool res = modem.isGprsConnected();
    DBG("GPRS status:", res ? "connected" : "not connected");
    String ccid = modem.getSimCCID();
    DBG("CCID:", ccid);
    String cop = modem.getOperator();
    DBG("Operator:", cop);

#if TINY_GSM_TEST_SMS && defined(SMS_TARGET)
  res = modem.sendSMS(SMS_TARGET, String("Hello chi Diem, chi co khoe khong a? "));
  DBG("SMS:", res ? "OK" : "fail");

#endif
#if TINY_GSM_TEST_BATTERY
  uint8_t chargeState = -99;
  int8_t percent = -99;
  uint16_t milliVolts = -9999;
  modem.getBattStats(chargeState, percent, milliVolts);
  DBG("Battery charge state:", chargeState);
  DBG("Battery charge 'percent':", percent);
  DBG("Battery voltage:", milliVolts / 1000.0F);
#endif
#if TINY_GSM_POWERDOWN
  // Try to power-off (modem may decide to restart automatically)
  // To turn off modem completely, please use Reset/Enable pins
  modem.sleepEnable();
  DBG("Sleep Mode");
  modem.radioOff();
  DBG("Radiooff.");
  modem.poweroff();
  DBG("Poweroff.");
  digitalWrite(BEE_DTR_PIN,HIGH); // Enable SLEEP Mode
  DBG("SetDTR-HIGH Enable Sleep Mode");
#endif

// If it is blinking every second, this means it is searching for a network. 
// If it blinks every three seconds,that mean already connect tor the network
// If the LED blinks very fast, this means it's connected through GPRS
#if TINY_GSM_TEST_GPRS
  DBG("Connecting to", apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    delay(10000);
    return;
  }

  bool res_gprs = modem.isGprsConnected();
  DBG("GPRS status:", res_gprs ? "connected" : "not connected");

  String ccid_gprs = modem.getSimCCID();
  DBG("CCID:", ccid_gprs);

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  String cop_gprs = modem.getOperator();
  DBG("Operator:", cop_gprs);

  IPAddress local_gprs = modem.localIP();
  DBG("Local IP:", local_gprs);

  int csq_gprs = modem.getSignalQuality();
  DBG("Signal quality:", csq_gprs);

  // This is only supported on SIMxxx series
  // String gsmLoc = modem.getGsmLocation();
  // DBG("GSM location:", gsmLoc);

  // This is only supported on SIMxxx series
  // String gsmTime = modem.getGSMDateTime(DATE_TIME);
  // DBG("GSM Time:", gsmTime);
  // String gsmDate = modem.getGSMDateTime(DATE_DATE);
  // DBG("GSM Date:", gsmDate);
#endif
Serial.println("Initializing modem...");
  modem.restart();

//Start Blink App
Blynk.begin(auth, modem, apn, user, pass);
timer_blynk.setInterval(1000L, myTimerEvent);
}


void loop()
{
Blynk.run();
timer_blynk.run(); // Initiates BlynkTimer
}
