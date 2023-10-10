/*********
Author: Robotlab.vn
Program: Grinding tool Programming
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define Variable
const int analogInPin = A12;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 5; // Analog output pin that the LED is attached to
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int voltageValue = 0; 
int forceValue = 0;
int floatVoltage=0;
int mV=0;

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Robotics Griding Tool");
  display.display(); 
  delay(100);
  display.setCursor(0, 20);
  // Display static text
  display.println("Robotlab.vn");
  display.display(); 
  delay(100);
 
}

void loop() {
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Robotics Griding Tool");
  
  // Display static text
 // display.setCursor(30, 20);
 // display.println("Robotlab");
  
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  // Voltage value 
  voltageValue = map(sensorValue, 0, 1023, 0, 297);
  floatVoltage = voltageValue/100;
  mV= voltageValue%100;
  // Force value
  forceValue = map(voltageValue, 100, 297, 0, 111);
  
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);
  
  // Display Voltage and Force
  display.setCursor(0, 20);
  display.println(floatVoltage);
  display.setCursor(5, 20);
  display.println(".");
  display.setCursor(10, 20);
  display.println(mV);
  display.setCursor(30, 20);
  display.println("V");
  display.setCursor(90, 20);
  display.println(forceValue);
  display.setCursor(115, 20);
  display.println("N");
// Display Value of Force and Input
  display.setTextSize(1);
  display.setCursor(5, 35);
  // Display Force Value
  display.println("Force ");
 
  
  // Display Force Value
  display.setCursor(80, 35);
  display.println("Input ");

  // Display Value of Force and Input
  display.setTextSize(2);
  display.setCursor(5, 50);
  display.println(forceValue);
  // Display Newton Unit
  display.setCursor(45, 50);
  display.println("N");

  
  // Display Value of Input
  display.setTextSize(2);
  display.setCursor(80, 50);
  display.println(sensorValue);
  // Draw a Line
  display.drawLine(65, 32, 65, 64, WHITE);
  display.drawLine(0, 32, 128, 32, WHITE);
  display.drawLine(0, 15, 128, 15, WHITE);
  display.display(); 
  delay(100);
  


  // print the results to the Serial Monitor:
 // Serial.print("sensor = ");
 // Serial.print(sensorValue);
 // Serial.print("\t output = ");
 // Serial.println(outputValue);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
 
}
