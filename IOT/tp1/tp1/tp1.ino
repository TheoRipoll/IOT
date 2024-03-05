// ledPin refers to ESP32 GPIO 19 : Broche D19 sur la carte
const int ledPin = 19;
#include "OneWire.h"
#include "DallasTemperature.h"
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);


int numberKeyPresses = 0;

// Setup function runs once when you press reset or power the board
void IRAM_ATTR isr() { // Interrupt Handler
numberKeyPresses++;
}

void setup() {
  Serial.begin(9600);
  Serial.println("** This message will only be displayed on start or reset. **");
  tempSensor.begin();
  pinMode(ledPin, OUTPUT);

  //PWM
  pinMode(26, INPUT_PULLUP); // broche 26
  attachInterrupt(26, isr, FALLING); // handler

  ledcAttachPin(27, 0); // broche 27, canal 0.
  ledcSetup(0, 25000, 8);

  ledcWrite(0, 255);

}

void loop() {
  //temp//
  float t;
  tempSensor.requestTemperaturesByIndex(0);
  t = tempSensor.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" C\n");

  delay(1000);

}
