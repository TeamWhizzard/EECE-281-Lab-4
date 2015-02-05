/* EECE 281
 * Lab 4
 *
 *      Team Whizzard is...
 *                           .
 *               /^\     .
 *          /\   "V"
 *         /__\   I      L  o           John Deppe
 *        //..\\  I        a               Theresa Mammarella
 *        \].`[/  I       b                   Steven Olsen
 *       /l\/j\  (]    .   2
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

#define DHTTYPE DHT11
#define TRUE 1
#define FALSE 0

const int pinButton = 5;
const int pinTemp = A6;
const int pinDHT = 4;
const int pinLight = A1;
const int pinBuzzer = 6;

boolean chimeIn = false;
boolean firstRun = true;

// variables to hold values from weather sensing inputs
int buttonState;
int tempVal;
int humidVal;
int lightVal;

DHT dht(pinDHT, DHTTYPE); // initialize DHT sensor
// Note: For our setup, ensure pinMode(INPUT_PULLUP) is set in DHT library
LiquidCrystal lcd(2, 3, 8, 9, 10, 11); // initialize LCD display

void setup() {
  // TODO does baud rate matter for external software? yep.
  Serial.begin(115200); //Serial Port Initialization

  pinMode (pinButton, INPUT_PULLUP); // initialize button
  pinMode (pinBuzzer, INPUT); // Piezo Buzzer

  dht.begin(); // sensor begins reading
  lcd.begin(16, 2);

  serial_lcd_intro(); // introductory messages
}

void loop() {
  // TODO delays needed for readings of DHT?
  buttonState = digitalRead(pinButton);
  tempVal = analogRead(pinTemp) * 0.48828125; // reads temperature in celcius
  humidVal = dht.readHumidity(); // reads humidity from DHT, returns percentage
  // light val devide by 10 is approx the percentage out of 100, devide by 2 is flooring values
  lightVal = analogRead(pinLight) / 20; // reads ambiant light, returns percentage out of max

  // check that DHT is reading correctly
  if (isnan(humidVal)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // switch is off, output on serial monitor
  if (buttonState == LOW) {
    serial_report(tempVal, humidVal, lightVal);  // printed values are to be read by external software Processing and MatLab
    if ((chimeIn == true) && (firstRun == false)) {
      buzzer(chimeIn);
      chimeIn = false;
    }
  } else {
    Serial.println(String(tempVal) + " " + String(humidVal) + " " + String(lightVal));
    if ((chimeIn == false) && (firstRun == false)) {
      buzzer(chimeIn);
      chimeIn = true;
    }
  }
  lcd_report(tempVal, humidVal, lightVal); // lcd message display not dependent on button
  firstRun = false;
}

void serial_report(int temp, int humid, int light) {
  Serial.println(F("Your Current Weather Reading is:"));
  Serial.println("Temperature at " + String(temp) + " degrees Celcius / " + \
                 String(dht.convertCtoF(temp)) + " degrees Fehrenheit.");
  Serial.println("Humidity at " + String(humid) + " percent.");
  Serial.println("Ambient light at " + String(light) + " percent.");
  Serial.println();

  if (tempVal < 10) Serial.println(F("Brrrrr.  It's cold outside!"));
  else if ((tempVal > 10) && (tempVal < 20)) Serial.println(F("You might need a jacket today."));
  else Serial.println(F("Wow, it feels like summer!"));
  Serial.println();
}

void lcd_report(int temp, int humid, int light) {
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print(F("Temp Light Humid"));

  lcd.setCursor (0, 1);
  lcd.print (String(temp) + "C");

  lcd.setCursor (6, 1);
  lcd.print (String(light) + "%");

  lcd.setCursor (12, 1);
  lcd.print (String(humid) + "%");

  delay(1000);

}

void serial_lcd_intro (void) {
  // serial monitor intro
  Serial.println(F("Welcome to Team Whizzard's Amazing Weather Machine!"));

  // lcd intro
  lcd.setCursor(0, 0);
  lcd.print(F("Whizzard is prou")); // first 16 characters of message
  lcd.setCursor(0, 1);
  lcd.print(F("d to present:")); // remaining characters of message
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("Weather Machine!"));
  delay(1000);

  Serial.println();
}

void buzzer(boolean in) {
  int chime[3] = {294, 370, 587};
  if (in) {
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
