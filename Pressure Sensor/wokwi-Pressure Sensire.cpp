#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <LiquidCrystal_I2C.h>

Adafruit_BMP085 bmp;
// If your LCD displays blocks or doesn't turn on, change 0x27 to 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2); 

unsigned long previousMillis = 0;
const long interval = 3000; // Switch display view every 3 seconds
bool displayScreen = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("BMP180 Barometer");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // Initialize BMP180 sensor
  if (!bmp.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("BMP180 Error!");
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1) {
      delay(10);
    }
  }
}

void loop() {
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0; // Convert Pascals to hPa
  float altitude = bmp.readAltitude(101325);    // Standard sea level pressure

  // Backup output to Serial Monitor
  Serial.print("Temp: "); Serial.print(temp); Serial.print(" C | ");
  Serial.print("Press: "); Serial.print(pressure); Serial.print(" hPa | ");
  Serial.print("Alt: "); Serial.print(altitude); Serial.println(" m");

  // Toggle display screen every 3 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    displayScreen = !displayScreen;
    lcd.clear();
  }

  if (!displayScreen) {
    // Screen 1: Temperature and Pressure
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp, 1);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Press:");
    lcd.print(pressure, 1);
    lcd.print("hPa");
  } else {
    // Screen 2: Altitude
    lcd.setCursor(0, 0);
    lcd.print("Altitude:       ");
    
    lcd.setCursor(0, 1);
    lcd.print(altitude, 1);
    lcd.print(" meters     ");
  }

  delay(500);
}