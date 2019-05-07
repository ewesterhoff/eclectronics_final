int pwm1 = 9;
int pwm2 = 10;
int dir = A5;
int notFault = A6;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir, OUTPUT);  
  pinMode(notFault, INPUT);
}

// the loop function runs over and over again forever
void loop() {
   analogWrite(pwm1, 0);
   analogWrite(pwm2, 0);
   //digitalWrite(dir, HIGH);
   int val = analogRead(notFault);
   Serial.println(val);
}
