// lab 4

#include "DHT.h"
#include <LiquidCrystal.h>

#define DHTTYPE DHT11
#define TRUE 1
#define FALSE 0

const int pinButton = 5;
const int pinTemp = A6;
const int pinDHT = 4;
const int pinLight = A1;

// variables to hold values from weather sensing inputs
int buttonState;
int tempVal;
int humidVal;
int lightVal;

DHT dht(pinDHT, DHTTYPE); // initialize DHT sensor
// Note: For our setup, ensure pinMode(INPUT_PULLUP) is set in DHT library
LiquidCrystal lcd(2,3,8,9,10,11); // initialize LCD display

void setup() {
  // TODO does baud rate matter for external software? yep.
  Serial.begin(115200); //Serial Port Initialization
  
  pinMode (pinButton, INPUT_PULLUP); // initialize button
  
  dht.begin(); // sensor begins reading
  lcd.begin(16, 2);
  
  serial_lcd_intro(); // introductory messages
}

void loop() {
  delay(1000);
  
  Serial.println(); // add space for next weather report
  
  // TODO delays needed for readings of DHT?
  buttonState = digitalRead(pinButton);
  tempVal = analogRead(pinTemp) * 0.48828125; // reads temperature in celcius
  humidVal = dht.readHumidity() * 100; // reads humidity from DHT, returns percentage
  lightVal = analogRead(pinLight) * (100/1024); // reads ambiant light, returns percentage out of max

  // check that DHT is reading correctly 
  if (isnan(humidVal)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  // switch is off, output on serial monitor
  if (buttonState == LOW) serial_report(tempVal, humidVal, lightVal);
    
  // printed values are to be read by external software Processing and MatLab
  else Serial.println(String(tempVal) + " " + String(humidVal) + \
          " " + String(lightVal));
          
  lcd_report(tempVal, humidVal, lightVal); // lcd message display not dependent on button
}

void serial_report(int temp, int humid, int light) {
  Serial.println(F("Your Current Weather Reading is:"));
  Serial.println("Temperature at " + String(temp) + " degrees Celcius / " + \
    String(dht.convertCtoF(temp)) + " degrees Fehrenheit.");
  Serial.println("Humidity at " + String(humid) + " percent.");
  Serial.println("Ambiant light at " + String(light) + " percent.");
  Serial.println();
  
  if (tempVal < 10) Serial.println(F("Brrrrr.  It's cold outside!"));
  else if ((tempVal > 10) && (tempVal < 20)) Serial.println(F("You might need a jacket today."));
  else Serial.println(F("Wow, it feels like summer!"));
}

void lcd_report(int temp, int humid, int light) {
  lcd.clear();
  lcd.setCursor (0,0); 
  lcd.print(F("Temp Light Humid"));
  
  lcd.setCursor (0,1);
  lcd.print(String(temp) + " " + String(light) + String(humid));
  
}

void serial_lcd_intro (void) {
  // serial monitor intro
  Serial.println(F("Welcome to Team Whizzard's Amazing Weather Machine!"));
  
  // lcd intro
  lcd.setCursor(0,0);
  lcd.print(F("Whizzard is prou")); // first 16 characters of message
  lcd.setCursor(0,1);
  lcd.print(F("d to present:")); // remaining characters of message
  delay(1000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(F("Weather Machine!"));
}
