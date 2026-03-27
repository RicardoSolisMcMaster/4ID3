// DHT11 Libraries
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// I2C
#include <Wire.h>

// HP_BH1750 Libraries
#include <hp_BH1750.h>

// MQTT Libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Servo Library
#include <Servo.h>

// Macros
#define DHTPIN1 14
#define DHTPIN2 10
#define DHTTYPE DHT11
#define DELAY_BETWEEN_SAMPLES_MS 1000

// Global Variables
char* ssid = "iPhone";
char* pass = "teddy123";
const char* brokerAddress = "172.20.10.9";
uint16_t addressPort = 1883;

// Instantiate Sensor Objects
DHT_Unified dht1(DHTPIN1, DHTTYPE);
DHT_Unified dht2(DHTPIN2, DHTTYPE);

// Two light sensors
hp_BH1750 BH1750_1;   // Room 1
hp_BH1750 BH1750_2;   // Room 2

Servo myServo;

// Instantiate MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);