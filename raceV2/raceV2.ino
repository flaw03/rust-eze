#include <Arduino.h>
#include "GestionCapteurs.h"
#include <PID_v1_bc.h>
#include "GestionMoteurs.h"

#define NB_CAPTEURS 3
#define TIME_CALIBRATION 5000
#define DELAY_TIME 10
#define PIN_FIRST_CAPTEUR 10

int EN1 = 6;
int EN2 = 5;  
int IN1 = 7;
int IN2 = 4;

int pins[NB_CAPTEURS] = {PIN_FIRST_CAPTEUR, PIN_FIRST_CAPTEUR + 1, PIN_FIRST_CAPTEUR + 2};
double kpLeft = 1.0, kiLeft = 0.0, kdLeft = 0.0;
double kpRight = 1.0, kiRight = 0.0, kdRight = 0.0;



double sensorErrorLeft, sensorErrorRight;
double motorOutputLeft, motorOutputRight;

double baseSpeedLeft = 255; // Vitesse de base pour le moteur gauche
double baseSpeedRight = 255; // Vitesse de base pour le moteur droit

PID pidLeft(&sensorErrorLeft, &motorOutputLeft, &baseSpeedLeft, kpLeft, kiLeft, kdLeft, DIRECT);
PID pidRight(&sensorErrorRight, &motorOutputRight, &baseSpeedRight, kpRight, kiRight, kdRight, DIRECT);

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
    moteurs.motor1(50,false);
    moteurs.motor2(0,false)
}

void loop() {
    capteurs.measure();
    sensorValueLeft = capteurs.getValue(0);
    sensorValueCenter = capteurs.getValue(1);
    sensorValueRight =   capteurs.getValue(2);

    sensorErrorLeft = (sensorValueLeft + sensorValueCenter);
    sensorErrorRight = (sensorValueRight + sensorValueCenter);

    pidLeft.Compute();
    pidRight.Compute();

    
    moteurs.motor1(motorOutputLeft,true);
    moteurs.motor2(motorOutputRight,true);

    Serial.print("Cycle ");
    Serial.print(cycleCount++);
}
   