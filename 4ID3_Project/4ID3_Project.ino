#include "4ID3_Project.h"

const int LED1_Pin = 13;
const int LED2_Pin = 15;
const int Servo_Pin = 12;

int ledsp1 = 100;         // room 1 threshold
int ledsp2 = 100;         // room 2 threshold
int tempsp = 25;

int ledPWM1 = 0;
int ledPWM2 = 0;

String mode = "AUTO";

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";

  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  msg.trim();

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (String(topic) == "4ID3_Group555/TEMP") {
    tempsp = msg.toInt();
  }

  if (String(topic) == "4ID3_Group555/room1/LED_THRESHOLD") {
    ledsp1 = msg.toInt();
  }

  if (String(topic) == "4ID3_Group555/room2/LED_THRESHOLD") {
    ledsp2 = msg.toInt();
  }

  if (String(topic) == "4ID3_Group555/room1/LED_PWM") {
    ledPWM1 = constrain(msg.toInt(), 0, 1023);
  }

  if (String(topic) == "4ID3_Group555/room2/LED_PWM") {
    ledPWM2 = constrain(msg.toInt(), 0, 1023);
  }

  if (String(topic) == "4ID3_Group555/MODE") {
    String lowerMsg = msg;
    lowerMsg.toLowerCase();

    if (lowerMsg == "auto" || lowerMsg == "0" || lowerMsg == "false") {
      mode = "AUTO";
    }
    else if (lowerMsg == "manual" || lowerMsg == "1" || lowerMsg == "true") {
      mode = "MANUAL";
    }

    Serial.print("Mode set to: ");
    Serial.println(mode);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1_Pin, OUTPUT);
  pinMode(LED2_Pin, OUTPUT);

  analogWrite(LED1_Pin, 0);
  analogWrite(LED2_Pin, 0);

  dht1.begin();
  dht2.begin();

  // Correct I2C pins
  Wire.begin(4, 5);
  delay(100);

  // Init BH1750 sensors
  bool bh1_ok = BH1750_1.begin(BH1750_TO_GROUND);
  bool bh2_ok = BH1750_2.begin(BH1750_TO_VCC);

  Serial.print("BH1750 Room1: ");
  Serial.println(bh1_ok ? "OK" : "FAIL");

  Serial.print("BH1750 Room2: ");
  Serial.println(bh2_ok ? "OK" : "FAIL");

  myServo.attach(Servo_Pin);
  myServo.write(90);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  client.setServer(brokerAddress, addressPort);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");

    if (client.connect("ESP8266Client_Ricardo")) {
      Serial.println("Connected");

      client.subscribe("4ID3_Group555/TEMP");
      client.subscribe("4ID3_Group555/MODE");

      client.subscribe("4ID3_Group555/room1/LED_THRESHOLD");
      client.subscribe("4ID3_Group555/room2/LED_THRESHOLD");

      client.subscribe("4ID3_Group555/room1/LED_PWM");
      client.subscribe("4ID3_Group555/room2/LED_PWM");
    } else {
      Serial.print("Failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  sensors_event_t dht1TempEvent, dht1HumEvent;
  sensors_event_t dht2TempEvent, dht2HumEvent;

  dht1.temperature().getEvent(&dht1TempEvent);
  dht1.humidity().getEvent(&dht1HumEvent);

  dht2.temperature().getEvent(&dht2TempEvent);
  dht2.humidity().getEvent(&dht2HumEvent);

  // 🔥 FIX: proper BH1750 reading
  BH1750_1.start();
  delay(10);
  float lux1 = BH1750_1.getLux();

  BH1750_2.start();
  delay(10);
  float lux2 = BH1750_2.getLux();

  Serial.println("\n--------------------");

  Serial.println("ROOM 1:");
  Serial.println("Temp: " + String(dht1TempEvent.temperature));
  Serial.println("Humidity: " + String(dht1HumEvent.relative_humidity));
  Serial.println("Light: " + String(lux1));

  Serial.println("ROOM 2:");
  Serial.println("Temp: " + String(dht2TempEvent.temperature));
  Serial.println("Humidity: " + String(dht2HumEvent.relative_humidity));
  Serial.println("Light: " + String(lux2));

  Serial.println("Mode: " + mode);

  // LED CONTROL
  if (!isnan(lux1)) {
    if (mode == "AUTO") {
      analogWrite(LED1_Pin, (lux1 <= ledsp1) ? 1023 : 0);
    } else {
      analogWrite(LED1_Pin, ledPWM1);
    }
  }

  if (!isnan(lux2)) {
    if (mode == "AUTO") {
      analogWrite(LED2_Pin, (lux2 <= ledsp2) ? 1023 : 0);
    } else {
      analogWrite(LED2_Pin, ledPWM2);
    }
  }

  // SERVO CONTROL
  if (!isnan(dht1TempEvent.temperature) && !isnan(dht2TempEvent.temperature)) {

    if (dht1TempEvent.temperature > tempsp && dht2TempEvent.temperature < tempsp) {
      myServo.write(0);
      Serial.println("Servo LEFT");
    }
    else if (dht2TempEvent.temperature > tempsp && dht1TempEvent.temperature < tempsp) {
      myServo.write(180);
      Serial.println("Servo RIGHT");
    }
    else {
      myServo.write(90);
      Serial.println("Servo CENTER");
    }
  }

  // MQTT publish
  client.publish("4ID3_Group555/room1/temperature", String(dht1TempEvent.temperature).c_str());
  delay(50);
  client.publish("4ID3_Group555/room1/humidity", String(dht1HumEvent.relative_humidity).c_str());
  delay(50);
  client.publish("4ID3_Group555/room1/light", String(lux1).c_str());
  delay(50);

  client.publish("4ID3_Group555/room2/temperature", String(dht2TempEvent.temperature).c_str());
  delay(50);
  client.publish("4ID3_Group555/room2/humidity", String(dht2HumEvent.relative_humidity).c_str());
  delay(50);
  client.publish("4ID3_Group555/room2/light", String(lux2).c_str());

  delay(DELAY_BETWEEN_SAMPLES_MS);
}