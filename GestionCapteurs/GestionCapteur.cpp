#include "GestionCapteurs.h"

GestionCapteurs::GestionCapteurs(int* pins, int nbCapteurs, int delayTime) {
    this->pins = pins;
    this->nbCapteurs = nbCapteurs;
    this->delayTime = delayTime;
    this->states = new bool[nbCapteurs];
    this->startTimes = new unsigned long[nbCapteurs];
    this->minValues = new int[nbCapteurs];
    this->maxValues = new int[nbCapteurs];
    this->thresholds = new int[nbCapteurs];
    this->responseTimes = new int[nbCapteurs];

    for (int i = 0; i < nbCapteurs; i++) {
        this->minValues[i] = 9999;
        this->maxValues[i] = 0;
    }
}

void GestionCapteurs::init() {
    for (int i = 0; i < nbCapteurs; i++) {
        states[i] = false;
        startTimes[i] = 0;
        responseTimes[i] = 0;
    }
}

void GestionCapteurs::calibrate(unsigned long calibrationTime) {
    Serial.println("------- Starting calibration -------");
    unsigned long startTime = millis();
    while (millis() - startTime <= calibrationTime) {
        measure();
        for (int i = 0; i < nbCapteurs; i++) {
            updateMinValue(i);
            updateMaxValue(i);
        }
    }
    for (int i = 0; i < nbCapteurs; i++) {
        calculateThreshold(i);
    }
    Serial.println("------- Starting calibration -------");

}

void GestionCapteurs::measure() {
    init();
    int timeout;
    for (int i = 0; i < nbCapteurs; i++) {
        setOutput(i);
        setHigh(i);
    }
    delayMicroseconds(delayTime); // attendre x delay
    for (int i = 0; i < nbCapteurs; i++) {
        setInput(i);
        setLow(i);
    }
    for (int i = 0; i < nbCapteurs; i++) {
        while (!states[i]) {
            timeout = micros() - startTimes[i];
            if (digitalRead(pins[i]) == LOW || timeout > 1000) {
                states[i] = true;
                responseTimes[i] = timeout;
                updateMaxValue(i);
                updateMinValue(i);
            }
        }
    }
}

bool GestionCapteurs::isWhite(int sensorIndex) {
    if (sensorIndex < 0 || sensorIndex >= nbCapteurs) {
        Serial.print("Error: Sensor index ");
        Serial.print(sensorIndex);
        Serial.println(" is out of bounds !");
        return false;
    }
    return responseTimes[sensorIndex] < 128 ;
}
bool GestionCapteurs::isBlack(int sensorIndex) {
    if (sensorIndex < 0 || sensorIndex >= nbCapteurs) {
        Serial.print("Error: Sensor index ");
        Serial.print(sensorIndex);
        Serial.println(" is out of bounds !");
        return false;
    }
    return responseTimes[sensorIndex] >= 128 ;
}



int GestionCapteurs::getValue(int sensorIndex) {
    if (sensorIndex < 0 || sensorIndex >= nbCapteurs) {
        Serial.print("Error: Sensor index ");
        Serial.print(sensorIndex);
        Serial.println(" is out of bounds !");
        return -1;
    }
    
    return map( responseTimes[sensorIndex], minValues[sensorIndex], maxValues[sensorIndex], 255, 0);
}

void GestionCapteurs::displayDetails() {
    for (int i = 0; i < nbCapteurs; i++) {
        Serial.print("Sensor ");
        Serial.print(i + 1);
        Serial.print(": Min: ");
        Serial.print(minValues[i]);
        Serial.print(" | Max: ");
        Serial.print(maxValues[i]);
        Serial.print(" | Threshold: ");
        Serial.print(thresholds[i]);
        Serial.println();
    }
}

void displaySensorDetailsColor(){
  for (int i = 0; i < nbCapteurs; i++) {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": Value :");
    Serial.print(responseTimes[i]);
    Serial.print("| Min: ");
    Serial.print(minValues[i]);
    Serial.print(" | Max: ");
    Serial.print(maxValues[i]);
    Serial.print(" | Threshold: ");
    Serial.print(thresholds[i]);
    Serial.print(" | Color: ");
    if (isWhite(i)){
      Serial.print("White");
    }else{
      Serial.print("Black");
    }
    Serial.println();
  }
}

void GestionCapteurs::setHigh(int sensorIndex) {
    digitalWrite(pins[sensorIndex], HIGH);
    startTimes[sensorIndex] =  micros();;
}

void GestionCapteurs::setLow(int sensorIndex) {
    digitalWrite(pins[sensorIndex], LOW);
}

void GestionCapteurs::setOutput(int sensorIndex) {
    pinMode(pins[sensorIndex], OUTPUT);
}

void GestionCapteurs::setInput(int sensorIndex) {
    pinMode(pins[sensorIndex], INPUT);
}

void GestionCapteurs::updateMinValue(int sensorIndex) {
    if (responseTimes[sensorIndex] < minValues[sensorIndex]) {
        minValues[sensorIndex] = responseTimes[sensorIndex];
    }
}

void GestionCapteurs::updateMaxValue(int sensorIndex) {
    if (responseTimes[sensorIndex] > maxValues[sensorIndex]) {
        maxValues[sensorIndex] = responseTimes[sensorIndex];
    }
}

void GestionCapteurs::calculateThreshold(int sensorIndex) {
    thresholds[sensorIndex] = (minValues[sensorIndex] + maxValues[sensorIndex]) / 2;
}
