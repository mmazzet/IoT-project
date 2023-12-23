
#define BLYNK_TEMPLATE_ID "TMPL4s3OwgJjV"
#define BLYNK_TEMPLATE_NAME "RoomTempConSys"
#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>
#include <Arduino_MKRIoTCarrier.h>
#include "secret.h"


char ssid[] = SSID;
char pass[] = PASSWORD;
char blynk_auth_token[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;
MKRIoTCarrier carrier;

float temperature;  // Declare the temperature variable
float humidity;
float minTurnOnThreshold = 22.5;  // Temperature to turn on the LED
float minTurnOffThreshold = 22.0;

int blynkButtonState = 0;

BLYNK_WRITE(V0) {
  blynkButtonState = param.asInt();

  // Clear the screen
  carrier.display.fillScreen(0);
  carrier.display.setCursor(50, 150);
  carrier.display.setTextSize(3);

  if (blynkButtonState) {
    carrier.display.setCursor(50, 100);
    carrier.display.setTextColor(0x07E0);  // green
    carrier.display.print("Button ON");
  } else {
    carrier.display.setTextColor(0xF800);  // red
    carrier.display.print("Button OFF");
  }
}

void writeTemperature() {
  // Don't send more than 10 values per second.
  float temperature = carrier.Env.readTemperature() - 6.5;
  Blynk.virtualWrite(V1, temperature);
  // Print temperature to the serial monitor
  Serial.print("Temperature: ");
  Serial.println(temperature);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(blynk_auth_token, ssid, pass);
  timer.setInterval(2000L, writeTemperature);
  carrier.noCase();
  carrier.begin();
  carrier.leds.clear();
  carrier.leds.show();
}

void loop() {
  // temperature = carrier.Env.readTemperature() - 6.5;

  if (temperature < minTurnOffThreshold) {
    carrier.leds.clear();
  } else if (temperature > minTurnOnThreshold) {
    carrier.leds.fill(carrier.leds.Color(0, 73, 255));
  }

  carrier.leds.show();
  // delay(1000);

  Blynk.run();
  timer.run();
}