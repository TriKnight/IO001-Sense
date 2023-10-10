#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128              // OLED display width, in pixels
#define SCREEN_HEIGHT 64              // OLED display height, in pixels

float temperature, DO;
int device_id = 13469;                // ID of this End node


const int csPin = 11;                 // LoRa radio chip select
const int resetPin = 25;              // LoRa radio reset
const int irqPin = 2;                 // change for your board; must be a hardware interrupt pi

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433175000)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125000);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(0x34);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {          // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Starting Hello");
  display.display(); 
  delay(100);
}

void loop() {
  delay(1000);
  temperature = ((float)random(280,310))/10;         // Generate a random temperature.
  DO = ((float)random(70,180))/10;                   // Generate a random DO.
 int sensorValue = analogRead(A5);
  double voltBatt= sensorValue*(5.11/1023) ;
  // print out the value you read:
  Serial.println(voltBatt+1.2);
  delay(1);        // delay in between reads for stability
  delay(1000); // every second
  // compose and send packet
  LoRa.beginPacket();
  LoRa.print("<");
  LoRa.print(device_id);
  LoRa.print(">field1=");
  LoRa.print(temperature);
  LoRa.print("&field2=");
  LoRa.print(DO); 
  LoRa.print("&field3=");
  LoRa.print(voltBatt+1.2); 
  LoRa.endPacket();

  display.clearDisplay();
  display.drawLine(display.width()/5*2, 0, display.width()/5*2, display.height(), WHITE);
  display.drawLine(0, display.height()/3-5, display.width(), display.height()/3-5, WHITE);


  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Batt");
  display.setCursor(0, display.height()/3);
  display.setTextSize(2);
  display.print(voltBatt+1.2, 2);
  display.setCursor(display.width()/5*2-25, display.height()/4*3);
  display.setTextSize(2);
  //display.print((char)247);
  display.println("V");

  display.setTextSize(2);
  display.setCursor(display.width()/5*2 + 30, 0);
  display.print("DO");
  display.setCursor(display.width()/5*2 + 5, display.height()/3);
  display.setTextSize(3);
  display.print(DO, 1);
  display.setCursor(display.width()-54 + 5, display.height()/4*3);
  display.setTextSize(2);
  display.println("mg/l");
  display.display(); 
  
  delay(10000);
}
