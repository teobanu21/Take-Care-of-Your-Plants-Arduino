//nodemcu
//transmisia datelor catre site-ul ThingSpeak.com unde am creat un canal pe care se poate urmari evolutia plantei
//link canal https://thingspeak.com/channels/1778168

#include <Wire.h>
#include <SPI.h>
#include <DNSServer.h>
#include<ESP8266WiFi.h> 

#include <ThingSpeak.h>
unsigned long myChannelNumber = 1778168;
const char * myWriteAPIKey = "GXWER7ZU31C5J6CR";


const char* ssid = "TEO 2865"; // Write here your router's username
const char* password = "nR27@200"; // Write here your router's passward

int status = WL_IDLE_STATUS;
WiFiClient  client;

//WiFiServer server(80);

void setup() {
  Serial.begin(9600); /* begin serial for debug */
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */


  //Serial.print("Connecting to ");
  //Serial.println(ssid); 
  
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected"); 
 
  //server.begin();
  //Serial.println("Server started");
  //Serial.print("Use this URL to connect: ");
  //Serial.print("http://");
  //Serial.print(WiFi.localIP());
  //Serial.println("/"); 

}

void loop() {
     Wire.beginTransmission(8); /* begin with device address 8 */
     Wire.write("Hello Arduino");  /* sends hello string */
     Wire.endTransmission();    /* stop transmitting */

   /* 
    WiFiClient client = server.available();
    if (!client) {
      return;
    }
     
    Serial.println("new client");
    /*while(!client.available()){
        delay(1);
    } */
  
   /*client.println("HTTP/1.1 200 OK");
   client.println("Content-Type: text/html");
   client.println(""); 
         
   client.println("<!DOCTYPE HTML>");
   client.println("<html>");
   */

    /*
    int i=0;
    for(i=0;i<4;i++){
       Wire.requestFrom(8, 32); // request & read data of size 13 from slave 
       while(Wire.available()){
          char c = Wire.read();
          Serial.print(c);
          client.print(c);
       }
       
       delay(100);
    }
    */

   //client.println("<br><br>");
   //client.println("</html>");
    
     Serial.println();

    String printare;

//temp
     Wire.requestFrom(8, 32); // request & read data of size 13 from slave 
     while(Wire.available()){
          char c = Wire.read();
          Serial.print(c);
          printare=printare+c;
          //client.print(c);
       }
 
     delay(100);
    
     Serial.println();
     float temp=printare.toFloat();
     printare=""; 
     
//umiditate
     Wire.requestFrom(8, 32); // request & read data of size 13 from slave 
       while(Wire.available()){
          char c = Wire.read();
          Serial.print(c);
          printare=printare+c;
          //client.print(c);
       }
    
     Serial.println();   
     delay(100);
     float hum=printare.toFloat();
     printare=""; 

//intensitate luminoasa
  Wire.requestFrom(8, 32); // request & read data of size 13 from slave 
       while(Wire.available()){
          char c = Wire.read();
          Serial.print(c);
          printare=printare+c;
          //client.print(c);
       }
     
     Serial.println();  
     delay(100);
     float intensitate_luminoasa=printare.toFloat();
     printare=""; 

//planta udata
Wire.requestFrom(8, 32); // request & read data of size 13 from slave 
       while(Wire.available()){
          char c = Wire.read();
          Serial.print(c);
          printare=printare+c;
          //client.print(c);
       }
       Serial.println();
       delay(100);
     float uda=printare.toFloat();
     printare=""; 

Serial.println("Afisare date float ce sunt transmise catre canal");

     Serial.println(temp);
     Serial.println(hum);
     Serial.println(intensitate_luminoasa);
     Serial.println(uda);
     
     ThingSpeak.writeField(myChannelNumber, 1, temp, myWriteAPIKey);
     ThingSpeak.writeField(myChannelNumber, 2, hum, myWriteAPIKey);
     ThingSpeak.writeField(myChannelNumber, 3, intensitate_luminoasa, myWriteAPIKey);
     ThingSpeak.writeField(myChannelNumber, 4, uda, myWriteAPIKey);
     
     delay(5000); // ThingSpeak will accept updates every 5 seconds.
 
}
