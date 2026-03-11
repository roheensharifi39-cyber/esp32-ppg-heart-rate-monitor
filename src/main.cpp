#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 particleSensor;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

float beatsPerMinute = 0;
int beatAvg = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(A4, A5);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(18, 20);
  display.println("Starting");
  display.display();
  delay(1000);

  Serial.println("Initializing sensor...");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 not found");

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("MAX30102 not found");
    display.display();

    while (1);
  }

  Serial.println("Sensor ready!");

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeIR(0x1F);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sensor ready");
  display.setCursor(0, 16);
  display.println("Place finger...");
  display.display();

  Serial.println("Place your finger on the sensor.");
}

void loop() {
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60.0 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++) {
        beatAvg += rates[x];
      }
      beatAvg /= RATE_SIZE;
    }
  }

  if (irValue < 50000) {
    beatsPerMinute = 0;
    beatAvg = 0;
  }

  Serial.print(irValue);
  Serial.print(",");
  Serial.print(beatsPerMinute);
  Serial.print(",");
  Serial.println(beatAvg);

  // OLED display
  display.clearDisplay();

  if (irValue < 50000) {
    display.setTextSize(1);
    display.setCursor(0, 8);
    display.println("Place finger");
    display.setCursor(0, 22);
    display.println("on sensor");
    display.setCursor(0, 44);
    display.print("IR:");
    display.print(irValue);
  } else {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("BPM");

    display.setTextSize(2);
    display.setCursor(0, 12);
    display.print((int)beatsPerMinute);

    display.setTextSize(1);
    display.setCursor(70, 0);
    display.print("AVG");

    display.setTextSize(2);
    display.setCursor(70, 12);
    display.print(beatAvg);

    display.setTextSize(1);
    display.setCursor(0, 46);
    display.print("IR:");
    display.print(irValue);
  }

  display.display();
  delay(20);
}