//DHT11 Libraries
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//HP_BH1750 Libraries
#include <hp_BH1750.h> 

//MQTT Libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Servo Library
#include <Servo.h>

//Macros
#define DHTPIN 14
#define DHTTYPE DHT11
#define DELAY_BETWEEN_SAMPLES_MS 5000

//Global Variables
char* ssid = "iPhone";
char* pass = "teddy123";
const char* brokerAddress = "172.20.10.9";   // keep your hotspot laptop IP here
uint16_t addressPort = 1883;

//Instantiate Sensor Objects
DHT_Unified dht(DHTPIN, DHTTYPE);
hp_BH1750 BH1750;
Servo myServo;

//Instantiate MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);