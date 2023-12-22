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

// Simulated temp for testing
//float simulatedTemperature = 23.0;

// Temperature thresholds with hysteresis
float minTurnOnThreshold = 22.5;   // Temperature to turn on the LED
float minTurnOffThreshold = 22.0;  // Temperature to turn off the LED

void setup() {
  // Initialize the Arduino MKR IoT Carrier
  carrier.begin();
  carrier.leds.clear();
  carrier.leds.show();
  // Initialize the serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  // For simulated temp only
  // temperature = simulatedTemperature;


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

  // Check temperature against hysteresis thresholds
  if (temperature < minTurnOffThreshold) {
    // temp below turn-off threshold, switch off the LED
    carrier.leds.clear();
  } else if (temperature > minTurnOnThreshold) {
    // temp above turn-on threshold, switch on the blue LED
    carrier.leds.fill(carrier.leds.Color(0, 73, 255));
  }

  // Update the state of the LEDs
  carrier.leds.show();

  delay(1000);
}