

// ------------------------- FONCTIONS ----------------------------- //

void IRAM_ATTR isr() {
  numberKeyPresses++;
}

 void ledFeuInit(){
// Initialiser les diodes LED pour le feu
  pinMode(ledFirePin, OUTPUT);
 }

// Initialiser le Ventilateur
 void initVent(){
  pinMode(26, INPUT_PULLUP); // broche 26
  attachInterrupt(26, isr, FALLING);

  ledcAttachPin(27, 0); // broche 27 et canal 0
  ledcSetup(0, 25000, 8);

  ledcWrite(0, 255);
  }
  
// Fonction pour faire clignoter le PWM
void PWMBlink() {
  int nb_state = 12;
  int rcv[nb_state] = {0, 64, 64,
                       0, 127, 127,
                       0, 191, 191,
                       0, 255, 255
                      }; // Rapports cycliques si 8 bits
  // 255 => 100%, 127 => 50% et 0 => 0%
  static uint32_t tick = 0; // temps de départ
  static int elapsed; // temps écoulé

  static int current = 0; // etat courant

  elapsed = millis() - tick;
  if (elapsed > 10000) { // Toutes les 10 sec on change de Rapport Cyc

    Serial.printf("Fan speed = %f RPM\n", (numberKeyPresses / 10.0) * 30); // 30 = 60/2 (2 pales par tour)
    numberKeyPresses = 0; // On réinitialise le compteur

    ledcWrite(0, rcv[current]); // canal = 0, rapport cyclique
    Serial.printf("Rapport Cyc = %lf %%\n", (rcv[current] / 255.0) * 100);

    current += 1; // Prochain rapport cyclique
    current %= nb_state; // On revient à 0 si on a dépassé le dernier état

    tick = millis(); // On réinitialise le temps de départ
  }
}

// Fonction pour lire la présence de feu
boolean readFire() {
  int sensorValue;
  sensorValue = analogRead(A5);
  if (sensorValue > 800) {
    return true;
  } else {
    return false;
  }
}

// Fonction pour renvoyer la valeur de la lumière
int readLight() {
  int sensorValue;
  sensorValue = analogRead(A5);
  return sensorValue;
}

// Fonction pour lire la vitesse du ventilateur
int readFanSpeed() {
  return (numberKeyPresses / 10.0) * 30;
}
