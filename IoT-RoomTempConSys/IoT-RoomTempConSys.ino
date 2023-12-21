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

void setup() {
  // Initialize the Arduino MKR IoT Carrier
  carrier.begin();

  // Initialize the serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
   
  // Read temperature and humidity from the environmental sensor
  temperature = carrier.Env.readTemperature() - 6.43;
  humidity = carrier.Env.readHumidity();

  // Print the temperature and humidity readings to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

// Display the temperature and humidity on the center of the screen
carrier.display.fillScreen(0); // Clear the screen
carrier.display.setCursor(50, 100);
carrier.display.setTextSize(2);
carrier.display.setTextColor(0xFFE0); 
carrier.display.print("Temp: ");
carrier.display.println(temperature);
carrier.display.print("Humidity: ");
carrier.display.println(humidity);
}