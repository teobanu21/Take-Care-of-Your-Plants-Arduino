/*stabilire comunicatie seriala intre NodeMCU si Arduino */

#include <Wire.h>
//nodemcu
void setup() {
 Serial.begin(9600); /* begin serial for debug */
 Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
}

void loop() {
     Wire.beginTransmission(8); /* begin with device address 8 */
     Wire.write("Hello Arduino");  /* sends hello string */
     Wire.endTransmission();    /* stop transmitting */

    int i=0;
    for(i=0;i<4;i++){
       Wire.requestFrom(8, 32); /* request & read data of size 13 from slave */
       while(Wire.available()){
          char c = Wire.read();
          Serial.print(c);
       }
    }

    
     Serial.println();
     delay(1000);
 
}
