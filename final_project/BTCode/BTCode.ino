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
int LED=13;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;
boolean hasStarted=false;
int dataNumber = 0;            
int prevDataNumber[5]={-100, -100, -100, -100, -100}; //starting values-- will mean buddy will not move until these values are filled 
int derivative=0;
bool forward=true;
int counter=6; //counter is initialized 6 so that if it needs to change direction initially, it can

void setup() {
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  pinMode(RST, OUTPUT); 
  digitalWrite(RST,LOW);
  pinMode(RXIND, OUTPUT); 
  digitalWrite(RXIND,HIGH);  
  Serial.begin(9600);   // This will communicate with PC via USB
  mySerial.begin(9600); // This will communicate with device via 10,11 pins with 9600 baud rate
  pinMode(pwm1, OUTPUT);
  digitalWrite(pwm1,0);
  pinMode(pwm2, OUTPUT);
  digitalWrite(pwm2,0);
  pinMode(dir, OUTPUT);  
  pinMode(notFault, INPUT);
  
  while(!Serial || !mySerial){ //wait to connect
    ;
  }
  digitalWrite(dir,LOW); //starts going forward
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
      takeDerivative();
      
      if(derivative < 0 && prevDataNumber[0] != -100){ //if the average derivative is positive or negative and the board has gone through the entire cycle of -100's
        changeDirection();
        //Serial.println("change direction");
        digitalWrite(LED,HIGH);
        
      }
      
      else if(derivative >=0 && prevDataNumber[0]!=-100) {
        stay();
        //Serial.println("stay");
        digitalWrite(LED,LOW);
      }


      //update 10 previous numbers to take average derivative
      for(int i=0;i<4;i++){ //put data into array and move data up in the array
        prevDataNumber[i]=prevDataNumber[i+1]; 
      }
      prevDataNumber[4]=dataNumber;
      
      delay(500); 
 }

//adapted from: https://forum.arduino.cc/index.php?topic=396450 
void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char startMarker= '-'; //b/c RSSI data is mostly negative, most of the data starts with '-'
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
        dataNumber = strtol(receivedChars,NULL,16);   //translate to hex int
        Serial.print("Decimal RSSI readings ... ");    
        Serial.println(dataNumber);     
        newData = false;
    }
}

void changeDirection(){
  if(forward && counter>5){ //counter must get higher than 5 to clear the buffer of previous negative values
    forward=false;
    digitalWrite(dir,HIGH);
  }
  else if(counter>5){
    forward=true;
    digitalWrite(dir,LOW);
  }
  counter++;
}

void stay(){
    int pwmStrength=int(60+(40/12)*abs(derivative)); //scaled from 40-80 pwm range
    digitalWrite(pwm1,pwmStrength);
    digitalWrite(pwm2,pwmStrength);

}

void Stop(){
  digitalWrite(pwm1,0);
  digitalWrite(pwm2,0);
}

void takeDerivative(){
  int sum=0;
  int deriv=0;
  for (int i=0;i<5;i++){
      deriv=(dataNumber-prevDataNumber[i]);
      sum=sum+deriv;
  }
  derivative=sum/4;
}
