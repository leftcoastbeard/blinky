/*
 * File:   Control.h
 * Author: C0320318
 *
 * Created on October 28, 2014, 11:20 AM
 */

#ifndef CONTROL_H
#define CONTROL_H
//#include "mbed-drivers/mbed.h"

#define AUTOMATIC 1
#define MANUAL 0
#define DIRECT 0
#define REVERSE 1

class Control
{
public:

    Control(float *setpoint, float *input, float *output, float kp, float ki, float kd, bool dir);
    virtual ~Control();
    bool Kompute();
    void setKonstants(float Kp, float Ki, float Kd);
    void setMode(bool mode);
    void setLimits(float min, float max);
    void setDirection(bool dir);
    void updateSamplePeriod(int ms);

    float getKP();
    float getKI();
    float getKD();
    bool getMode();
    bool getDirection();
    unsigned long getSamplePeriod();

#if __cplusplus == 201103L
    constexpr static const float dt= 0.001;
#else
    static const float dt= 0.001;
#endif

private:
    void Init();
    float dispKp, dispKi, dispKd;
    float *Setpoint,*Input, *Output;
    float Kp, Ki, Kd;
    bool Direction;
    unsigned long lastTime, samplePeriod;
    float iTerm, lastInput;
    float outMin, outMax;
    bool isAuto;

};

#endif  /* CONTROL_H */
