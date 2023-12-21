/* Fill-in information from Blynk Device Info tab  here 
#define BLYNK_TEMPLATE_ID "xxx"
#define BLYNK_TEMPLATE_NAME "xxx"
#define BLYNK_AUTH_TOKEN "xxx"
*/
// #include <SPI.h>
// #include <WiFi.h>
// #include <BlynkSimpleWifi.h>
#include <Arduino_MKRIoTCarrier.h>


// char ssid[] = SSID;
// char pass[] = PASSWORD;


// Create an instance of the MKRIoTCarrier class
MKRIoTCarrier carrier;

// Variables to store temperature and humidity readings
float temperature;
float humidity;

// Temperature thresholds
float minTempThreshold = 20.0;
float maxTempThreshold = 22.0;

void setup() {
  // Initialize the Arduino MKR IoT Carrier
  carrier.begin();
  carrier.leds.clear();
  carrier.leds.show();
  // Initialize the serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  // Read temperature and humidity
  temperature = carrier.Env.readTemperature() - 6.43;
  humidity = carrier.Env.readHumidity();

  // Print data to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Display the temperature and humidity on the center of the screen
  carrier.display.fillScreen(0);  // Clear the screen
  carrier.display.setCursor(50, 100);
  carrier.display.setTextSize(2);
  carrier.display.setTextColor(0xFFE0);
  carrier.display.print("Temp: ");
  carrier.display.println(temperature);
  carrier.display.print("Humidity: ");
  carrier.display.println(humidity);

  if (temperature < minTempThreshold) {
    // temp below min threshold, switch on the blue LED
    carrier.leds.fill(carrier.leds.Color(0, 73, 255));
  } else if (temperature > maxTempThreshold) {
    // temp above max threshold, switch on the red LED
    carrier.leds.fill(carrier.leds.Color(255, 0, 0));
  } else {
    // temp in between thresholds, switch off leds
    carrier.leds.clear();
  }

  // Update the state of the LEDs
  carrier.leds.show();

  delay(1000);
}