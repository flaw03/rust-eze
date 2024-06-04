#include <Arduino.h>
#include "GestionCapteurs.h"
#include "GestionMoteurs.h"

#define NB_CAPTEURS 3
#define TIME_CALIBRATION 5000
#define DELAY_TIME 10
#define PIN_FIRST_CAPTEUR 10
#define PIN_LED 13
#define SEARCHTIMEOUT  5000

int EN1 = 6;
int EN2 = 5;  
int IN1 = 7;
int IN2 = 4;

int pins[NB_CAPTEURS] = {PIN_FIRST_CAPTEUR, PIN_FIRST_CAPTEUR + 1, PIN_FIRST_CAPTEUR + 2};


int baseSpeedLeft = 255; // Vitesse de base pour le moteur gauche
int baseSpeedRight = 255; // Vitesse de base pour le moteur droit

GestionCapteurs capteurs(pins, NB_CAPTEURS, DELAY_TIME);
GestionMoteurs moteurs(EN1,EN2,IN1,IN2);

enum RobotState { INIT_CAPTEURS, RACE, RECHERCHE_LIGNE, FIN_COURSE };
RobotState state = INIT_CAPTEURS;

unsigned long startTime = 0;

int cycleCount = 0;


void raceMode() {
    capteurs.measure();

    bool isLeftBlack = capteurs.isBlack(0);
    bool isCenterBlack = capteurs.isBlack(1);
    bool isRightBlack = capteurs.isBlack(2);

    if (isLeftBlack && isRightBlack) {
        moteurs.motorL(baseSpeedLeft, true);
        moteurs.motorR(baseSpeedRight, true);
    } else if (!isLeftBlack && isRightBlack) {
        moteurs.motorL(baseSpeedLeft,true);
        moteurs.motorR(0,true);
    } else if (isLeftBlack && !isRightBlack) {
        moteurs.motorR(baseSpeedRight,true);
        moteurs.motorL(0,true);
    } else if (!isLeftBlack && !isRightBlack) {
        moteurs.stop();
        digitalWrite(PIN_LED, HIGH);
        startTime = millis();
        state = RECHERCHE_LIGNE;
    }
}

void rechercheLigne() {
    moteurs.motorL(100, true);
    moteurs.motorR(100, false);
    digitalWrite(PIN_LED, millis() % 500 < 250 ? HIGH : LOW);  // Clignotement LED

    capteurs.measure();
    if (capteurs.isBlack(0) || capteurs.isBlack(1) || capteurs.isBlack(2)) {
        digitalWrite(PIN_LED, LOW);
        state = RACE;
    } else if (millis() - startTime >= SEARCHTIMEOUT) {
        state = FIN_COURSE;
    }
}



void calibrage(){
    moteurs.motorL(100,true);
    moteurs.motorR(0,true);
    capteurs.calibrate(TIME_CALIBRATION);
    moteurs.motorL(0,false);
    moteurs.motorR(0,false);
    capteurs.displayDetails();
    state = RACE;
}

void finCourse() {
    moteurs.stop();
    digitalWrite(PIN_LED, HIGH);
}


void setup() {
    Serial.begin(9600);
    Serial.println("Init");
    digitalWrite(PIN_LED, LOW);
    moteurs.motorL(0,false);
    moteurs.motorR(0,false);
}

void loop() {
    switch (state) {
        case INIT_CAPTEURS:
            calibrage();
            break;

        case RACE:
            raceMode();
            break;

        case RECHERCHE_LIGNE:
            rechercheLigne();
            break;

        case FIN_COURSE:
            finCourse();
            break;
    }
}