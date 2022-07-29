// proiect final pt programarea placutei arduino si transmiterea datelor catre nodemcu
#include <DHT.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>



#define MAX_TEMP  25      //temp maxima sub care se permite udatul plantei -> activarea pompei de apa
#define MIN_LIGHT 40

#define DHTPIN 6        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE) ;

LiquidCrystal_I2C lcd(0x20,24,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//setup trimitere pachete catre nodemcu
String packet;

float tempr;
float umiditate;
float intensitate_luminoasa;
float uda_planta;

int index_pachet = 0;
/////////////////////////////////////////

const int fotoPin=0;          //fotorezistor
const int ledPin =  11;       //led care semnalizeaza miscarea 
const int MotionPin = 12;     //sensor miscare
const int buzzerPin = 7;      //buzzer
const int humidityPin = 2;    //senzor umiditate sol
const int pumpPin = 4;        //motoras apa
const int ledOpenPin = 5;     //led iluminat planta
const int buttonPin = 9;

//const int servoPin = 13;

void turnOnLed (){
       digitalWrite(ledPin, HIGH);    //Turn on led
}

void turnOffLed (){
       digitalWrite(ledPin, LOW);    //Turn off led
}

void buzzerOn(){          //porneste buzzer atunci cand este detectata miscare in jurul plantei
  tone(buzzerPin, 900); // Send 1KHz sound signal...
  delay(50);
  tone(buzzerPin, 100);
}

void buzzerOff(){         //opreste buzzer
  noTone(buzzerPin);
}

void temperature_humidity_air(){                            //senzor dht11 citeste date legate de temp si umiditatea aerului 

      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
          float h = dht.readHumidity();
   
      // Read temperature as Celsius (the default)
          float t = dht.readTemperature();
 
     if (isnan(h) || isnan(t)) {
           lcd.print(F("Failed to read from DHT sensor!"));
           return;
     }

//afisam pe ecranul lcd informatiile din mediu
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print(" Hum.: ");
      lcd.print(h);
      lcd.print("%");
      lcd.setCursor(0,1);
      lcd.print("Temp.: ");
      lcd.print(t);
      lcd.print(F("C "));
}


int getSoilMoisture() {
    int val = digitalRead(humidityPin);         // Read the analog value from sensor
  
    return val;                                 // Return analog moisture value
}

float getPhotoresistorValue() {
    int sensorValue = analogRead(fotoPin);           // read the input on analog pin 0
    float voltage = sensorValue * (100 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a percentage 
 
    return voltage;
}

/*
Servo servo;
void servoStart(){
  
  servo.write(90);
  delay(500);
  servo.write(0);
  delay(500);
  servo.write(0);
  delay(500);
  servo.write(90);
  delay(500);
  servo.write(180);
  delay(500);
}
*/

void setup() {
    pinMode(ledPin, OUTPUT);          //setare pini 
    pinMode(MotionPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    
    lcd.init();                      // initialize the lcd

    pinMode(pumpPin, OUTPUT);
    pinMode(ledOpenPin, OUTPUT);

////////////COMUNICATIE NODEMCU //////////////////////////

    Wire.begin(8);                /* join i2c bus with address 8 */
    Wire.onReceive(receiveEvent); /* register receive event */
    Wire.onRequest(requestEvent); /* register request event */

///////////////////////////////////////////////////////////

   Serial.begin(9600);              //afisare in consola
   dht.begin();                     //initializare senzor dht

  //servo.attach(servoPin);

}
  
void loop()
  {

    temperature_humidity_air();                       //apel functie temperatura+umiditate aer pentru a le afisa pe ecranul lcd

    float light_value = getPhotoresistorValue();      //intensitatea luminii din jurul plantei
    int soilDry = getSoilMoisture();                  //umiditatea solului plantei -> 0 ud, 1 uscat
    float temp = dht.readTemperature();               //temperatura aerului
    
    Serial.print(light_value);                        //afisam in consola intensitatea luminoasa in procentaj
    Serial.println("%"); 
        
    Serial.println(getSoilMoisture());               //afiseaza in consola daca e ud(0) sau nu(1) solul

    //intensitate_luminoasa= "Intensitate luminoasa: " + String(light_value);
    //intensitate_luminoasa=intensitate_luminoasa+"%";
    //intensitate_luminoasa= intensitate_luminoasa+"\n";

    //float hum = dht.readHumidity();
    //umiditate = "Umiditate: " + String(hum); 
    //umiditate=umiditate+"\n";

    //tempr = "Temparatura: " + String(temp);  
    //tempr=tempr+"\n";
    
    //packet = intensitate_luminoasa+tempr+umiditate;

   // Serial.println(packet);


    intensitate_luminoasa = light_value;
    tempr=temp;
    umiditate=dht.readHumidity();

     
    if (digitalRead(MotionPin) == HIGH)             //daca este detectata miscare activam "sperietoarea" -> buzzer + led rosu alerta
    {
          turnOnLed();
          //buzzerOn();
          delay(200);
          turnOffLed();
         // buzzerOff();
    }
    else 
    {
         // buzzerOff();
          turnOffLed();
    }


//verificam daca solul este uscat pentru e porni pompa de apa
    if (soilDry) 
    {
        if (temp < MAX_TEMP)
        {
            digitalWrite(pumpPin, LOW);
            //uda_planta="Planta a fost udata";
            uda_planta=1;
        }
        else 
        {
             digitalWrite(pumpPin, HIGH);
        }
    } 
    else 
    {
       digitalWrite(pumpPin, LOW);
    }


//verificam luminozitatea din jurul plantei, daca nu este suficienta lumina vom aprinde un led       
    if(light_value < MIN_LIGHT)
    {
      digitalWrite(ledOpenPin, HIGH);
    } 
    else
    {
      digitalWrite(ledOpenPin, LOW);
    }

    if(digitalRead(buttonPin)){         //pornirea pompei de apa pentru 2s la apasarea butonului
      lcd.clear();
      lcd.print("udam planta");
      digitalWrite(pumpPin, LOW);
       uda_planta=1;
      delay(2000);
      digitalWrite(pumpPin,HIGH);
    }

      //servoStart();
  
       delay(1000);
  }



////////////////////FUNCTII COMUNICATIE INTRE ARDUINI SI NODEMCU'

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
   while (0 <Wire.available()) {
      char c = Wire.read();      /* receive byte as a character */
      Serial.print(c);           /* print the character */
   }
   Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() {
  //char buff[1000];
  //packet.toCharArray(buff,1000);
  //Serial.println(buff);

  String val;
  
  int i;
  if(index_pachet%4==0){
    //for (i=0;i<32;i++){
    //    Wire.write(tempr[i]);  /*send string on request */
    //}
    //tempr="";
     
     val=String(tempr,2);

    for(int i=0;i<32;i++){
        Wire.write(val[i]);
    }
  }

   if(index_pachet%4==1){
    //for (i=0;i<32;i++){
      //  Wire.write(umiditate[i]);  /*send string on request */
    //}
    //umiditate="";

     val=String(umiditate,2);

    for(int i=0;i<32;i++){
        Wire.write(val[i]);
    }
    
   }

   if(index_pachet%4==2){
   // for (i=0;i<32;i++){
    //    Wire.write(intensitate_luminoasa[i]);  /*send string on request */
    //}
    //intensitate_luminoasa="";

      val=String(intensitate_luminoasa,2);

    for(int i=0;i<32;i++){
        Wire.write(val[i]);
    }
    
   } 
 
   if(index_pachet%4==3){
    //for (i=0;i<32;i++){
      //  Wire.write(uda_planta[i]);  /*send string on request */
    //}

    val=String(uda_planta,2);

    for(int i=0;i<32;i++){
        Wire.write(val[i]);
    }
    
    uda_planta=0;
    
    //uda_planta="";
   }
   index_pachet++;
  }









  