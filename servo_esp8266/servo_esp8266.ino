#include <Servo.h>  // Include the Servo library
#include <ESP8266WiFi.h>

#define wifi_ssid "*******" // put your wifi ID
#define wifi_password "*******" // put your wifi Password
#define servoPin 3 // Declare the Servo pin

WiFiServer server(80);
Servo Servo1;  // Create a servo object

void setup() {
Serial.begin(115200);
Servo1.attach(servoPin); // We need to attach the servo to the used pin number

 // Connect to WiFi network
  Serial.println();
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop(){
  
// Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();


  // Match the request

  int servodegree = 90;
  
  if (request.indexOf("/deg=0") != -1)  {
    servodegree = 0;
    value = 0;
    Servo1.write(servodegree);   
   
  }

  if (request.indexOf("/deg=90") != -1)  {
    servodegree = 90;
    value = 90;
    Servo1.write(servodegree);    
  }

  if (request.indexOf("/deg=180") != -1)  {
    servodegree = 180;
    value = 180;
    Servo1.write(servodegree);   
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  
  if(servodegree == 90) {
    client.print("front wheel : straight");
  } 
  if(servodegree == 0) {
    client.print("front wheel : left");
  }

  if(servodegree == 180) {
    client.print("front wheel : right");
  }
  client.println("<br><br>");
  client.println("<a href=\"/deg=90\"\"><button>Straight </button></a>");
  client.println("<a href=\"/deg=0\"\"><button>Left </button></a>");  
  client.println("<a href=\"/deg=180\"\"><button>Right </button></a><br />");  
  client.println("</html>");

 
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  
}
