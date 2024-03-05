const int ledCoolerPin = 19; // ledPin refers to ESP32 GPIO 19 : Broche D19 sur la carte
const int ledHeaterPin = 21;

#include <ArduinoJson.h>

//Libraries for temp sensor
#include "OneWire.h"
#include "DallasTemperature.h"

//setup for temp sensor
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

// led strip
#include <Adafruit_NeoPixel.h>
#define PIN 13
#define NUMLEDS 5
Adafruit_NeoPixel strip(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);

// Setup function runs once when you press reset or power the board
int numberKeyPresses = 0;
void IRAM_ATTR isr() { // Interrupt Handler
  numberKeyPresses++;
}

// Thresholds
float upperThreshold = 25.0;    // Upper temperature threshold for cooling (SH)
float lowerThreshold = 20.0;    // Lower temperature threshold for heating (SB)

void setup() {
  Serial.begin(9600);
  Serial.println("** This message will only be displayed on start or reset. **");
  tempSensor.begin();
  delay(2 * 1000); // wait for two seconds

  // Initialize/configure digital pin ledPin as an OUTPUT.
  pinMode(ledCoolerPin, OUTPUT);
  pinMode(ledHeaterPin, OUTPUT);

  //PWM
  pinMode(26, INPUT_PULLUP); // broche 26
  attachInterrupt(26, isr, FALLING); // handler

  ledcAttachPin(27, 0); // broche 27, canal 0.
  ledcSetup(0, 25000, 8);

  ledcWrite(0, 255);

  //LED STRIP
  strip.begin();
  delay(1);

  for (int i = 0; i < 1; i++) {
    //turn color to red
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  for (int i = 1; i < 4; i++) {
    //turn color to green
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  for (int i = 4; i < NUMLEDS; i++) {
    //turn color to blue
    strip.setPixelColor(i, strip.Color(0, 0, 255));
  }
  strip.show();
}


// Loop function runs over and over again forever
void loop() {
  //PMW//
  int nb_state = 12;
  int rcv[nb_state] = {0, 64, 64,
                       0, 127, 127,
                       0, 191, 191,
                       0, 255, 255
                      }; // Rapports cycliques si 8 bits
  // 255 => 100%, 127 => 50% et 0 => 0%
  static uint32_t tick = 0;
  static int elapsed;

  static int current = 0; // etat courant

  elapsed = millis() - tick;
  if (elapsed > 10000) { // Toutes les 10 sec on change de Rapport Cyc

    Serial.printf("Fan speed = %f RPM\n", (numberKeyPresses / 10.0) * 30); // 30 = 60/2 cf doc
    numberKeyPresses = 0;

    ledcWrite(0, rcv[current]); // canal = 0, rapport cyclique
    Serial.printf("Rapport Cyc = %lf %%\n", (rcv[current] / 255.0) * 100);

    current += 1; // Prochain rapport cyclique
    current %= nb_state;

    tick = millis();
  }

  //temp//
  float t;
  tempSensor.requestTemperaturesByIndex(0);
  t = tempSensor.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" C\n");

  // PARTIE CLIMATISATION

  if (t > upperThreshold) {
    // Cooling required
    digitalWrite(ledCoolerPin, HIGH);  // Turn on cooler LED
    digitalWrite(ledHeaterPin, LOW);   // Turn off heater LED

    // Fan speed adjustment (proportional control)
    int fanSpeed = map(t, upperThreshold, 30, 0, 255); // Adjust fan speed based on temperature
    ledcWrite(0, fanSpeed); // Set fan speed

    strip.begin();
    delay(1);

    for (int i = 0; i < NUMLEDS; i++) {
      //turn color to red
      strip.setPixelColor(i, strip.Color(255, 5, 0));
    }
    strip.show();
  }

} else if (t < lowerThreshold) {
  // Heating required
  digitalWrite(ledCoolerPin, LOW);   // Turn off cooler LED
  digitalWrite(ledHeaterPin, HIGH);  // Turn on heater LED
  ledcWrite(pwmChannel, 0); // Turn off fan

  strip.begin();
  delay(1);

  for (int i = 0; i < NUMLEDS; i++) {
    //turn color to red
    strip.setPixelColor(i, strip.Color(3, 255, 0));
  }
  strip.show();
}

} else {
  // Temperature within acceptable range
  digitalWrite(ledCoolerPin, LOW);   // Turn off cooler LED
  digitalWrite(ledHeaterPin, LOW);   // Turn off heater LED
  ledcWrite(pwmChannel, 0); // Turn off fan

  strip.begin();
  delay(1);

  for (int i = 0; i < NUMLEDS; i++) {
    //turn color to red
    strip.setPixelColor(i, strip.Color(255, 124, 0));
  }
  strip.show();
}

delay(1000); // Delay for stability

// Light sensor //
int sensorValue;
sensorValue = analogRead(A5);
Serial.println(sensorValue, DEC);



// Led blink //
digitalWrite(ledPin, HIGH); // turn the LED on (HIGH is the voltage level)
Serial.println("in loop");
delay(1 * 1000); // wait for a second
digitalWrite(ledPin, LOW); // turn the LED off by making the voltage LOW
delay(1 * 1000); // wait for a second

//PARTIE JSON
char payload[256]

StaticJasonDocument<256> jdoc;




}