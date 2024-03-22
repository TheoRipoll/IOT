#include "feu.h"
#include "json.h"
#include "regulation.h"

void setup() {
  Serial.begin(9600); 
  Serial.println("** This message will only be displayed on start or reset. **");
  tempSensor.begin();
  delay(2 * 1000); // Délai pour laisser le temps au capteur de température de s'initialiser

  // Initialisation LED et Ventilateur
  ledFeuInit();
  ledRegInit();
  initVent();

  // Initialiser le ruban de LED
  strip.begin();
  delay(1);
}

void loop() {
    delay(100); 
    exportDataJson();
}
