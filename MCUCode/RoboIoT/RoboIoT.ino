//This code for the RoboIoT Project 2020. Board v1.0

// I2C Address
// 0x3C --> OLED 0.96 Display
// 0x44 --> SHT30 Temp and Humidity sensor
// 0x68 --> RTC Clock Module
// 0x69 --> MPU-6050 Accelerometer

// Define all Parameters

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SHT3x.h>
SHT3x SHT_Sensor_Air;
int analogPin = A10;


#define I2C_OLED_ADDRESS    0x3C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initialize_device(){
  display.begin(SSD1306_SWITCHCAPVCC,I2C_OLED_ADDRESS);
  display.setTextColor(WHITE);
  display.clearDisplay();
  
  }
void reflesh_oled(float temp_SHT, float humi_SHT, float humi_ground){
  SHT_Sensor_Air.UpdateData();
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("T:");display.print(temp_SHT); display.print(" C");
  display.drawCircle(90, 3, 3, WHITE);
  display.setCursor(0,18);
  display.print("KK:");display.print(humi_SHT); display.print("%");
  display.setCursor(0,36);
  display.print("DAT:");display.print(humi_ground); display.print("%");
  display.display();

  // Display on Serial 
  Serial.print("Temperature: ");
  Serial.print(SHT_Sensor_Air.GetTemperature());
  Serial.print("Humidity: ");
  Serial.print(SHT_Sensor_Air.GetRelHumidity());
  }

void setup() {
  // This code for RoboIoT Project
  //---------------------------

  // This code for the OLED Screen
  Serial.begin(9600);   // Enable the communication PC
  initialize_device();

  
 
  

  
  // This code for the Sim Module
  //------------------------------
//#define DEEP_SLEEP 23 // This is pin for enable DEEP_SLEEP_MODE.
//
//  //---- Code setting up
//  Serial1.begin(19200); // Enable the Serial Communication with Sim Module
//  pinMode(DEEP_SLEEP, OUTPUT);
//  // Output LOW- Disabled the Sleep Mode, Output-HIGH- Enable SLeep Mode
//  //digitalWrite(DEEP_SLEEP, LOW); //Disable DEEP_SLEEP Mode
//  digitalWrite(DEEP_SLEEP, HIGH); //Enable DEEP_SLEEP Mode
//
//
//  Serial.println("Initializing...");
//  delay(1000);
//  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK
//  updateSerial();
//  Serial1.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
//  updateSerial();
//  Serial1.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
//  updateSerial();
//  Serial1.println("AT+CREG?"); //Check whether it has registered in the network
//  updateSerial();
//  Serial1.write(26); //End SMS Command
//


}

void loop() {
  // put your main code here, to run repeatedly:
//  //updateSerial();
  
  float temp_SHT = SHT_Sensor_Air.GetTemperature();
  float humi_SHT = SHT_Sensor_Air.GetRelHumidity();
  float humi_ground= analogRead(analogPin);
  humi_ground= ((1024-humi_ground)/730)*100;
  if(humi_ground >=100){
    humi_ground=100;}
  else if(humi_ground <=0){
    humi_ground =0;}
  reflesh_oled(temp_SHT,humi_SHT,humi_ground);
  delay(333);

  
}
//Update Serial E
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
