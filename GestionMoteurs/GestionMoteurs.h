#ifndef GESTION_MOTORS_H
#define GESTION_MOTORS_H
#include <Arduino.h>

class GestionMoteurs{
public:
    GestionMoteurs(int EN1,int EN2,int IN1,int IN2);
    void motorL(int pwm, boolean reverse);
    void motorR(int pwm, boolean reverse);
    void turnRight();
    void turnLeft();
    void stop();

private:
    int EN1;
    int EN2;
    int IN1;
    int IN2;

};

#endif