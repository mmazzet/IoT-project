#include <SPI.h>
#include <WiFi.h>
#include <WiFiNINA.h>
#include "Firebase_Arduino_WiFiNINA.h"
#include <Arduino_MKRIoTCarrier.h>
#include <ThingSpeak.h>
#include "secret.h"


// Wifi, Firebase, ThingSpeak log in details
char ssid[] = SSID;
char pass[] = PASSWORD;
char dbUrl[] = DATABASE_URL;
char dbSecret[] = DATABASE_SECRET;
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

int status = WL_IDLE_STATUS;

//Define Firebase data object
FirebaseData fbdo;
const long interval = 5000;
unsigned long previousMillis = millis();
unsigned long currentMillis = millis();

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

  Firebase.begin(dbUrl, dbSecret, ssid, pass);
  Firebase.reconnectWiFi(true);
  carrier.noCase();
  carrier.begin();
}

void loop() {
  // Read the sensor values
  float temperature = carrier.Env.readTemperature();

  // Display temperature on the MKR IoT Carrier display
  carrier.display.fillScreen(0);
  carrier.display.setTextSize(2);
  carrier.display.setCursor(50, 100);
  carrier.display.setTextColor(0xF81F);
  carrier.display.print("Temp: ");
  carrier.display.print(temperature);
  carrier.display.println(" C");

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

  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;
    Serial.println("Sending message: ");
    sendMessage(temperature);
    Serial.println();
  }
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

void sendMessage(float temperature) {
  // send message, the Print interface can be used to set the message contents
  String path = "/events";
  String jsonStr;

  Serial.println("Pushing json... ");

  jsonStr = "{\"event\":\"push\",\"temperature\":" + String(temperature) + "}";
  Serial.println(jsonStr);

  // Push data to Firebase
  if (Firebase.pushJSON(fbdo, path + "/temperature", jsonStr)) {
    Serial.println("Path: " + fbdo.dataPath());
  } else {
    Serial.println("Error: " + fbdo.errorReason());
  }

  // Clear Firebase data object
  fbdo.clear();
}
