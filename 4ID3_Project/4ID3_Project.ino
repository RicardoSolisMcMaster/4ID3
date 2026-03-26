#include "4ID3_Project.h"

const int Fan_Pin = 5;    
const int LED_Pin = 13;
const int Servo_Pin = 12;  

int ledsp = 100;

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";

  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (String(topic) == "4ID3_Group555/LED") {
    ledsp = msg.toInt();
    Serial.print("Updated LED threshold to: ");
    Serial.println(ledsp);
  }
}

void setup() {
  Serial.begin(115200);

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  pinMode(Fan_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);

  dht.begin();
  BH1750.begin(BH1750_TO_GROUND);

  myServo.attach(Servo_Pin);
  myServo.write(0);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(brokerAddress, addressPort);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("\nAttempting MQTT connection...");

    if (client.connect("ESP8266Client_Ricardo")) {
      Serial.println("Connected to MQTT server");
      client.subscribe("4ID3_Group555/LED");
      Serial.println("Subscribed to 4ID3_Group555/LED");
    } else {
      Serial.print("\nFailed to connect to MQTT server, rc = ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  sensors_event_t dhtTempEvent, dhtHumEvent, bmpEvent;
  dht.temperature().getEvent(&dhtTempEvent);
  dht.humidity().getEvent(&dhtHumEvent);

  BH1750.start();
  float lux = BH1750.getLux();

  Serial.println("\n-");
  (!isnan(dhtTempEvent.temperature)) ? Serial.println("Temperature: " + String(dhtTempEvent.temperature) + " degC") : Serial.println("Temperature Sensor Disconnected");

  if (!isnan(dhtTempEvent.temperature)) {
    if (dhtTempEvent.temperature >= 27) {
      digitalWrite(Fan_Pin, HIGH);
    } else if (dhtTempEvent.temperature <= 24) {
      digitalWrite(Fan_Pin, LOW);
    }
  }

  (!isnan(dhtHumEvent.relative_humidity)) ? Serial.println("Humidity: " + String(dhtHumEvent.relative_humidity) + " %") : Serial.println("Humidity Sensor Disconnected");
  (!isnan(lux)) ? Serial.println("Light Intensity: " + String(lux) + " lux") : Serial.println("Lux Sensor Disconnected");

  if (!isnan(lux)) {
    if (lux <= ledsp) {
      digitalWrite(LED_Pin, HIGH);
    } else {
      digitalWrite(LED_Pin, LOW);
    }
  }

  // Servo goes left if lux < temperature, otherwise right
  if (!isnan(lux) && !isnan(dhtTempEvent.temperature)) {
    if (lux < dhtTempEvent.temperature) {
      myServo.write(0);     // all the way left
      Serial.println("Servo -> LEFT");
    } else {
      myServo.write(180);   // all the way right
      Serial.println("Servo -> RIGHT");
    }
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  client.publish("4ID3_Group555/temperature", String(dhtTempEvent.temperature).c_str());
  delay(100);
  client.publish("4ID3_Group555/humidity", String(dhtHumEvent.relative_humidity).c_str());
  delay(100);
  client.publish("4ID3_Group555/light", String(lux).c_str());

  Serial.println("Published data.");
  Serial.print("Current LED threshold: ");
  Serial.println(ledsp);

  delay(100);
}