#include "mbed-drivers/mbed.h"
#include "minar/minar.h"
#include "core-util/FunctionPointer.h"

static DigitalOut MyLED(LED1);
static AnalogIn Pot0(A0);
static AnalogIn Pot1(A1);
static DigitalOut Input1(D4);
static DigitalOut Input2(D2);
static PwmOut MyPwm(D5);
static float Tol = 0.01;

static void blinky_event(void) {
 MyLED = !MyLED;
}

static void pwm_toggle(void){
    if(MyPwm()==0.0f){
        MyPwm = 0.5f;
    } else {
        MyPwm = 0.0f;
    }
}

static void pwm_toggle_irq(void){
    minar::Scheduler::postCallback(FunctionPointer0<void>(&pwm_toggle).bind());
}

static void l293d_event(void){
    float p0 = Pot0;
    float p1 = Pot1;
    
    //Should be three cases
    
    //1. p0 > p1 +/- tolerance 
    if((p0 > p1-Tol)&&(p0 > p1+Tol)){
        Input1 =1;
        Input2 =0;
    }
    //2. p0 is within +/- tolerance of p1
    if((p0 > p1-Tol)&&(p0 < p1+Tol)){
        Input1 = 0;
        Input2 = 0;
    }
    //3. p0 is < p1 +/- tolerance 
    if((p0 < p1-Tol)&&(p0 < p1+Tol)){
        Input1 = 0;
        Input2 = 1;
    }
    
    
}

void app_start(int, char**){
    MyPwm.period_ms(100);
    static InterruptIn UserButton(B1);
    UserButton.rise(&pwm_toggle_irq);
    minar::Scheduler::postCallback(l293d_event).period(minar::milliseconds(50));
    minar::Scheduler::postCallback(blinky_event).period(minar::milliseconds(666));
}
