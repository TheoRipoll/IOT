const int ledCoolerPin = 19; // ledPin refers to ESP32 GPIO 19 : Broche D19 sur la carte
const int ledHeaterPin = 21;

const int ledFirePin = 2;

int fanSpeed = 0;

//Setup du json output
#ifndef MAKEJSONH
#define MAKEJSONH
#include <ArduinoJson.h>
#endif

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
  pinMode(ledFirePin, OUTPUT);

  //PWM
  pinMode(26, INPUT_PULLUP); // broche 26
  attachInterrupt(26, isr, FALLING); // handler

  ledcAttachPin(27, 0); // broche 27, canal 0.
  ledcSetup(0, 25000, 8);

  ledcWrite(0, 255);

  //LED STRIP
  strip.begin();
  delay(1);
}


// Loop function runs over and over again forever
void loop() {
  //PARTIE JSON
  char payload[512];
  StaticJsonDocument<512> doc;
  String* status = Climatiseur(); // Assuming Climatiseur() is defined somewhere else in your code


  doc["status"]["temperature"] = temperature(); // Replace with your function to read temperature
  doc["status"]["light"] = readLight(); // Replace with your function to read light
  doc["status"]["regul"] = "HALT";
  doc["status"]["fire"] = readFire(); // Replace with your function to read fire status
  doc["status"]["heat"] = status[0];
  doc["status"]["cold"] = status[1];
  doc["status"]["fanspeed"] = readFanSpeed(); // Replace with your function to read fan speed

  // Fill in the location
  doc["location"]["room"] = "312";
  doc["location"]["gps"]["lat"] = 43.62453842;
  doc["location"]["gps"]["lon"] = 7.050628185;
  doc["location"]["address"] = "Les lucioles";

  // Fill in the regul
  doc["regul"]["lt"] = upperThreshold;
  doc["regul"]["ht"] = lowerThreshold;

  // Fill in the info
  doc["info"]["ident"] = "Com4 esp";
  doc["info"]["user"] = "CB";
  doc["info"]["loc"] = "A Biot";

  // Fill in the net
  doc["net"]["uptime"] = null ;
  doc["net"]["ssid"] = "A definir";
  doc["net"]["mac"] =  "A definir";
  doc["net"]["ip"] =  "A definir";

  doc["reporthost"]["target_ip"] = "A definir";
  doc["reporthost"]["target_port"] = null ;
  doc["reporthost"]["sp"] = null ;

  // Serialize JSON document
  String json;
  serializeJson(doc, payload);

  // Print JSON to Serial
  Serial.println(payload);
  delay(20000);
}

void PWMBlink() {
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
}

float temperature() {
  float t;
  tempSensor.requestTemperaturesByIndex(0);
  t = tempSensor.getTempCByIndex(0);
  return t;
}

String* Climatiseur() {
  static String status[2] = {"OFF", "OFF"}; // Initialize status array
  // PARTIE CLIMATISATION
  float t;
  tempSensor.requestTemperaturesByIndex(0);
  t = tempSensor.getTempCByIndex(0);

  if (t > upperThreshold) {
    // Cooling required
    digitalWrite(ledCoolerPin, HIGH);  // Turn on cooler LED
    digitalWrite(ledHeaterPin, LOW);   // Turn off heater LED

    status[0] = "OFF";
    status[1] = "ON";

    // Fan speed adjustment (proportional control)
    fanSpeed = map(t, upperThreshold, 30, 0, 255); // Adjust fan speed based on temperature
    ledcWrite(0, fanSpeed); // Set fan speed

    strip.begin();
    delay(1);

    for (int i = 0; i < NUMLEDS; i++) {
      //turn color to red
      strip.setPixelColor(i, strip.Color(255, 5, 0));


    }
    strip.show();


  } else if (t < lowerThreshold) {
    // Heating required
    digitalWrite(ledCoolerPin, LOW);   // Turn off cooler LED
    digitalWrite(ledHeaterPin, HIGH);  // Turn on heater LED
    ledcWrite(0, 0); // Turn off fan

    status[0] = "ON";
    status[1] = "OFF";

    strip.begin();
    delay(1);

    for (int i = 0; i < NUMLEDS; i++) {
      //turn color to red
      strip.setPixelColor(i, strip.Color(3, 255, 0));
    }
    strip.show();
  }

  else {
    // Temperature within acceptable range
    digitalWrite(ledCoolerPin, LOW);   // Turn off cooler LED
    digitalWrite(ledHeaterPin, LOW);   // Turn off heater LED
    ledcWrite(0, 0); // Turn off fan

    status[0] = "OFF";
    status[1] = "OFF";

    strip.begin();
    delay(1);

    for (int i = 0; i < NUMLEDS; i++) {
      //turn color to red
      strip.setPixelColor(i, strip.Color(255, 124, 0));
    }
    strip.show();
    delay(1000);
  }

  return status;
}

boolean readFire() {
  int sensorValue;
  sensorValue = analogRead(A5);
  if (sensorValue > 800) {
    return true;
  } else {
    return false;
  }
}

void ledblink() {
  // Led blink //
  digitalWrite(ledHeaterPin, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(1 * 1000); // wait for a second
  digitalWrite(ledHeaterPin, LOW); // turn the LED off by making the voltage LOW
  delay(1 * 1000); // wait for a second

}

int readLight() {
  int sensorValue;
  sensorValue = analogRead(A5);
  return sensorValue;
}

int readFanSpeed() {
  return (numberKeyPresses / 10.0) * 30;
}