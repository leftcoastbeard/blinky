#include "mbed-drivers/mbed.h"
#include "minar/minar.h"
#include "core-util/FunctionPointer.h"
//#include "Sample.h"
#include "Control.h"

using namespace mbed::util;

/*******************************************************************/
/* This does not function properly or compile properly as an 
 * external file.
 */
template <class a_T = int, int max = 8 >
class Sample
{
public:
    Sample() {
        max_samples = max;
        sample_index = 0;
        //samples = new a_T[MAX];

        for (int i = 0; i < max; i++) {
            samples[i] = 0;
        }

        sum = (a_T) (0);
        current = (a_T) (0);
        average = (a_T) (0);
        //calibrated = (a_T)(0);
    }
    a_T Update(a_T sample_data) {
        current = sample_data;
        sum += current;
        sum -= samples[sample_index];
        samples[sample_index] = current;
        average = sum / max_samples;
        sample_index++;

        if (sample_index == max_samples) sample_index = 0;

        return average;
    }
    a_T GetSum() {
        return sum;
    }
    a_T GetCurrent() {
        return current;
    }
    a_T GetAverage() {
        return average;
    }
    //sample(const sample& orig);
    ~Sample() {}

private:
    a_T sum;
    a_T current;
    a_T samples[max];
    a_T average;
    //a_T calibrated;
    unsigned int sample_index;
    unsigned int max_samples;
};
/*******************************************************************/


PwmOut L293D_Enable(D3);
DigitalOut myled(LED1);
Sample<float, 8> InputSamples;
Sample<float, 32> KonstantSamples;
Sample<float, 32> SetpointSamples;
float input =0.0f, output=0.0f, setpoint=1.0f;
Control MotorControl(&setpoint, &input, &output, 1.0f, 0.0f, 0.0f, DIRECT);
AnalogIn InputPot(A0);
AnalogIn SetpointPot(A1);
AnalogIn KonstantPot(A2);
DigitalOut L293D_Input1(D2);
DigitalOut L293D_Input2(D4);
Serial pc(USBTX,USBRX);
InterruptIn UserButton(USER_BUTTON);
//Ticker UpdateControl;
//Ticker UpdateOutputs;
//float Correction =0.0f;
//It takes ~60% duty cycle to move the motor
//So the PWM threshold should be 60% - the tolerance.
static const float TOL = 0.010f;
static const float PWM_THRESHOLD=0.600f-TOL;
//static float pwm_state = 0.0f;
int output_enable = 0;

static void blinky_event(void) {
    static float temp_setpoint = 0.0f;
    static float temp_konstant = 0.0f;
    temp_setpoint = SetpointPot.read();
    SetpointSamples.Update(2.0f*temp_setpoint);
    setpoint = SetpointSamples.GetAverage();//update setpoint

    temp_konstant = KonstantPot.read();
    KonstantSamples.Update(20.0f*temp_konstant);
    MotorControl.setKonstants(KonstantSamples.GetAverage(),0.0f,0.0f);

    pc.printf("\t Output: %1.3f Setpoint: %1.3f Input: %1.3f Konstant: %1.3f\r", output,SetpointSamples.GetAverage(),InputSamples.GetAverage(),KonstantSamples.GetAverage());
        
    myled = !myled;
    //pc.printf("                                                 \r");
}

void enable_toggle(void)
{
    output_enable = output_enable?0:1;
    wait_ms(10);
}

void pid_kompute_event(void){
    input = InputSamples.GetAverage();
    MotorControl.Kompute();
}

static void pwm_toggle_irq(void){
    minar::Scheduler::postCallback(FunctionPointer0<void>(&enable_toggle).bind());
}

static void l293d_event(void){
    float temp_input = InputPot.read();
    temp_input = InputSamples.Update(2.0f*temp_input);
    
    L293D_Enable =((float)output_enable)*((output>=0.0f?output:-output)+PWM_THRESHOLD);//TODO: Need a better function

    if((output <= TOL)&&(output >= -TOL)) {

        L293D_Input1 =0;
        L293D_Input2 =0;
        return;
    }
    if(output < -TOL) {

        L293D_Input1 = 1;
        L293D_Input2 = 0;
        return;
    }
    if (output > TOL) {

        L293D_Input1 = 0;
        L293D_Input2 = 1;
        return;
    } 
}

void app_start(int, char**){
    //MotorControl.setKonstants(5.0f,1.0f,0.0f);
    MotorControl.setLimits(-1.0f, 1.0f);
    MotorControl.setMode(AUTOMATIC);
    L293D_Enable.period_us(10);
    //L293D_Input2.period_us(10);
    L293D_Enable = PWM_THRESHOLD;
    //L293D_Input2 = PWM_THRESHOLD;
    //UserButton.rise(&enable_toggle);
    //UpdateControl.attach(&pid_kompute_event, (float)(MotorControl.dt*MotorControl.getSamplePeriod()));
    //UpdateOutputs.attach(&l293d_event, 0.001f);
    
    //MyControl.setKonstants(0.001f,0.0f,0.0f);
    //L293D_Enable.period_ms(1);
    //static InterruptIn UserButton(USER_BUTTON);
    UserButton.rise(&pwm_toggle_irq);
    minar::Scheduler::postCallback(pid_kompute_event).period(minar::milliseconds(MotorControl.getSamplePeriod()));
    minar::Scheduler::postCallback(l293d_event).period(minar::milliseconds(1));
    minar::Scheduler::postCallback(blinky_event).period(minar::milliseconds(250));
}
