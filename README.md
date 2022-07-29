# Take Care of Your Plants - Arduino

_**Descrierea proiectului:**_
        Scopul principal al proiectului este de a dezvolta un sistem automatizat care sa aiba grija de plantele tale. Acesta este proiectat sa ude planta atunci cand detecteaza ca solul este uscat (dar doar in anumite conditii) prin intermediul unei pompe de apa actionata automat, ii creeaza conditii de luminozitate optime pentru dezvoltare atunci cand intensitatea luminoasa scade sub un anumit procent prin aprinderea unui led. Planta poate fi udata si la dorinta proprietarului prin apasarea unui buton ce actioneaza pompa de apa. 
        De asemenea, in cazul in care pasarile iti ataca planta se porneste un sistem sonor si luminos calibrat in asa fel incat sa sperie orice se 
apropie. Acesta este declansat prin intermediul unui senzor de miscare. Sistemul va tine constant evidenta datelor din mediul inconjurator, acestea 
fiind afisate pe un ecran lcd.

**Componente utilizate:**
- placuta Arduino UNO
- USB Cable A-B for Arduino
- I/O Expansion Shield V7.1
- breadboard
- placuta NodeMCU
- I2C/TWI LCD 1602 Module
- Digital infrared motion sensor
- Gravity Digital Piranha LED Module-Red
- Buzzer Module
- Digital Push Button
- DHT11 Temperature and Humidity sensor
- Fotorezistor
- Rezistenta
- Pompa de apa 3-6V
- Releu
- Baterie
- Senzor umiditate sol 
- Tranzistor
- LED Galben 
- Cabluri


**Testarea componentelor individual:**

**1. Digital infrared motion sensor + led rosu**
    
Am urmarit citit urmatoarele linkuri:
            
- https://www.dfrobot.com/product-472.html
- https://www.youtube.com/watch?v=tDGFiVR9X5U&t=175s
- https://create.arduino.cc/projecthub/electropeak/               
- https://create.arduino.cc/projecthub/electropeak/         
- https://create.arduino.cc/projecthub/electropeak/pir-motion-sensor-how-to-use-pirs-w-arduino-raspberry-pi-18d7fa
- https://wiki.keyestudio.com/Ks0232_keyestudio_Red_LED_Module


_Parte de cod:_
 

    ` const int ledPin =  11;
    const int MotionPin = 12;
    void turnOnLed (){
        digitalWrite(ledPin, HIGH);    //Turn on led
        //delay(1000); 
    }
    void turnOffLed (){
        digitalWrite(ledPin, LOW);    //Turn on led
        //delay(1000);
    }
    void setup() {
        pinMode(ledPin, OUTPUT);          //setare pini 
        pinMode(MotionPin, INPUT);
    }
    void loop(){
         if (digitalRead(MotionPin) == HIGH)             //daca este detectata miscare activam "sperietoarea" -> buzzer + led rosu alerta
    {
          turnOnLed();
          delay(200);
          turnOffLed();
    }
    else 
    {
          turnOffLed();
    }
    }
    `



**2. Buzzer Module**

Linkuri utilizate:
-  https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody
- https://create.arduino.cc/projecthub/SURYATEJA/use-a-buzzer-module-piezo-speaker-using-arduino-uno-89df45
- https://www.instructables.com/How-to-use-a-Buzzer-Arduino-Tutorial/


_Parte de cod_
```
`const int buzzerPin = 7;      //buzzer
 void buzzerOn(){          //porneste buzzer atunci cand este detectata miscare in jurul plantei
  tone(buzzerPin, 900); // Send 1KHz sound signal...
  delay(50);
  tone(buzzerPin, 100);
}

void buzzerOff(){         //opreste buzzer
  noTone(buzzerPin);
}   
`
```


Integrare cu sistemul anterior:
   
```
    if (digitalRead(MotionPin) == HIGH)             //daca este detectata miscare activam "sperietoarea" -> buzzer + led rosu alerta
    {
          turnOnLed();
          buzzerOn();
          delay(200);
          turnOffLed();
         buzzerOff();
    }
    else 
    {
          buzzerOff();
          turnOffLed();
    }

```

**3. I2C/TWI LCD 1602 Module**

Ecranul a fost mai dificil de conectat decat componetele anterioare, intrucat pe langa conexiunea fizica, era necesara identificarea unui port la care acesta este conectat. Dificultatea aparea intrucat adresa afisata de unul din programe era diferita de cea reala. Astfel a durat ceva pana sa gasim un program care sa genereze adresa corecta. Am folosit codul din urmatorul link: https://playground.arduino.cc/Main/I2cScanner/

Dupa ce am testat o parte din functiile ecranului lcd folosind exemplele furnizate de biblioteca Liquid Crystal am hotarat ca vom folosi ecranul doar pentru a afisa informatii legate de temperatura si umiditate aerului si de asemenea cate un mesaj sugestiv cand este detectata miscare/ cand udam planta.

_Cod testat:_

```
//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");
   lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
   lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");
}


void loop()
{
}
```

Intrucat afisam doar o parte din informatii pe ecranul lcd, am ales sa lucram si prin intermediul consolei pentru a afisa diferiti parametri.

Initializare in functia setup():
`Serial.begin(9600);              //afisare in consola`

Am testat si functiile print() si println() -> print + newline

**4. DHT11 Temperature and Humidity sensor**

Conectarea pe placuta Arduino a fost foarte simpla intrucat trebuiau legati 3 pini-> Vcc, Ground si cel pt transmisie de date
Linkul folosit pentru a configura senzorul este: https://create.arduino.cc/projecthub/pibots555/how-to-connect-dht11-sensor-with-arduino-uno-f4d239

