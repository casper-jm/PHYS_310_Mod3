
//www.elegoo.com
//2016.12.9

/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <DS3231.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define RED 2
#define GREEN 4
#define BLUE 3

#define ENABLE 1
#define DIRA 6
#define DIRB 0


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DS3231  rtc(SDA, SCL);

void setup() {
  rtc.begin();
  //rtc.setTime(2, 30, 0);     // Set the time to 12:00:00 (24hr format)
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Hello, World!");
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();

  {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  }

  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

}

int redValue;
int greenValue;
int blueValue;

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr());
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  //delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  lcd.setCursor(9, 0);

  if (h >= 20.00){
    redValue = 255;
    greenValue = 0;
    blueValue = 0;
    lcd.print("DAMP ");
    digitalWrite(ENABLE,HIGH); //enable on
    digitalWrite(DIRA,HIGH); //one way
    digitalWrite(DIRB,LOW);
  
  } else if (h >= 18.00 && h < 20.00){
    redValue = 255;
    greenValue = 255;
    blueValue = 0;
    lcd.print("MOIST");

  } else if (h >= 16.00 && h < 18.00){
    redValue = 0;
    greenValue = 255;
    blueValue = 255;
    lcd.print("OKAY ");
    digitalWrite(ENABLE,LOW); //enable on
    digitalWrite(DIRA,LOW); //one way
    digitalWrite(DIRB,LOW);

  } else {
    redValue = 0;
    greenValue = 0;
    blueValue = 255;
    lcd.print("DRY  ");
  }

  analogWrite(RED, redValue);
  analogWrite(BLUE, blueValue);
  analogWrite(GREEN, greenValue);
  


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(t);
  //lcd.print(F("°C"));
  lcd.print(F(" H:"));
  lcd.print(h);
  lcd.print("%");

  delay(1000);
}