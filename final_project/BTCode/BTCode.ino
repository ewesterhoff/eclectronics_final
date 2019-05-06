/* TEST */
#include <SoftwareSerial.h>

/* Connect device Rx - 10, Tx - 11 */
SoftwareSerial mySerial(5,6); //RX, TX 
char incomingByte=0; //int for reading serial data
int RST = 3; 
int RXIND=A0; 
int pwm1 = 9; //left wheels
int pwm2 = 10; //right wheels
int dir = A5;
int notFault = A6;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;
boolean hasStarted=false;
int dataNumber = 0;            
int prevDataNumber=0; 

void setup() {
  pinMode(RST, OUTPUT); 
  digitalWrite(RST,LOW);
  pinMode(RXIND, OUTPUT); 
  digitalWrite(RXIND,HIGH);  
  Serial.begin(9600);   // This will communicate with PC via USB
  mySerial.begin(9600); // This will communicate with device via 10,11 pins with 9600 baud rate
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir, OUTPUT);  
  pinMode(notFault, INPUT);
  
  while(!Serial || !mySerial){ //wait to connect
    ;
  }
  digitalWrite(RST,HIGH); 
  delay(200);
  mySerial.print("$$$");
  delay(70);
}
void loop() {
  
      mySerial.println("M");//finds signal strength of last paired device-- found on page 37 of user guide
      delay(70);
      
      recvWithEndMarker();
      putTogetherNumber();
      
      if(dataNumber<prevDataNumber){
        turnRight();
        Serial.println("turn right.");
      }
      
      else{
        goStraight();
        Serial.println("go straight.");
      }
      
      prevDataNumber=dataNumber;
      
      delay(3000); //temporarily large
 }

//adapted from: https://forum.arduino.cc/index.php?topic=396450 
void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char startMarker= '-';
    char rc;
    
    
    while (mySerial.available() > 0) {
        rc = mySerial.read();
        //Serial.println(rc);
        if(rc == startMarker){
          hasStarted=true;
        }
        if (rc != endMarker && hasStarted==true) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else if (rc==endMarker && hasStarted==true){
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
            hasStarted=false;
        }
    }
}

void putTogetherNumber() { //adapted from: https://forum.arduino.cc/index.php?topic=396450 
    if (newData == true) {
        dataNumber = 0;
        dataNumber = strtol(receivedChars,NULL,16);   
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        Serial.print("Data as Number ... ");    
        Serial.println(dataNumber);     
        newData = false;
    }
}

void turnRight(){
  digitalWrite(pwm1,200); //needs to be calibrated
  delay(2000); // needs to be calibrated
  digitalWrite(pwm1,0);
}

void goStraight(){
  digitalWrite(pwm1,200);
  digitalWrite(pwm2,200);
  delay(2000);
  digitalWrite(pwm1,0);
  digitalWrite(pwm2,0);

}
