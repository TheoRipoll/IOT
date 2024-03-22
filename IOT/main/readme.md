# Projet ESP Dashboard 

## Partie 1: Codage de l'ESP

## Configuration:
L'ESP est branché en suivant ces pins: 
- GPIO 19 => Cooler (LED)
- GPIO 21 => Heater (LED)
- GPIO 23 => Temperature sensor
- GPIO 27 => PWM (Fan or LED)
- GPIO 33/ ADC1_CH5 => Light/Luminosity sensor
- Pin 2 =>  Fire Led
- GPIO 13  => Led Strip (5 Leds)

## Contrôle de climatisation et Alerte incendie

Ce projet a un système de contrôle de climatisation basé sur ESP32. 
Il utilise un capteur de température DallasTemperature pour surveiller la température ambiante et contrôler un système de chauffage et de refroidissement en conséquence. 
Le système surveille également la lumière ambiante et le feu. Si un feu est détecté, une LED de feu s'allume.
### Fonctionnement

Le système surveille la température ambiante et active le système de chauffage ou de refroidissement en conséquence. Si la température dépasse un seuil supérieur, le système de refroidissement est activé. Si elle tombe en dessous d'un seuil inférieur, le système de chauffage est activé. 
Si la température est entre les deux seuils, les deux systèmes sont désactivés.

Le système surveille également la lumière ambiante et le feu. Si un feu est détecté, une LED de feu s'allume.

Pour rentre le code plus propre, chaque lecture de valeur est une fonction appelée dans le void loop quand necessaire. 
### Envoi de la donnée 

Les informations sur l'état du système sont envoyées via le port série sous forme de JSON. Cela comprend la température, la lumière, l'état du feu, l'état du chauffage et du refroidissement, la vitesse du ventilateur, et d'autres informations. 

Les informations qui ne sont pas encore connues, tel que le reseau, son marquées "A definir" ou NULL pour l'instant.
### Bibliothèques utilisées

- ArduinoJson : pour la création et la manipulation de JSON.
- OneWire : pour la communication avec le capteur de température.
- DallasTemperature : pour la lecture du capteur de température.
- Adafruit_NeoPixel : pour contrôler la bande de LED.

## Partie 2: Validation

Un script python permet de vérifier si le fichier JSON à analyser est conforme pour être traiter.

Via le cmd, tapez :
python [chemin vers le script python] [chemin vers le fichier JSON à valider]

### Bibliothèques requis
- jsonschema : pour valider un fichier JSON par rapport à un schéma JSON
- serial : pour lire le contenu d'un fichier JSON

## Partie 3: Dashboard 

### Description
Ce dashboard permet de suivre les différentes valeurs envoyées par l'ESP

### Structure
- Le premier groupe contient les valeurs brutes sous forme de texte 
- Le second groupe contient les valeurs liés a la climatisation 
- Le troisième groupe contient les informations liés a l'identification, l'utilisateur et la localisation
### Fonctionnement

Le flux recois le JSON envoyé par l'esp. Les valeurs sont ensuites extraites en utilisant des fonctions pour être affichées avec des nœuds UI.

Le flux permet aussi d'envoyer des commandes a l'ESP grace au Serial out. Deux sliders ont été ajoutés pour ajuster les seuils de la climatisation mais malheuresement nous n'avons pas reussi a faire fonctionner l'écoute sur la serie du coté de l'ESP. 

