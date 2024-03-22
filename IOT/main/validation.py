import sys
import json


from jsonschema import validate
from jsonschema.exceptions import ValidationError

import serial

# Définir le schéma
schema = {
    "type": "object",
    "properties": {
        "status": {
            "type": "object",
            "properties": {
                "temperature": {"type": "number"},
                "light": {"type": "number"},
                "regul": {"type": "string"},
                "fire": {"type": "boolean"},
                "heat": {"type": "string"},
                "cold": {"type": "string"},
                "fanspeed": {"type": "number"}
            },
            "required": ["temperature", "light", "regul", "fire", "heat", "cold", "fanspeed"]
        },
        "location": {
            "type": "object",
            "properties": {
                "room": {"type": "string"},
                "gps": {
                    "type": "object",
                    "properties": {
                        "lat": {"type": "number"},
                        "lon": {"type": "number"}
                    },
                    "required": ["lat", "lon"]
                },
                "address": {"type": "string"}
            },
            "required": ["room", "gps", "address"]
        },
        "regul": {
            "type": "object",
            "properties": {
                "lt": {"type": "number"},
                "ht": {"type": "number"}
            },
            "required": ["lt", "ht"]
        },
        "info": {
            "type": "object",
            "properties": {
                "ident": {"type": "string"},
                "user": {"type": "string"},
                "loc": {"type": "string"}
            },
            "required": ["ident", "user", "loc"]
        },
        "net": {
            "type": "object",
            "properties": {
                "uptime": {"type": "string"},
                "ssid": {"type": "string"},
                "mac": {"type": "string"},
                "ip": {"type": "string"}
            },
            "required": ["uptime", "ssid", "mac", "ip"]
        },
        "reporthost": {
            "type": "object",
            "properties": {
                "target_ip": {"type": "string"},
                "target_port": {"type": "number"},
                "sp": {"type": "number"}
            },
            "required": ["target_ip", "target_port", "sp"]
        },
        "port": {"type": "string"},
        "_msgid": {"type": "string"}
    },
    "required": ["status", "location", "regul", "info", "net", "reporthost", "port", "_msgid"]
}

# Fonction pour validation JSON
def validate_json(json_data):
    try:
        validate(instance=json_data, schema=schema)
        print("Le JSON est valide par rapport au schéma.")
    except Exception as e:
        print("Le JSON n'est pas valide par rapport au schéma.")
        print(e)

# Lecture contenu du fichier JSON
def read_json_file(filename):
    with open(filename, 'r') as file:
        json_data = json.load(file)
    return json_data

# Chemin vers le fichier JSON à valider
json_file_path = sys.argv[1]

# Lire le fichier JSON
json_data = read_json_file(json_file_path)

# Appel de la fonction de validation
validate_json(json_data)
