/*
 * File:   Control.cpp
 * Author: C0320318
 *
 * Created on October 28, 2014, 11:20 AM
 */

#include "Control.h"

Control::Control(float *setpoint, float *input, float *output, float kp, float ki, float kd, bool dir)
{
    Setpoint = setpoint;
    Input = input;
    Output = output;
    Direction = dir;
    samplePeriod = 10;//in ms
    this->setKonstants(kp, ki, kd);
    isAuto = false;
    this->setLimits(0.00f, 1.00f); //defaults corresponds to Nucleo Limits
    this->setDirection(DIRECT);
}

Control::~Control()
{
}

bool Control::Kompute()
{
    if (!isAuto) return false;

    float input = *Input;
    float error = *Setpoint - input;

    iTerm += Ki * error;

    if (iTerm > outMax) iTerm = outMax;
    else if (iTerm < outMin ) iTerm = outMin;

    float delta = (input - lastInput);

    float output = Kp * error + iTerm - Kd * delta;

    if (output > outMax) output = outMax;
    else if (output < outMin) output = outMin;

    *Output = output;

    lastInput = input;
    return true;
}

void Control::setKonstants(float kp, float ki, float kd)
{
    if (kp < 0.0f || ki < 0.0f || kd < 0.0f) return;

    dispKp = kp;
    dispKi = ki;
    dispKd = kd;

    float sample_period_s = samplePeriod * dt;

    Kp = kp;
    Ki = ki * sample_period_s;
    Kd = kd / sample_period_s;

    if (Direction == REVERSE) {
        Kp = (0 - Kp);
        Ki = (0 - Ki);
        Kd = (0 - Kd);
    }
}

void Control::setMode(bool mode)
{
    bool newAuto = (mode == AUTOMATIC);

    if (newAuto == !isAuto) this->Init();

    isAuto = newAuto;
}

void Control::setLimits(float min, float max)
{
    if (min >= max) return;

    outMax = max;
    outMin = min;

    if (isAuto) {
        if (*Output > outMax) *Output = outMax;
        else if (*Output < outMin) *Output = outMin;

        if (iTerm > outMax) iTerm = outMax;
        else if (iTerm < outMin ) iTerm = outMin;
    }
}

void Control::setDirection(bool dir)
{
    if (isAuto && dir != Direction) {
        Kp = (0 - Kp);
        Ki = (0 - Ki);
        Kd = (0 - Kd);
    }

    Direction = dir;
}

void Control::updateSamplePeriod(int ms)
{
    if (ms > 0) {
        float ratio = (float)ms / (float)samplePeriod;
        Ki *= ratio;
        Kd /= ratio;
        samplePeriod = ms;
    }
}

float Control::getKP()
{
    return dispKp;
}

float Control::getKI()
{
    return dispKi;
}

float Control::getKD()
{
    return dispKd;
}

bool Control::getMode()
{
    return isAuto;
}

bool Control::getDirection()
{
    return Direction;
}

unsigned long Control::getSamplePeriod()
{
    return samplePeriod;
}

void Control::Init()
{
    iTerm = *Output;

    if (iTerm > outMax) iTerm = outMax;
    else if (iTerm < outMin ) iTerm = outMin;

    lastInput = *Input;
}
