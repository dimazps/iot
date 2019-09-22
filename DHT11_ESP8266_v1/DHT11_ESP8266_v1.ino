#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>

#include <Adafruit_ADS1015.h>

#include "DHT.h"

#define DHTPIN 2 // what digital pin we're connected to NodeMCU (D4)

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11


//Adafruit_ADS1115 ads(0x48);

DHT dht(DHTPIN, DHTTYPE);

#define wifi_ssid "wifiname"
#define wifi_password "********"

#define mqtt_server "192.168.1.*"
#define mqtt_user "username"
#define mqtt_password "********"

#define humidity_topic "testing_humidity"
#define temperature_celsius_topic "testing_temperature_celsius"
#define temperature_fahrenheit_topic "testing_temperature_fahrenheit"

//#define ph_topic "randobawa_ph"

#define moist_topic "testing_moist"

//#define photo_topic "randobawa_photo"

WiFiClient espClient;
PubSubClient client(espClient);

//int pHSense = A0;

int moistSensor = A0;

void setup() {
  
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
//  ads.begin();
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
    
          // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;  
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      clientName += "-";
      clientName += String(micros() & 0xff, 16);
      Serial.print("Connecting to ");
      Serial.print(mqtt_server);
      Serial.print(" as ");
      Serial.println(clientName);


    // Attempt to connect
    // If you do not want to use a username and password, change next line to
  // if (client.connect((char*) clientName.c_str())) {
   if (client.connect(((char*) clientName.c_str()), mqtt_user, mqtt_password)) {
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

      // Wait a few seconds between measurements.
      delay(15000);

      const unsigned long fiveMinutes = 30 * 60 * 1000UL;
      static unsigned long lastSampleTime = 0 - fiveMinutes;  // initialize such that a reading is due the first time through loop()

    //  int x,y,z,w;
    //  int16_t adc0, adc1, adc2, adc3; // initialize pin ADC from ads1115
    
      unsigned long now = millis();
      if (now - lastSampleTime >= fiveMinutes)
      {
        lastSampleTime += fiveMinutes;
        float h = dht.readHumidity();
        // Read temperature as Celsius (the default)
        float t = dht.readTemperature();
        // Read temperature as Fahrenheit (isFahrenheit = true)
        float f = dht.readTemperature(true);
        // Read Analog from Moisture Sensor
        int moistVal = analogRead(moistSensor);

       //  adc0 = ads.readADC_SingleEnded(0); 
       // adc1 = ads.readADC_SingleEnded(1); 
      //  adc2 = ads.readADC_SingleEnded(2); 
      //  adc3 = ads.readADC_SingleEnded(3); 
        
      //  x=map(adc0,6877,10400,0,100);
      //  y=map(adc1,6900,10500,0,100);
       // z=map(adc2,6900,10500,0,100);
       // w=map(adc3,0,17463,0,100);

        // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
      }
      
      // Compute heat index in Fahrenheit (the default)
      float hif = dht.computeHeatIndex(f, h);
      // Compute heat index in Celsius (isFahreheit = false)
      float hic = dht.computeHeatIndex(t, h, false);
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.print(" *C ");
      Serial.print(f);
      Serial.println(" *F\t");
      /*
      Serial.print("Heat index: ");
      Serial.print(hic);
      Serial.print(" *C ");
      Serial.print(hif);
      Serial.println(" *F");
      */

      
     
      
    //  Serial.print("Moisture Sensor (adc0): ");
    //  Serial.println(x);
    //  Serial.print("Photosensitive Sensor (adc1): ");
    //  Serial.println(y);
    //  Serial.print("adc2: ");
    //  Serial.println(z);
    //  Serial.print("adc3: ");
    //  Serial.println(w);

      
      // printing raw value from pH Sensor
  /*    Serial.print("Moisture Raw Value > ");
      Serial.print(moistVal);
      Serial.print(", ");

      // converting analog to digital
      double vltValue = measuringVal * 3.04/1024.0 ;
      Serial.print("Voltage Value > ");
      Serial.print(vltValue, 3);
      Serial.print(", ");

      // calculating pH
      float P0 = 7 + ((2.5 - vltValue) / 0.12);
      Serial.print("pH Value > ");
      Serial.println(P0, 3);
*/

      Serial.print("Temperature in Celsius:");
      Serial.println(String(t).c_str());
      client.publish(temperature_celsius_topic, String(t).c_str(), true);
/*
      Serial.print("Temperature in Fahrenheit:");
      Serial.println(String(f).c_str());
      client.publish(temperature_fahrenheit_topic, String(f).c_str(), true);
*/
      Serial.print("Humidity:");
      Serial.println(String(h).c_str());
      client.publish(humidity_topic, String(h).c_str(), true);
/*
      Serial.print("pH Value:");
      Serial.println(String(P0).c_str());
      client.publish(ph_topic, String(P0).c_str(), true);
*/
      Serial.print("Moisture Sensor:");
      Serial.println(String(moistVal).c_str());
      client.publish(moist_topic, String(moistVal).c_str(), true);
/*
      Serial.print("Photosensitive Sensor:");
      Serial.println(String(y).c_str());
      client.publish(photo_topic, String(y).c_str(), true);

 */      
      }
    
     

      
      

}
