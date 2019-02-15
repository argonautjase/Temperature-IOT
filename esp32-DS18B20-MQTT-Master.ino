/*
 * Jason wilkins
 * www.jasonwilkins.co.uk
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2
#define ONE_WIRE_BUS 15 
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);  
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire); 
// Address of onewiresensor
DeviceAddress sensor1 = { 0x28, 0xFF, 0x41, 0x5, 0x64, 0x14, 0x2, 0xBD };

//Network setup
#define HOSTNAME "temp-sense"
const char* ssid     = "YOURSSIDHERE";
const char* password = "YOURWIFIPASSWORDHERE";
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT setup
const char* mqtt_server = "YOURMQTTBROKERIPHERE";
const PROGMEM char *environmentTopic = "RL/esp32-1/temp1/environment";

long lastMsg = 0;
char msg[50];
int value = 0;
float temp = 0;

void setup() {
  
  //Initialize serial port monitor
  Serial.begin(115200);
  
  // initialise sensor
  sensors.begin();

  // setup wifi
  setup_wifi();

  // setup MQTT
  client.setServer(mqtt_server, 1883);
  
  }

void setup_wifi() {
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //Set Hostname
//  String hostname ("temp-sense-1");
//  WiFi.hostname(hostname);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


// Connect to MQTT server
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(HOSTNAME)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

  
void loop() {

if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    sensors.setResolution(12);
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0);
    
            
    
   client.publish(environmentTopic, String(temp).c_str());


  }
}



 
