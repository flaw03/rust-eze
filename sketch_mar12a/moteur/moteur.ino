//This motor shield use Pin 6,5,7,4 to control the motor
// Simply connect your motors to M1+,M1-,M2+,M2-
// Upload the code to Arduino/Roboduino
// Through serial monitor, type 'a','s', 'w','d','x' to control the motor
// www.dfrobot.com
// Last modified on 24/12/2009

int EN1 = 6;
int EN2 = 5;  
int IN1 = 7;
int IN2 = 4;

void Motor1(int pwm, boolean reverse) {
  analogWrite(EN1, pwm); //set pwm control, 0 for stop, and 255 for maximum speed
  if (reverse)  {
    digitalWrite(IN1, HIGH);
  }
  else  {
    digitalWrite(IN1, LOW);
  }
}

void Motor2(int pwm, boolean reverse) {
  analogWrite(EN2, pwm);
  if (reverse)  {
    digitalWrite(IN2, HIGH);
  }
  else  {
    digitalWrite(IN2, LOW);
  }
}


void turnRight(){
  Motor2(150,true);
  Motor1(250,1);
}

void turnLeft(){
  Motor2(250,true);
  Motor1(150,true);
}

void setup() {
  int i;
  for (i = 4; i <= 7; i++) //For Arduino Motor Shield
    pinMode(i, OUTPUT);  //set pin 4,5,6,7 to output mode
  Serial.begin(9600);
}

void loop() {
  turnLeft()
  delay(1000)
  turnRight(1000)
  delay(1000)

}