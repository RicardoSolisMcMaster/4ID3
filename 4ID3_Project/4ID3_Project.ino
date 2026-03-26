#include "4ID3_Project.h"

const int LED1_Pin = 13;
const int LED2_Pin = 15;
const int Servo_Pin = 12;

int ledsp = 100;
int tempsp = 25;

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
  if (String(topic) == "4ID3_Group555/TEMP") {
    tempsp = msg.toInt();
    Serial.print("Updated temp threshold to: ");
    Serial.println(tempsp);
  }
}

void setup() {
  Serial.begin(115200);

  sensor_t sensor;
  dht1.temperature().getSensor(&sensor);
  dht1.humidity().getSensor(&sensor);
  dht2.temperature().getSensor(&sensor);
  dht2.humidity().getSensor(&sensor);

  pinMode(LED1_Pin, OUTPUT);
  pinMode(LED2_Pin, OUTPUT);

  dht1.begin();
  dht2.begin();
  BH1750.begin(BH1750_TO_GROUND);

  myServo.attach(Servo_Pin);
  myServo.write(90);   // center at startup

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
      client.subscribe("4ID3_Group555/TEMP");
    } else {
      Serial.print("\nFailed to connect to MQTT server, rc = ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  sensors_event_t dht1TempEvent, dht1HumEvent;
  sensors_event_t dht2TempEvent, dht2HumEvent;

  dht1.temperature().getEvent(&dht1TempEvent);
  dht1.humidity().getEvent(&dht1HumEvent);

  dht2.temperature().getEvent(&dht2TempEvent);
  dht2.humidity().getEvent(&dht2HumEvent);

  BH1750.start();
  float lux = BH1750.getLux();

  Serial.println("\n--------------------");

  (!isnan(dht1TempEvent.temperature)) ? Serial.println("Room 1 Temperature: " + String(dht1TempEvent.temperature) + " degC") : Serial.println("Room 1 Temperature Sensor Disconnected");
  (!isnan(dht1HumEvent.relative_humidity)) ? Serial.println("Room 1 Humidity: " + String(dht1HumEvent.relative_humidity) + " %") : Serial.println("Room 1 Humidity Sensor Disconnected");

  (!isnan(dht2TempEvent.temperature)) ? Serial.println("Room 2 Temperature: " + String(dht2TempEvent.temperature) + " degC") : Serial.println("Room 2 Temperature Sensor Disconnected");
  (!isnan(dht2HumEvent.relative_humidity)) ? Serial.println("Room 2 Humidity: " + String(dht2HumEvent.relative_humidity) + " %") : Serial.println("Room 2 Humidity Sensor Disconnected");

  (!isnan(lux)) ? Serial.println("Light Intensity: " + String(lux) + " lux") : Serial.println("Lux Sensor Disconnected");

  // LED control based on lux and Node-RED threshold
  if (!isnan(lux)) {
    if (lux <= ledsp) {
      digitalWrite(LED1_Pin, HIGH);
    } else {
      digitalWrite(LED1_Pin, LOW);
    }
  }
   if (!isnan(lux)) {
    if (lux <= ledsp) {
      digitalWrite(LED2_Pin, HIGH);
    } else {
      digitalWrite(LED2_Pin, LOW);
    }
  }

  // Servo control:
  // left if temp1 > temp2
  // right if temp2 > temp1
  // center if equal
  if (!isnan(dht1TempEvent.temperature) && !isnan(dht2TempEvent.temperature)) {
    if (dht1TempEvent.temperature > tempsp && dht2TempEvent.temperature < tempsp) {
      myServo.write(0);
      Serial.println("Servo -> LEFT (Room 1 hotter than Room 2)");
    } else if (dht2TempEvent.temperature > tempsp && dht1TempEvent.temperature < tempsp) {
      myServo.write(180);
      Serial.println("Servo -> RIGHT (Room 2 hotter than Room 1)");
    } else {
      myServo.write(90);
      Serial.println("Servo -> CENTER (Room temperatures equal)");
    }
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publish to MQTT
  client.publish("4ID3_Group555/room1/temperature", String(dht1TempEvent.temperature).c_str());
  delay(50);
  client.publish("4ID3_Group555/room1/humidity", String(dht1HumEvent.relative_humidity).c_str());
  delay(50);

  client.publish("4ID3_Group555/room2/temperature", String(dht2TempEvent.temperature).c_str());
  delay(50);
  client.publish("4ID3_Group555/room2/humidity", String(dht2HumEvent.relative_humidity).c_str());
  delay(50);

  client.publish("4ID3_Group555/light", String(lux).c_str());

  Serial.println("Published data.");
  Serial.print("Current LED threshold: ");
  Serial.println(ledsp);
  Serial.print("Current TEMP threshold: ");
  Serial.println(tempsp);

  delay(DELAY_BETWEEN_SAMPLES_MS);
}