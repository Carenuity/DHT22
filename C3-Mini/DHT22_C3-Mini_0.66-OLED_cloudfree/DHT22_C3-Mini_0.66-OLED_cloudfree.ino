#include "DHT.h"                // DHT Library
#include <Wire.h>               // For I2C communication
#include <U8g2lib.h>            // OLED Display library
#include <Adafruit_NeoPixel.h>   // NeoPixel library

// OLED display connected using I2C
#define SCREEN_ADDRESS 0x3C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Pin to which the DHT11 sensor is connected
const int DHTPIN = 6;
#define DHTTYPE DHT22  // Define the type of DHT sensor (DHT11 or DHT22)

// Initialize the sensor with the pin and type
DHT dht(DHTPIN, DHTTYPE);

// NeoPixel setup
#define PIN 7                 // Pin where the NeoPixel is connected
#define NUMPIXELS 1           // Number of NeoPixels (using one for this example)
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);  // Start serial communication
  Wire.begin();        // Start I2C communication

  dht.begin();         // Initialize DHT sensor
  u8g2.begin();        // Initialize OLED display

  // Initialize NeoPixel
  pixels.begin();
  pixels.setBrightness(150);  // Set brightness (0-255)

  // Set a font for OLED display
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Use a serif font, size 8
}

void loop() {
  delay(2000);  // DHT22 sensor provides a new reading every 2 seconds

  // Read temperature and humidity values
  double humidity = dht.readHumidity();
  double temperatureC = dht.readTemperature();  // Celsius
  double temperatureF = dht.readTemperature(true);  // Fahrenheit

  // Check if the sensor readings are valid
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Error reading data from DHT sensor.");
    return;
  }

  // Clear the display
  u8g2.clearBuffer();

  // Display temperature and humidity on OLED
  u8g2.setCursor(33, 25);
  u8g2.print("Temperature:");
  u8g2.setCursor(32, 37);
  String tempValue = String(temperatureC);
  u8g2.println("   " + tempValue + " *C");

  u8g2.setCursor(32, 47);
  u8g2.print("Humidity:");
  u8g2.setCursor(32, 60);
  String humValue = String(humidity);
  u8g2.println("  " + humValue + "%");

  u8g2.sendBuffer();  // Send buffer to OLED

  // Control NeoPixel based on temperature
  if (temperatureC > 30) {
    pulseRed();  // Pulse red if temperature is above 30°C
  } else {
    pulseGreen();  // Pulse green if temperature is 30°C or below
  }

  delay(2000);  // 2-second delay between readings
}

// Function to pulse the NeoPixel red
void pulseRed() {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Set NeoPixel color to red
  pixels.show();  // Turn on the NeoPixel
  delay(500);
  pixels.clear();  // Turn off the NeoPixel
  pixels.show();
  delay(1500);  // Delay between pulses
}

// Function to pulse the NeoPixel green
void pulseGreen() {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Set NeoPixel color to green
  pixels.show();  // Turn on the NeoPixel
  delay(500);
  pixels.clear();  // Turn off the NeoPixel
  pixels.show();
  delay(1500);  // Delay between pulses
}
