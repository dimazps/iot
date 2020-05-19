#include <ESP8266WiFi.h>
 
#define wifi_ssid "*****" // put your wifi ID
#define wifi_password "*****" // put your wifi Password
 
int ledPinGreen = 2; // GPIO2---D4 of NodeMCU
int ledPinRed = 13; // GPIO13---D7 of NodeMCU
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPinRed, OUTPUT); 
  pinMode(ledPinGreen, OUTPUT);
 
 
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
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new input");
/*  while(!client.available()){
    delay(1);
  }
 */
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request


  int greenvalue = LOW;  
  if (request.indexOf("/GREENLED=ON") != -1)  {
    digitalWrite(ledPinGreen, HIGH);
    greenvalue = HIGH;
  }
  if (request.indexOf("/GREENLED=OFF") != -1)  {
    digitalWrite(ledPinGreen, LOW);
    greenvalue = LOW;
  }

  
  int redvalue = LOW;
  if (request.indexOf("/REDLED=ON") != -1)  {
    digitalWrite(ledPinRed, HIGH);
    redvalue = HIGH;
  }
  if (request.indexOf("/REDLED=OFF") != -1)  {
    digitalWrite(ledPinRed, LOW);
    redvalue = LOW;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  
  if(greenvalue == HIGH) {
    client.print("Led Green is now: ON");
  } 
 if(greenvalue == LOW) {
    client.print("Led Green is now: OFF");
  }
  client.println("<br><br>");
  client.println("<a href=\"/GREENLED=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/GREENLED=OFF\"\"><button>Off </button></a><br />");  
  client.println("</html>");

   client.print("<br><br>");
 
  if(redvalue == HIGH) {
    client.print("Led RED is now: ON");
  } 
 if(redvalue == LOW){
    client.print("Led RED is now: OFF");
  }
  client.println("<br><br>");
  client.println("<a href=\"/REDLED=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/REDLED=OFF\"\"><button>Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
