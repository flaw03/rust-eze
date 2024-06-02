#ifndef GESTION_CAPTEURS_H
#define GESTION_CAPTEURS_H

// #include <Arduino.h>

class GestionCapteurs {
public:
    GestionCapteurs(int* pins, int nbCapteurs, int delayTime);
    void init();
    void calibrate(unsigned long calibrationTime);
    void measure();
    bool isWhite(int sensorIndex);
    bool isBlack(int sensorIndex);
    // retoune des valeur de 0 a 255 -> 0 noire -> 255black
    int  getValue(int sensorIndex);
    void displayDetails();
    void displaySensorDetailsColor();

private:
    int* pins;
    int nbCapteurs;
    int delayTime;
    bool* states;
    unsigned long* startTimes;
    int* minValues;
    int* maxValues;
    int* thresholds;
    int* responseTimes;

    void setHigh(int sensorIndex);
    void setLow(int sensorIndex);
    void setOutput(int sensorIndex);
    void setInput(int sensorIndex);
    void updateMinValue(int sensorIndex);
    void updateMaxValue(int sensorIndex);
    void calculateThreshold(int sensorIndex);
};

#endif // GESTION_CAPTEURS_H
