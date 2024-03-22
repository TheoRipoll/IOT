
// ------------------------- FONCTIONS ----------------------------- //

// Initialiser les diodes LED pour le chauffage, la climatisation et le feu
void ledRegInit(){
  pinMode(ledCoolerPin, OUTPUT);
  pinMode(ledHeaterPin, OUTPUT);
 }


// Fonction pour récupérer la température
float temperature() {
  float t;
  tempSensor.requestTemperaturesByIndex(0);
  t = tempSensor.getTempCByIndex(0);
  return t;
}

// Fonction pour la climatisation
String* Climatiseur() {
  static String status[2] = {"OFF", "OFF"}; // Initialiser le statut de la climatisation et du chauffage
  // PARTIE CLIMATISATION
  float t;
  tempSensor.requestTemperaturesByIndex(0);
  t = tempSensor.getTempCByIndex(0);

  if (t > upperThreshold) { // Si la température est supérieure au seuil haut (Climatisation nécessaire)
    
    digitalWrite(ledCoolerPin, HIGH);  // Allumer la LED de climatisation
    digitalWrite(ledHeaterPin, LOW);   // Eteindre la LED de chauffage

    status[0] = "OFF"; // Mettre à jour le statut de la climatisation
    status[1] = "ON"; // Mettre à jour le statut du chauffage

    // Ajuster la vitesse du ventilateur en fonction de la température
    fanSpeed = map(t, upperThreshold, 30, 0, 255);
    ledcWrite(0, fanSpeed); // Ecrire la vitesse du ventilateur

    // Ruban de LED
    strip.begin();
    delay(1);

    for (int i = 0; i < NUMLEDS; i++) {
      // Changer la couleur en Rouge
      strip.setPixelColor(i, strip.Color(255, 5, 0));
    }
    strip.show();

  } else if (t < lowerThreshold) {
    // Si la température est inférieure au seuil bas (Chauffage nécessaire)
    digitalWrite(ledCoolerPin, LOW);   // Eteindre la LED de climatisation
    digitalWrite(ledHeaterPin, HIGH);  // Allumer la LED de chauffage
    ledcWrite(0, 0); // Eteindre le ventilateur

    status[0] = "ON"; // Mettre à jour le statut de la climatisation
    status[1] = "OFF"; // Mettre à jour le statut du chauffage

    // Ruban de LED
    strip.begin(); 
    delay(1);

    for (int i = 0; i < NUMLEDS; i++) {
      // Changer la couleur en Vert
      strip.setPixelColor(i, strip.Color(3, 255, 0));
    }
    strip.show();
  }

  else {
    // Si la température est comprise entre les seuils (Ni chauffage ni climatisation nécessaire)
    digitalWrite(ledCoolerPin, LOW);   // Eteindre la LED de climatisation
    digitalWrite(ledHeaterPin, LOW);   // Eteindre la LED de chauffage
    ledcWrite(0, 0); // Eteindre le ventilateur

    status[0] = "OFF"; // Mettre à jour le statut de la climatisation
    status[1] = "OFF"; // Mettre à jour le statut du chauffage

    // Ruban de LED
    strip.begin();
    delay(1);

    for (int i = 0; i < NUMLEDS; i++) {
      // Changer la couleur en Orange
      strip.setPixelColor(i, strip.Color(255, 124, 0));
    }
    strip.show();
    delay(1000);
  }

  return status; // Retourner le statut de la climatisation et du chauffage
}

// Fonction pour faire clignoter la LED de climatisation
void ledblink() {
  digitalWrite(ledHeaterPin, HIGH); // Allumer la LED en faisant
  delay(1 * 1000); // Attendre une seconde
  digitalWrite(ledHeaterPin, LOW); // Eteindre la LED
  delay(1 * 1000); // Attendre une seconde
}
