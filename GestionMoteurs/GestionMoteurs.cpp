#include <Arduino.h>
#include "GestionMoteurs.h"

GestionMoteurs::GestionMoteurs(int EN1,int EN2,int IN1,int IN2){
    this->EN1 = EN1;
    this->EN2 = EN2;
    this->IN1 = IN1;
    this->IN2 = IN2;
    pinMode(EN1,OUTPUT);
    pinMode(IN1,OUTPUT);
    pinMode(EN2,OUTPUT);
    pinMode(IN2,OUTPUT);
}
void GestionMoteurs::motorR(int pwm, boolean reverse){
    analogWrite(EN1, pwm); //set pwm control, 0 for stop, and 255 for maximum speed
    if(reverse)  {
        digitalWrite(IN1, HIGH);
    }
    else {
        digitalWrite(IN1, LOW);
    }
}
void GestionMoteurs::motorL(int pwm, boolean reverse){
    analogWrite(EN2, pwm); //set pwm control, 0 for stop, and 255 for maximum speed
    if (reverse)  {
        digitalWrite(IN2, HIGH);
    }
    else  {
        digitalWrite(IN2, LOW);
    }
}
void GestionMoteurs::turnRight(){
    motorL(250,true);
    motorR(150,true);
}
void GestionMoteurs::turnLeft(){
    motorL(150,true);
    motorR(250,true);
}
void GestionMoteurs::stop(){
    motorR(0,true);
    motorL(0,true);
}
