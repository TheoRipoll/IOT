//Mise en place sortie JSON
#ifndef MAKEJSONH //
#define MAKEJSONH
#include <ArduinoJson.h> 
#endif

void exportDataJson(){
  delay(100);
  char payload[512];
  StaticJsonDocument<512> doc; // Création d'un document JSON
  String* status = Climatiseur();

  doc["status"]["temperature"] = temperature(); //Récupération de la température pour l'envoyer dans le JSON
  doc["status"]["light"] = readLight(); //Récupération de la lumière pour l'envoyer dans le JSON
  doc["status"]["regul"] = "HALT"; //Récupération de la régulation pour l'envoyer dans le JSON
  doc["status"]["fire"] = readFire(); //Récupération de la présence de feu pour l'envoyer dans le JSON
  doc["status"]["heat"] = status[0]; //Récupération du statut de la climatisation pour l'envoyer dans le JSON
  doc["status"]["cold"] = status[1]; //Récupération du statut du chauffage pour l'envoyer dans le JSON
  doc["status"]["fanspeed"] = readFanSpeed(); //Récupération de la vitesse du ventilateur pour l'envoyer dans le JSON

  // Récupération des informations sur la localisation pour l'envoyer dans le JSON
  doc["location"]["room"] = "312";
  doc["location"]["gps"]["lat"] = 43.62453842;
  doc["location"]["gps"]["lon"] = 7.050628185;
  doc["location"]["address"] = "Les lucioles";

  // Récupération des informations sur la régulation pour l'envoyer dans le JSON
  doc["regul"]["lt"] = lowerThreshold;
  doc["regul"]["ht"] = upperThreshold;

  // Récupération d'informations lié à l'ESP pour l'envoyer dans le JSON
  doc["info"]["ident"] = "Com4 esp";
  doc["info"]["user"] = "CB";
  doc["info"]["loc"] = "A Biot";

  // Récupération d'informations sur le réseau pour l'envoyer dans le JSON
  doc["net"]["uptime"] = NULL ;
  doc["net"]["ssid"] = "A definir";
  doc["net"]["mac"] =  "A definir";
  doc["net"]["ip"] =  "A definir";

  // Récupération d'informations sur le serveur pour l'envoyer dans le JSON
  doc["reporthost"]["target_ip"] = "A definir";
  doc["reporthost"]["target_port"] = NULL ;
  doc["reporthost"]["sp"] = NULL ;

  // Sérialisation du JSON
  String json;
  serializeJson(doc, payload);

  // Affichage du JSON dans le moniteur série
  Serial.println(payload);
  delay(200);
  }

  
