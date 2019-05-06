/* TEST */
#include <SoftwareSerial.h>

/* Connect device Rx - 10, Tx - 11 */
SoftwareSerial mySerial(5,6); //RX, TX
int incomingByte=0; //int for reading serial data
int RST = 3;
int RXIND=A0;

void setup() {
  pinMode(RST, OUTPUT); 
  digitalWrite(RST,LOW);
  pinMode(RXIND, OUTPUT); 
  digitalWrite(RXIND,HIGH);
  Serial.begin(115200);   // This will communicate with PC via USB
  mySerial.begin(115200); // This will communicate with device via 10,11 pins with 9600 baud rate
  
  //pinMode(RXIND,OUTPUT); 
  while(!Serial || !mySerial){ //wait to connect
    ;
  }
  digitalWrite(RST,HIGH); 
  delay(200);
  mySerial.print("$$$");
  delay(70);

}
void loop() {
    
    //Serial.println(Serial.available());
    while (Serial.available()>0){
        mySerial.println("SB,09");
        delay(10);
        mySerial.println("SB,09");
        delay(10);
        mySerial.println("SB,09");
        Serial.read();
    }
  
      if(mySerial.available()>0){
        // read the incoming byte:
        incomingByte = mySerial.read();
      
        // say what you got:
        Serial.print("I received: ");
        Serial.println(char(incomingByte));}
      delay(10);
}