Acest senzor a fost mai dificil de codat intrucat au trebuit adaugate 2 biblioteci noi pe care nu reuseam sa le gasim direct in aplicatia de Arduino, iar cele de pe internet dadeau foarte multe erori de tipul ca nu exista/nu este definit un anumit parametru. Am instalat DHT sensor Library si Adafruit Unified Sensor.

_Codul creat + afisarea pe ecranul LCD este urmatorul:_

```
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 6        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE) ;


LiquidCrystal_I2C lcd(0x20,24,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


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

void setup() {    
  lcd.init();                      // initialize the lcd
  Serial.begin(9600);              //afisare in consola
  dht.begin();                     //initializare senzor dht
}

void loop(){
  temperature_humidity_air();                       //apel functie temperatura+umiditate aer pentru a le afisa pe ecranul lcd
}
```

**5. Fotorezistorul**

Pentru acesta a trebuit conectata si o rezistenta, intrucat functioneaza la o tensiune mai mica decat cea de pe breadbord (3V vs 5v). Configurarea a fost usoara, am utilizat https://www.instructables.com/How-to-use-a-photoresistor-or-photocell-Arduino-Tu/#:~:text=A%20photoresistor%20or%20photocell%20is,and%20dark%2Dactivated%20switching%20circuits.

Fotorezistorul da date analog asa ca am convertit informatiile de la acesta in proente folosind urmatoarea formula:
    sensorValue * (100 / 1023.0)

Am afisat in consola intensitatea luminoasa data de fotorezistor.

_Cod:_

```
const int fotoPin=0;          //fotorezistor

float getPhotoresistorValue() {
  int sensorValue = analogRead(fotoPin);           // read the input on analog pin 0
  float voltage = sensorValue * (100 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a percentage 
 
  return voltage;
}

void loop(){
    float light_value = getPhotoresistorValue();      //intensitatea luminii din jurul plantei
   
    Serial.print(light_value);                        //afisam in consola intensitatea luminoasa in procentaj
    Serial.println("%"); 
}
```

**6. Led galben (cel pentru iluminarea plantei)**

Utilizam fotorezistorul pentru a activa acest led.

_Codul pentru activarea ledului in anumite conditii de mediu:_

```
#define MIN_LIGHT 40

const int fotoPin=0;          //fotorezistor
const int ledOpenPin = 5;     //led iluminat planta


float getPhotoresistorValue() {
  int sensorValue = analogRead(fotoPin);           // read the input on analog pin 0
  float voltage = sensorValue * (100 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a percentage 
 
  return voltage;
}

void setup() {
    pinMode(ledPin, OUTPUT);          //setare pini 
    pinMode(MotionPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    
    pinMode(ledOpenPin, OUTPUT);

   Serial.begin(9600);              //afisare in consola

}

void loop(){
   float light_value = getPhotoresistorValue();      //intensitatea luminii din jurul plantei

  Serial.print(light_value);                        //afisam in consola intensitatea luminoasa in procentaj
  Serial.println("%"); 

  if(light_value < MIN_LIGHT)
    {
      digitalWrite(ledOpenPin, HIGH);
    } 
    else
    {
      digitalWrite(ledOpenPin, LOW);
    }

}
```

**7. Senzor umiditate sol**

Documentatie: https://create.arduino.cc/projecthub/electropeak/complete-guide-to-use-soil-moisture-sensor-w-examples-756b1f

Acest senzor a trebuit conectat prin intermediul unui releu. Senzorul returneaza 0 daca solul este ud si 1 in caz contrar.
Am folosit funtia digitalRead(). Am conectat acest senzor pe pinul 2.

Pentru a testa daca functioneaza am creat o functie getSoilMoisture() si am afisat in consola valoarea returnata. Pentru test am scufundat senzorul in apa.

```
const int humidityPin = 2;    //senzor umiditate sol

int getSoilMoisture() {
  int val = digitalRead(humidityPin);         // Read the analog value from sensor
  
  return val;                                 // Return analog moisture value
}

void loop(){
  int soilDry = getSoilMoisture();                  //umiditatea solului plantei -> 0 ud, 1 uscat
  Serial.println(getSoilMoisture());               //afiseaza in consola daca e ud(0) sau nu(1) solul
     
}
```

**8. Pompa de apa + pornire automata in functie de umiditate sol + pornire de la buton**


Documentatie pompa de apa + mod de conectare: https://arduinogetstarted.com/tutorials/arduino-controls-pump
Documentatie buton: https://docs.arduino.cc/built-in-examples/digital/Button

```
const int pumpPin = 4;        //motoras apa
const int buttonPin = 9;

void setup(){
  pinMode(pumpPin, OUTPUT);
  Serial.begin(9600);              //afisare in consola
}

void loop(){
  int soilDry = getSoilMoisture();                  //umiditatea solului plantei -> 0 ud, 1 uscat
   if (soilDry) 
    {
        if (temp < MAX_TEMP)
        {
            digitalWrite(pumpPin, LOW);
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

    if(digitalRead(buttonPin))   //pornirea pompei de apa pentru 2s la apasarea butonului
    {
        lcd.clear();
        lcd.print("udam planta");
        digitalWrite(pumpPin, LOW);
        delay(2000);
        digitalWrite(pumpPin,HIGH);
    }
}
```
**9. Transmiterea datelor catre o platforma web**

Am folosit site-ul https://thingspeak.com/ pentru a putea monitoriza planta de la distanta. Poti urmari pe telefon daca aceasta a fost udata, 
la ce temperaturi a fost supusa, intensitatea luminoasa si umiditatea, aspecte esentiale pentru o cultura prospera.

Link-ul canalului este: https://thingspeak.com/channels/1778168

Codul utilizat pentru conexiune este: upload_data_to_internet_final.ino