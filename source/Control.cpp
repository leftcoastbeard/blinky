/* 
 * File:   Control.cpp
 * Author: C0320318
 * 
 * Created on October 28, 2014, 11:20 AM
 */

#include "Control.h"

Control::Control(float setpoint) {
    this->setpoint = setpoint;
    prev_error = 0;
    integral = 0;
    this->setKonstants(0.0f,0.0f,0.0f);
}

//Control::Control(const Control& orig) {
//}

Control::~Control() {
}

float Control::Update(float input){
    error = setpoint-input;
    integral += error*dt;
    derivative = (error-prev_error)/dt;
    prev_error = error;
    return setpoint-((Kp*error) + (Ki*integral) + (Kd*derivative));
}

void Control::setKonstants(float Kp, float Ki, float Kd){
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

void Control::setSetpoint(float setpoint){
    this->setpoint = setpoint;
}

float Control::getSetpoint(void){
    return this->setpoint;
}

/*void Control::update_isr(void){
        
}*/
