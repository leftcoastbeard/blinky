/* 
 * File:   Control.h
 * Author: C0320318
 *
 * Created on October 28, 2014, 11:20 AM
 */

#ifndef CONTROL_H
#define CONTROL_H
#include "mbed-drivers/mbed.h"

class Control {
public:
    Control(float setpoint);
    Control(const Control& orig);
    virtual ~Control();
    float update(float input);
    //void update_isr(void);
    void setKonstants(float Kp, float Ki, float Kd);
    void setSetpoint(float setpoint);
#if __cplusplus == 201103L
    constexpr static const float dt= 0.001;
#else
    static const float dt= 0.001;
#endif
    
private:
    
    float setpoint;
    float measured;
    float integral;
    float derivative;
    float error;
    float prev_error;
    float Kp, Ki, Kd;
    float input, output;
};

#endif  /* CONTROL_H */