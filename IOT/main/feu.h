
int numberKeyPresses = 0;
int fanSpeed = 0; // Initialiser la vitesse du ventilateur
const int ledFirePin = 2; // Broche D2 sur la carte

void IRAM_ATTR isr();

void ledFeuInit();
void initVent();

void PWMBlink();
boolean readFire();
int readLight();
int readFanSpeed();
