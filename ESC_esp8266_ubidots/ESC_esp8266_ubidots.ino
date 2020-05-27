#include <Servo.h>  // Include the Servo library
#include "UbidotsESPMQTT.h"
#include <ESP8266WiFi.h>

#define TOKEN "*******" // Your Ubidots TOKEN
#define wifi_ssid "********" // put your wifi ID
#define wifi_password "********" // put your wifi Password
#define DEVICE_LABEL  "motor"  // Put here your Ubidots device label
#define MQTTCLIENTNAME "qwertyuiop" // random ASCII to define unique name
#define MQTTCLIENTNAME2 "qwertyuiop2" // random ASCII to define unique name
#define VARIABLE  "speed"  // Put here your Ubidots variable label
#define VARIABLE2  "speedbackward"// Put here your Ubidots variable label
#define MotorPin 14 // Declare the ESC pin D5

WiFiServer server(80);
Servo ESC; // Create a ESC object
float MotorSpeed=0;
float value=0;
Ubidots client(TOKEN,MQTTCLIENTNAME);
Ubidots client2(TOKEN,MQTTCLIENTNAME2);

float btof(byte * payload, unsigned int length) {
  char * demo = (char *) malloc(sizeof(char) * 10);
  for (int i = 0; i < length; i++) {
    demo[i] = payload[i];
  }
  float value = atof(demo);
  free(demo);
  return value;
}

// Callback to handle subscription
void callback(char* topic, byte* payload, unsigned int length) {
    value = btof(payload, length);
      value = map(value, 0, 100, 96, 70); //Map the 0-100 values from the slider to the 0-180 to use the servo lib.
      ESC.write(value); //Send the value (PWM) to the ESC
     // Servo1.write(value);
}

void callback2(char* topic, byte* payload, unsigned int length) {
    value = btof(payload, length);
      value = map(value, 0, 10, 96, 120); //Map the 0-100 values from the slider to the 0-180 to use the servo lib.
      ESC.write(value); //Send the value (PWM) to the ESC
     // Servo1.write(value);
}



void setup() {

  client.ubidotsSetBroker("industrial.api.ubidots.com"); // Sets the broker 
  client2.ubidotsSetBroker("industrial.api.ubidots.com");// Sets the broker 
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client2.setDebug(true);
  
Serial.begin(115200);

 // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
 
  client.wifiConnection(wifi_ssid, wifi_password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

   client.begin(callback);
   client2.begin(callback2);
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE); //Insert the dataSource and Variable's Labels
  client2.ubidotsSubscribe(DEVICE_LABEL, VARIABLE2);
  ESC.attach(MotorPin);// We need to attach the ESC to the used pin number

}

void loop(){
  
// Check if a client has connected
  if (!client.connected()) {
    client.reconnect();
    client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE); //Insert the dataSource and Variable's Labels 
    client2.ubidotsSubscribe(DEVICE_LABEL, VARIABLE2); //Insert the dataSource and Variable's Labels
  }
  client.loop();
  client2.loop();

  
}
