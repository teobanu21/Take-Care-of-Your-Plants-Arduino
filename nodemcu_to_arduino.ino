//conexiune intre nodemcu si arduino care sa permita transmisia de date catre un pc
#include <SPI.h>
#include <DNSServer.h>

#include<ESP8266WiFi.h> 
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(D1,D2); //RX tx

const char* ssid = "iPhone 11"; //your WiFi Name
const char* password = "password";  //Your Wifi Password
int ledPin = 03; 
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  //mySerial.begin(9600);
  delay(10); 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  server.begin();
  Serial.println("Server started");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/"); 
}

void loop() {

  //String msg = mySerial.readStringUntil('\r');
  //Serial.println(msg);
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  } 
  Serial.println("Client request");
  String request = client.readStringUntil('\r');
  delay(1000);
  Serial.println(request);
  client.flush(); 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
 
 client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Off </button></a><br />");  
  client.println("</html>");
  
     delay(1);
   Serial.println("Client disonnected");
  Serial.println("");   }
