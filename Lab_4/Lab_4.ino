// lab 4

#include "DHT.h"

#define DHTTYPE DHT11
#define TRUE 1
#define FALSE 0

const int pinButton = 0; // TODO
const int pinDHT = 0; // TODO

int buttonState;
float tempVal;
float humidVal;

DHT dht(pinDHT, DHTTYPE); // initialize DHT sensor

void setup() {
  // TODO does this rate matter for external software?
  Serial.begin(115200); //Serial Port Initialization
  Serial.println("Welcome to Team Whizzard's Amazing Weather Machine!");
  pinMode (pinButton, INPUT); // initialize button
  
  dht.begin(); // sensor begins reading
}

void loop() {
  Serial.println(); // add space for next weather report
  
  // TODO delays on readings?
  buttonState = digitalRead(pinButton);
  tempVal = dht.readTemperature(); // reads temperature are celcius
  humidVal = dht.readHumidity(); // reads humidity from DHT
  
  // check that DHT is reading correctly 
  if (isnan(tempVal) || isnan(humidVal)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // switch is off, output on serial monitor
  if (buttonState == LOW) printReport(tempVal, humidVal);
    
  // printed values are to be read by external software Processing and MatLab
  else Serial.println(String(tempVal) + " " + String(humidVal));
  
  delay(1000);
}

void printReport(float temp, float humid) {
  Serial.println("Your Current Weather Reading is:");
  Serial.println("Temperature at " + String(temp) + " degrees Celcius / " + \
    String(dht.convertCtoF(temp)) + " degrees Fehrenheit.");
  Serial.println("Humidity at " + String(humid * 100) + " percent");
  Serial.println();
  
  if (tempVal < 10) Serial.println("Brrrrr.  It's cold outside!");
  else if (tempVal > 20) Serial.println("You might need a jacket today.");
  else Serial.println("Wow, it feels like summer!");
}
