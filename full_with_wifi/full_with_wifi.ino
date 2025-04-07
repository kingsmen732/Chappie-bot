//***********************************************************************************************
//  This example shows how to create random emotions and movements with the FluxGarage Robo Eyes library. 
//
//  Hardware: You'll need a breadboard, a microcontroller such as Arduino Nano v3 or better an ESP32, 
//  an I2C OLED display with SH1106 chip, and some jumper wires.
//
//  Use the dedicated I2C pins of your microcontroller. On ESP32-WROOM-32 module, use pin 22 for SCL and pin 21 for SDA.
//
//  Published in April 2025 by [kingsmen732]
//***********************************************************************************************

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>

/* Uncomment the correct I2C address. If you get a blank screen, try the other. */
#define i2c_Address 0x3c // I2C addr 0x3C (typically eBay OLEDs)
//#define i2c_Address 0x3d // I2C addr 0x3D (typically Adafruit OLEDs)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   // QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <FluxGarage_RoboEyes.h>
roboEyes roboEyes; // create RoboEyes instance

unsigned long lastChangeTime = 0; // Track the last change time
int changeInterval = 2000; // Initial random change interval

// WiFi Credentials
const char* ssid = "test";
const char* password = "12345678";

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  delay(250); // Wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  //display.setContrast (0); // Dim display

  // Startup Robo Eyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 120); // screen-width, screen-height, max framerate

  // Define automated behaviors
  roboEyes.setAutoblinker(ON, 3, 2); // Auto blink -> active, interval, variation
  roboEyes.setIdleMode(OFF, 0, 0); // Disable idle mode (controlled manually)

} // end of setup

void loop() {
  unsigned long currentTime = millis();

  // Change emotion and position at random intervals
  if (currentTime - lastChangeTime > changeInterval) {
    lastChangeTime = currentTime;
    changeInterval = random(1000, 4000); // Set new random interval (1-4 sec)

    // Set a random position
    int pos = random(9); // 9 possible positions
    switch (pos) {
      case 0: roboEyes.setPosition(N); break;
      case 1: roboEyes.setPosition(NE); break;
      case 2: roboEyes.setPosition(E); break;
      case 3: roboEyes.setPosition(SE); break;
      case 4: roboEyes.setPosition(S); break;
      case 5: roboEyes.setPosition(SW); break;
      case 6: roboEyes.setPosition(W); break;
      case 7: roboEyes.setPosition(NW); break;
      case 8: roboEyes.setPosition(DEFAULT); break;
    }

    // Set a random mood
    int mood = random(5); // 4 moods: DEFAULT, HAPPY, ANGRY, TIRED
    switch (mood) {
      case 0: roboEyes.setMood(DEFAULT); break;
      case 1: roboEyes.setMood(HAPPY); break;
      case 2: roboEyes.setMood(ANGRY); break;
      case 3: roboEyes.setMood(TIRED); break;
      case 4: roboEyes.anim_confused(); break;// confused - eyes shaking left and right
      case 5: roboEyes.anim_laugh(); break;
    }

    Serial.println("Eyes changed!"); // Debug message
  }

  roboEyes.update(); // Update eye drawings
  // Don't use delay() here to ensure smooth animations.
} // end of loop
