#include "DHT.h"  // DHT Library
#include <Wire.h>
#include <U8g2lib.h>

// Declaration for SSD1306 display connected using I2C
#define SCREEN_ADDRESS 0x3C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Pin to which the DHT11 sensor is connected.
// If using the DHT11 Shield, it's digital Pin D4.
const int DHTPIN = 16;

// Specify the type of DHT sensor being used.
#define DHTTYPE DHT22

// Initialize the sensor with the pin and type.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  // Begin serial communication at 9600 Baud.
  Wire.begin();

  dht.begin();  // Start DHT communication.
                // Initialize the OLED object
  u8g2.begin();

  // Set a cool font (choose from options below)
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Nice serif font, size 8
  // u8g2.setFont(u8g2_font_fur20_tn);   // Futuristic, size 20 (works for bigger text)
  // u8g2.setFont(u8g2_font_helvR10_tf);  // Helvetica-like font, size 10 (similar to Arial)
  //u8g2.setFont(u8g2_font_t0_12_tr);    // Pixel font, size 10 (looks retro)
}

void loop() {
  // The DHT22 sensor provides a new reading every 2 seconds,
  // so there's no need to constantly loop in the program.
  delay(2000);

  // Read humidity value.
  double humidity = dht.readHumidity();
  // Read temperature in Celsius.
  double temperatureC = dht.readTemperature();
  // Read temperature in Fahrenheit.
  // The boolean parameter controls whether
  // the temperature is displayed in Fahrenheit or Celsius.
  double temperatureF = dht.readTemperature(true);

  // Check if the values were read successfully.
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Error reading data.");
    return;
  }

  // Initialize by clearing the display.
  u8g2.clearBuffer();

  // Display temperature
  u8g2.setCursor(33, 25);  // Set cursor position
  u8g2.print("Temperature:");
  u8g2.setCursor(32, 37);
  String tempValue = String(temperatureC);
  u8g2.println("   " + tempValue + " *C");

  u8g2.setCursor(32, 47);
  u8g2.print("Humidity:");
  u8g2.setCursor(32, 60);
  String humValue = String(humidity);
  u8g2.println("  " + humValue + "%");

  // Send buffer to the display
  u8g2.sendBuffer();
  delay(1000);
}
