/*
  Reading CO2, humidity and temperature from the SCD30
  By: Nathan Seidle
  SparkFun Electronics
  Date: May 22nd, 2018
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/15112

  This example prints the current CO2 level, relative humidity, and temperature in C.

  Hardware Connections:
  Attach RedBoard to computer using a USB cable.
  Connect SCD30 to RedBoard using Qwiic cable.
  Open Serial Monitor at 115200 baud.
*/

#include <Wire.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#include "SparkFun_SCD30_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD30

SCD30 airSensor;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define DHTPIN 6

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{ // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello, World!");

  Serial.begin(9600);
  
  //DHT11 set up
  Serial.println(F("DHTxx test!"));
  dht.begin();

  //SCD30 set up
  Serial.println("SCD30 Example");
  Wire.begin();

  if (airSensor.begin() == false)
  {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1)
      ;
  }

  //The SCD30 has data ready every two seconds

  //set up column headers
  Serial.println("Temp out(C), Temp in(C), CO2(ppm), Humidity(%)");
}

void loop()
{ 
  lcd.setCursor(0, 0);

  //DHT11 read room temperature in Celsius
  float t = dht.readTemperature();
  

  //check DHT11 reading
  if (isnan(t)) {
    Serial.println(F("Failed to read DHT11!"));
    return;
  }
  //print room temperature to LCD
  lcd.print(F("t(out):"));
  lcd.print(t);
  Serial.print(t);
  Serial.print(", ");

  if (airSensor.dataAvailable())
  { float tin = airSensor.getTemperature();
    float co2 = airSensor.getCO2();
    float h = airSensor.getHumidity();


    lcd.print(" t(in):");
    lcd.print(tin, 1);
    Serial.print(tin, 1);
    Serial.print(", ");

    lcd.setCursor(0, 1);
    lcd.print("co2(ppm):");
    lcd.print(co2);
    Serial.print(co2);
    Serial.print(", ");

    lcd.print(" humidity(%):");
    lcd.print(h, 1);
    Serial.print(h, 1);
    Serial.print(", ");

    Serial.println(millis());

    delay(1200);

    for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
      lcd.scrollDisplayLeft();
      delay(300);
    }

    //Serial.println();
  }
  else
    Serial.println("Waiting for new data");

  

  delay(1000);
  lcd.clear();
}
