#include "mbed-drivers/mbed.h"
#include "minar/minar.h"
#include "core-util/FunctionPointer.h"
#include "Sample.h"
#include "Control.h"

using namespace mbed::util;

Sample<float> P0Samples(100);
Sample<float> P1Samples(100);
Control MyControl(0.5f);
static DigitalOut MyLED(LED1);
AnalogIn Pot0(A0);
AnalogIn Pot1(A1);
static DigitalOut Input1(D4);
static DigitalOut Input2(D2);
PwmOut MyPwm(D5);
static Serial pc(USBTX,USBRX);
//static float Tol = 0.05;
unsigned int Tol = 0x0020;

static void blinky_event(void) {
    MyLED = !MyLED;
    pc.printf("                                                 \r");
}

static void pwm_toggle(void){
    if(MyPwm==0.0f){
        MyPwm = 0.5f;
    } else {
        MyPwm = 0.0f;
    }
}

static void pwm_toggle_irq(void){
    minar::Scheduler::postCallback(FunctionPointer0<void>(&pwm_toggle).bind());
}

static void l293d_event(void){
    float p0 = Pot0.read();
    float p1 = Pot1.read();
    
    MyControl.setSetpoint(P0Samples.Update(p0));//update setpoint

    float Correction = MyControl.Update(P1Samples.Update(p1));//Update correction
    pc.printf("\t Correction: 0.%3d Setpoint: 0.%3d Input: 0.%3d \r", (int)(Correction*1000),(int)(p0*1000),(int)(p0*1000));
    
    if((Correction <= 0.01)&&(Correction >= -0.01)){
        pc.printf("[..]\r");
        Input1 =0; 
        Input2 =0;
        return;
    }
    if(Correction > 0.01){
        pc.printf("[.>>\r");
        Input1 = 1;
        Input2 = 0;
        return;
    }
    if (Correction < -0.01){
        pc.printf("<<.]\r");
        Input1 = 0;
        Input2 = 1;
        return;
    }
    
    /*
    //Should be three cases
    
    //1. p0 > p1 - tolerance 
    if(p0 > p1-Tol){
        Input1 =1;
        Input2 =0;
    }
    
    //2. p0 is < p1 +/- tolerance 
    if(p0 < p1+Tol){
        Input1 = 0;
        Input2 = 1;
    }
    //3. p0 is within +/- tolerance of p1
    if((p0 > p1-Tol)&&(p0 < p1+Tol)){
        Input1 = 0;
        Input2 = 0;
    } 
    */ 
}

void app_start(int, char**){
    MyControl.setKonstants(0.001f,0.0f,0.0f);
    MyPwm.period_ms(1);
    static InterruptIn UserButton(USER_BUTTON);
    UserButton.rise(&pwm_toggle_irq);
    minar::Scheduler::postCallback(l293d_event).period(minar::milliseconds(10));
    minar::Scheduler::postCallback(blinky_event).period(minar::milliseconds(666));
}
