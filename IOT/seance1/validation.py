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

# Ouvrir le port série
ser = serial.Serial('COM4', 9600)

# Lire les données depuis le port série
data = ser.readline()

# Convertir les données en JSON
data_json = json.loads(data)

# Valider les données
try:
    validate(instance=data_json, schema=schema)
    print("JSON data is valid")
except ValidationError as e:
    print("JSON data is invalid: ", e.message)