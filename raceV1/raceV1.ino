#include <Arduino.h>
#include "GestionCapteurs.h"
#include "GestionMoteurs.h"

#define NB_CAPTEURS 3
#define TIME_CALIBRATION 5000
#define DELAY_TIME 10
#define PIN_FIRST_CAPTEUR 10
#define PIN_LED 13

int EN1 = 6;
int EN2 = 5;  
int IN1 = 7;
int IN2 = 4;

int pins[NB_CAPTEURS] = {PIN_FIRST_CAPTEUR, PIN_FIRST_CAPTEUR + 1, PIN_FIRST_CAPTEUR + 2};


double baseSpeedLeft = 255; // Vitesse de base pour le moteur gauche
double baseSpeedRight = 255; // Vitesse de base pour le moteur droit

GestionCapteurs capteurs(pins, NB_CAPTEURS, DELAY_TIME);
GestionMoteurs moteurs(EN1,EN2,IN1,IN2);

int cycleCount = 0;

void setup() {
    

    Serial.begin(9600);
    Serial.println("Init");
    moteurs.motor1(100,true);
    moteurs.motor2(0,true);
    capteurs.calibrate(TIME_CALIBRATION);
    capteurs.displayDetails();
    moteurs.motor1(0,false);
    moteurs.motor2(0,false)
}

void loop() {
    capteurs.measure();
    sensorValueLeft = capteurs.getValue(0);
    sensorValueCenter = capteurs.getValue(1);
    sensorValueRight =   capteurs.getValue(2);

    if (capteurs.isBlack(1) && capteurs.isBlack(3) ){
        moteurs.motorL(motorOutputLeft,true);
        moteurs.motorR(baseSpeedRight,true);
    }else if (capteurs.isWhite(1) && capteurs.isBlack(3)){
        moteurs.motorL(baseSpeedLeft);
        moteurs.motorR(0);
    }else if (capteurs.isBlack(1) && capteurs.isWhite(3)){
        moteurs.motorR(baseSpeedRight);
        moteurs.motoLR(0);
    }else if (capteurs.isWhite(1) && capteurs.isWhite(3)){
        moteurs.stop();
        digitalWrite(PIN_LED,HIGH);
    }

    Serial.print("Cycle ");
    Serial.print(cycleCount++);
}
   