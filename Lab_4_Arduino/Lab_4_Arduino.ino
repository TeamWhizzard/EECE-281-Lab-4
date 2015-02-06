/* EECE 281
 * Lab 4
 *
 *      Team Whizzard is...
 *                           .
 *               /^\     .
 *          /\   "V"
 *         /__\   I      L  .           John Deppe
 *        //..\\  I        a               Theresa Mammarella
 *        \].`[/  I       b                   Steven Olsen
 *       /l\/j\  (]    .   4
 *       /. ~~ ,\/I          .
 *       \\L__j^\/I       o
 *        \/--v}  I     o   .
 *        |    |  I   _________
 *        |    |  I c(`       ')o
 *        |    l  I   \.     ,/
 *       _/j  L l\_!  _//^---^\\_
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include "DHT.h"
#include <LiquidCrystal.h>

#define TRUE 1
#define FALSE 0

#define DHTTYPE DHT11 // DHT hardware type

// pin declarations
const int pinButton = 5;
const int pinDHT = 4;
const int pinLight = A1;
const int pinBuzzer = 6;

// variables to hold values from weather sensing inputs
int newButtonState;
int oldButtonState;
int tempVal;
int humidVal;
int lightVal;

DHT dht(pinDHT, DHTTYPE); // initialize DHT sensor
LiquidCrystal lcd(2, 3, 8, 9, 10, 11); // initialize LCD display

void setup() {
  Serial.begin(115200); //Serial Port Initialization

  pinMode (pinButton, INPUT_PULLUP); // initialize button
  pinMode (pinBuzzer, INPUT); // piezo buzzer

  dht.begin(); // sensor begins reading
  lcd.begin(16, 2);

  serial_lcd_intro(); // introductory messages
}

void loop() {
  tempVal = dht.readTemperature(); // reads temperature in celcius
  humidVal = dht.readHumidity(); // reads humidity from DHT, returns percentage
  // light val devide by 10 is approx the percentage out of 100, devide by 2 is flooring values
  lightVal = analogRead(pinLight) / 20; // reads ambiant light, returns percentage out of max

  if (isnan(humidVal)) { // check that DHT is reading correctly
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  oldButtonState = newButtonState;
  newButtonState = digitalRead(pinButton);
  
  if (newButtonState == LOW) { // switch is off, output on serial monitor
    serial_report(tempVal, humidVal, lightVal);
    
  } else {
    Serial.println(String(tempVal) + " " + String(humidVal) + " " + String(lightVal)); // printed values are to be read by external software Processing and MatLab
  }
  
  if (oldButtonState != newButtonState) { // play chime if changing verbosity mode
      buzzer(newButtonState);
  }
  
  lcd_report(tempVal, humidVal, lightVal); // lcd message display not dependent on button
}

// prints verbose weather report to Arduino serial monitor
void serial_report(int temp, int humid, int light) {
  Serial.println(F("Your Current Weather Reading is:"));
  Serial.println("Temperature at " + String(temp) + " degrees Celsius / " +
                 String(dht.convertCtoF(temp)) + " degrees Fehrenheit.");
  Serial.println("Humidity at " + String(humid) + " percent.");
  Serial.println("Ambient light at " + String(light) + " percent.");
  Serial.println();

  if (tempVal < 10) Serial.println(F("Brrrrr.  It's cold outside!"));
  else if ((tempVal > 10) && (tempVal < 20)) Serial.println(F("You might need a jacket today."));
  else Serial.println(F("Wow, it feels like summer!"));
  
  Serial.println(); // report spacing
}

// print weather report to 16*2 LCD display
void lcd_report(int temp, int humid, int light) {
  clear();
  
  lcd.setCursor (0, 1);
  lcd.print (String(temp) + "C  ");

  lcd.setCursor (6, 1);
  lcd.print (String(light) + "%  ");

  lcd.setCursor (12, 1);
  lcd.print (String(humid) + "%  ");

  delay(2000);
}

// introductory LCD messages
void serial_lcd_intro (void) {
  lcd.setCursor(0, 0);
  lcd.print(F(" Team Whizzard  ")); // first 16 characters of message
  lcd.setCursor(0, 1);
  lcd.print(F("   presents:    ")); // remaining characters of message
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("Weather Machine!"));
  delay(1000);
  lcd.clear();
  
  lcd.setCursor (0, 0);
  lcd.print(F("Temp Light Humid"));
}

// button being on causes ascending chime, else descending
void buzzer(int buttonState) {
  int chime[3] = {294, 370, 587}; // frequencies to be played by buzzer - D4, FS4, D5
  
  if (buttonState == HIGH) {
    for (int i = 0; i < 3; i++) {
      tone(pinBuzzer, chime[i], 200);
      delay(200);
    }
    
  } else {
    for (int i = 3; i >= 0; i--) {
      tone(pinBuzzer, chime[i], 200);
      delay(200);
    }
  }    
}
