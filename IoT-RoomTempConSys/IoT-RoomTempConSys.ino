#include <SPI.h>
#include <WiFi.h>
#include <WiFiNINA.h>
#include <Arduino_MKRIoTCarrier.h>
#include <ThingSpeak.h>
#include "secret.h"


// Wifi and ThingSpeak log in details
char ssid[] = SSID;
char pass[] = PASSWORD;
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

// ThingSpeak mqtt details
const char* mqttServer = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

int status = WL_IDLE_STATUS;

MKRIoTCarrier carrier;
WiFiClient wifiClient;

// Temperature thresholds for hysteresis
float lowerThreshold = 22;    // lower temperature threshold
float upperThreshold = 22.5;  // upper temperature threshold

// Plug state
bool plugState = false;  // false is OFF, true is ON

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    // Wait for the serial port to connect. Needed for the native USB port only
    ;
  }
  setupWiFi();
  ThingSpeak.begin(wifiClient);
  carrier.noCase();
  carrier.begin();
}

void loop() {
  // Read the sensor values
  float temperature = carrier.Env.readTemperature() - 2;

  // Display temperature on the MKR IoT Carrier display
  carrier.display.fillScreen(0);
  carrier.display.setTextSize(2);
  carrier.display.setCursor(50, 100);
  carrier.display.setTextColor(0xF81F);
  carrier.display.print("Temp: ");
  carrier.display.print(temperature);
  carrier.display.println(" C");

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Plug State: ");
  Serial.println(plugState ? "ON" : "OFF");

  // Check if temperature is below the lower threshold
  if (temperature < lowerThreshold && !plugState) {
    switchPlugOn();
    plugState = true;  // Update the state
  }

  // Check if temperature is above the upper threshold
  if (temperature > upperThreshold && plugState) {
    switchPlugOff();
    plugState = false;  // Update the state
  }

  // Set the fields with the values
  ThingSpeak.setField(1, temperature);

  // Update ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) {
    Serial.println("Channel update successful.");
    Serial.print("Room Temp: ");
    Serial.println(temperature);
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(INTERVAL);  // Delay for interval
}

void setupWiFi() {
  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // Don't continue
    while (true)
      ;
  }

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.println("Attempting to connect to SSID");
    // Connect to network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(INTERVAL);
  }

  // You're connected now, so print out the data:
  Serial.println("Connected to the network");
}

void switchPlugOn() {
  // Send request to ThingHTTP to switch on the plug
  Serial.println("Switching on the plug...");

  // Define the ThingHTTP endpoint for switching on the plug
  String endpoint = "/apps/thinghttp/send_request?api_key=" LOW_SWITCH_ON;

  // Make the HTTP request
  makeHttpRequest(endpoint);
}

void switchPlugOff() {
  // Send request to ThingHTTP to switch off the plug
  Serial.println("Switching off the plug...");

  // Define the ThingHTTP endpoint for switching off the plug
  String endpoint = "/apps/thinghttp/send_request?api_key=" HIGH_SWITCH_OFF;

  // Make the HTTP request
  makeHttpRequest(endpoint);
}

void makeHttpRequest(String endpoint) {
  // Make an HTTP request to the specified endpoint

  // Connect to the ThingHTTP server
  if (wifiClient.connect("api.thingspeak.com", 80)) {
    // Make a GET request to the specified endpoint
    wifiClient.print("GET " + endpoint + " HTTP/1.1\r\n" + "Host: api.thingspeak.com\r\n" + "Connection: close\r\n\r\n");

    // Wait for the server to respond
    // delay(500);

    // Read and print the response
    while (wifiClient.available()) {
      Serial.write(wifiClient.read());
    }

    // Close the connection
    wifiClient.stop();
  } else {
    Serial.println("Unable to connect to ThingHTTP server");
  }
}
