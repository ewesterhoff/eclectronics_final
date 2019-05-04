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
  Serial.begin(9600);   // This will communicate with PC via USB
  mySerial.begin(9600); // This will communicate with device via 10,11 pins with 9600 baud rate
  
  //pinMode(RXIND,OUTPUT); 
  while(!Serial || !mySerial){ //wait to connect
    ;
  }
  digitalWrite(RST,HIGH); 
  delay(200);
  mySerial.print("$$$");
  delay(70);
  mySerial.println("M");//finds signal strength of last pairerd device-- found on page 37 of user guide
  delay(70);
}
void loop() {
    
    //Serial.println(Serial.available());
    while (Serial.available()>0){
        mySerial.println("I");
        delay(50);
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
