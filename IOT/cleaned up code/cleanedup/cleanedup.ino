// Pin Definitions
const int tempSensorPin = 23;   // Temperature sensor pin
const int ledPinCooler = 19;    // Cooler LED pin (Green)
const int ledPinHeater = 21;    // Heater LED pin (Red)
const int fanPin = 27;          // Fan pin
const int pwmChannel = 0;       // PWM channel for fan control

// Thresholds
float upperThreshold = 25.0;    // Upper temperature threshold for cooling (SH)
float lowerThreshold = 20.0;    // Lower temperature threshold for heating (SB)

// Libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Objects initialization
OneWire oneWire(tempSensorPin);
DallasTemperature tempSensor(&oneWire);

// Setup function
void setup() {
  Serial.begin(9600);
  Serial.println("** Initialization **");

  // Temperature sensor setup
  tempSensor.begin();
  delay(2000); // Wait for sensor to initialize

  // LED pins setup
  pinMode(ledPinCooler, OUTPUT);
  pinMode(ledPinHeater, OUTPUT);

  // Fan setup
  pinMode(fanPin, OUTPUT);
  ledcAttachPin(fanPin, pwmChannel);
  ledcSetup(pwmChannel, 25000, 8);

  // Initially turn off LEDs and fan
  digitalWrite(ledPinCooler, LOW);
  digitalWrite(ledPinHeater, LOW);
  ledcWrite(pwmChannel, 0); // Turn off fan
}

// Loop function
void loop() {
  // Read temperature
  float temperature;
  tempSensor.requestTemperaturesByIndex(0);
  temperature = tempSensor.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Check if temperature exceeds thresholds
  if (temperature > upperThreshold) {
    // Cooling required
    digitalWrite(ledPinCooler, HIGH);  // Turn on cooler LED
    digitalWrite(ledPinHeater, LOW);   // Turn off heater LED

    // Fan speed adjustment (proportional control)
    int fanSpeed = map(temperature, upperThreshold, 30, 0, 255); // Adjust fan speed based on temperature
    ledcWrite(pwmChannel, fanSpeed); // Set fan speed
  } else if (temperature < lowerThreshold) {
    // Heating required
    digitalWrite(ledPinCooler, LOW);   // Turn off cooler LED
    digitalWrite(ledPinHeater, HIGH);  // Turn on heater LED
    ledcWrite(pwmChannel, 0); // Turn off fan
  } else {
    // Temperature within acceptable range
    digitalWrite(ledPinCooler, LOW);   // Turn off cooler LED
    digitalWrite(ledPinHeater, LOW);   // Turn off heater LED
    ledcWrite(pwmChannel, 0); // Turn off fan
  }

  delay(1000); // Delay for stability
}
