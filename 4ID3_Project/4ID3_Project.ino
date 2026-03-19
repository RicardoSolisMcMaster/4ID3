#include "4ID3_Project.h"

const int Fan_Pin = 12;
const int LED_Pin = 13;
int ledsp;

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
  //Start the serial monitor at 115200 baud
  Serial.begin(115200); 


  //Create a sensor object that is passed into the getSensor method of the dht class
  //Only the dht sensor requires this
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  pinMode(Fan_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  //Run the begin()method on each sensor to start communication
  dht.begin();
  
  BH1750.begin(BH1750_TO_GROUND);   

  //Start the WiFi driver and tell it to connect to your local network
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  //While it is connecting, print a '.' to the serial monitor every 500 ms
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Once connected, print the local IP address
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //Set the MQTT client to connect to the desired broker
  client.setServer(brokerAddress, 1883);
  client.setCallback(callback);
}

void reconnect() {

  //While the client remains unconnected from the MQTT broker, attempt to reconnect every 2 seconds
  //Also, print diagnostic information
  while (!client.connected()) {
    Serial.print("\nAttempting MQTT connection...");
  
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT server");  
      client.subscribe("4ID3_Group555/LED");
    } else {
      Serial.print("\nFailed to connect to MQTT server, rc = ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
void loop() {
  
   //Polling the DHT and BMP sensor using events
   sensors_event_t dhtTempEvent, dhtHumEvent, bmpEvent;
   dht.temperature().getEvent(&dhtTempEvent);
   dht.humidity().getEvent(&dhtHumEvent);
   
   //Polling the BH sensor
   BH1750.start();
   float lux=BH1750.getLux(); 

   //Printing sensor readings to serial monitor
   Serial.println("\n-");
   (!isnan(dhtTempEvent.temperature)) ? Serial.println("Temperature: " + String(dhtTempEvent.temperature) + " degC") : Serial.println("Temperature Sensor Disconnected");
    if(dhtTempEvent.temperature >= 27){
      digitalWrite(Fan_Pin,HIGH);
    } else if ((dhtTempEvent.temperature <= 24)){
      digitalWrite(Fan_Pin,LOW);
    }
   (!isnan(dhtHumEvent.relative_humidity)) ? Serial.println("Humidity: " + String(dhtHumEvent.relative_humidity) + " %") : Serial.println("Humidity Sensor Disconnected");


   (!isnan(lux)) ? Serial.println("Light Intensity: " + String(lux) + " lux") : Serial.println("Lux Sensor Disconnected");
    if(lux <= ledsp){
      digitalWrite(LED_Pin,HIGH);
    } else if (lux>= ledsp){
      digitalWrite(LED_Pin,LOW);
    }
  client.loop();
  //If the client disconnects from the MQTT broker, attempt to reconnect
  if (!client.connected()) {
    
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
  
  //Publish the sensor data to the associated topics
  client.publish("4ID3_Group555/temperature", String(dhtTempEvent.temperature).c_str());
  delay(100);
  client.publish("4ID3_Group555/humidity", String(dhtHumEvent.relative_humidity).c_str());
  delay(100);
 
  client.publish("4ID3_Group555/light", String(lux).c_str());
  Serial.println("Published data.");
  Serial.println(ledsp);  
   delay(DELAY_BETWEEN_SAMPLES_MS);
}
