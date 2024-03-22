#include "OneWire.h"
#include "DallasTemperature.h"
#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMLEDS 5

OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

const int ledCoolerPin = 19; // Broche D19 sur la carte
const int ledHeaterPin = 21; // Broche D21 sur la carte

float upperThreshold = 25.0;    // Seuil_haute pour la climatisation => En cas de haute température (SH)
float lowerThreshold = 20.0;    // Seuil_basse pour le chauffage => En cas de basse température (SB)

Adafruit_NeoPixel strip(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);
// ------------------------- FONCTIONS ----------------------------- //

void ledRegInit();
float temperature();
String* Climatiseur();
void ledblink();

// ----------------------------------------------------------------- //
