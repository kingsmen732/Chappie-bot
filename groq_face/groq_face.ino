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
#include <HTTPClient.h>
#include <ArduinoJson.h>

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

// Groq API Credentials
const char* apiEndpoint = "https://api.groq.com/openai/v1/chat/completions";
const char* apiKey = "";

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

  // Startup Robo Eyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 120);

  // Define automated behaviors
  roboEyes.setAutoblinker(ON, 3, 2); // Auto blink -> active, interval, variation
  roboEyes.setIdleMode(OFF, 0, 0); // Disable idle mode (controlled manually)
} // end of setup

String getHumanLikeBehavior() {
  HTTPClient http;
  http.begin(apiEndpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", String("Bearer ") + apiKey);

  String requestBody =
    "{\"model\": \"llama3-8b-8192\", \"messages\": ["
    "{\"role\": \"system\", \"content\": \"Act like a human and reply with one word describing your current emotion or action. Choose only from: HAPPY, ANGRY, TIRED, CONFUSED, LAUGHING.\"}]}";

  int httpResponseCode = http.POST(requestBody);
  String response = "";

  if (httpResponseCode > 0) {
    response = http.getString();
    Serial.println("Response:");
    Serial.println(response);
  } else {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, response);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return "";
  }

  const char* content = doc["choices"][0]["message"]["content"];
  return String(content).substring(0, 20); // Sanitize output length
}

void loop() {
  unsigned long currentTime = millis();

  // Change emotion and position at random intervals
  if (currentTime - lastChangeTime > changeInterval) {
    lastChangeTime = currentTime;
    changeInterval = random(1000, 4000);

    // Get AI-generated human-like emotion
    String mood = getHumanLikeBehavior();
    mood.trim();
    mood.toUpperCase();

    if (mood == "HAPPY") roboEyes.setMood(HAPPY);
    else if (mood == "ANGRY") roboEyes.setMood(ANGRY);
    else if (mood == "TIRED") roboEyes.setMood(TIRED);
    else if (mood == "CONFUSED") roboEyes.anim_confused();
    else if (mood == "LAUGHING") roboEyes.anim_laugh();
    else roboEyes.setMood(DEFAULT);

    Serial.print("Mood: ");
    Serial.println(mood);
    Serial.println("Eyes changed based on AI suggestion!");
  }

  roboEyes.update();
} // end of loop
