//-------------------------------------------------------------------------------------
//
//        Dekrispator for Daisy pod platform
//        by Xavier Halgand
//
//*************************************************************************************




#include "daisysp.h"
#include "daisy_pod.h"

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "math_tools.h"
#include "random.h"
#include "sequencer.h"
#include "oscillators.h"
#include "delay.h"
#include "chorusFD.h"
#include "phaser.h"
#include "sinetable.h"
#include "notesTables.h"
#include "resonantFilter.h"
#include "adsr.h"
#include "blepvco.h"
#include "soundGen.h"

using namespace daisysp;
using namespace daisy;

static DaisyPod pod;

static Parameter pitchParam, cutoffParam, lfoParam;
bool freeze = false;
bool demoMode = true;

int wave, mode;

float oldk1, oldk2, k1, k2;
bool selfCycle;

void ConditionalParameter(float oldVal, float newVal, float &param, float update);

void Controls();

static void AudioCallback(float *in, float *out, size_t size)
{
    Controls();
    //float	 	y = 0;
	float	 	yL, yR ;
	//float 		f1;
    
    for (size_t i = 0; i < size; i += 2)
    {
       computeSound(&yL, &yR);
	
	// left out
	out[i] = yL;
	
	// right out
	out[i+1] = yR;
    }
}

int main(void)
{
    // Set global variables
    float sample_rate;
    mode = 0;
   
    oldk1 = oldk2 = 0;
    k1 = k2 = 0;
   
    selfCycle = false;
    
    //Init everything
    pod.Init();
    sample_rate = pod.AudioSampleRate();

    randomGen_init();/* Initialize the on-board random number generator ! */
    Synth_Init();

       //set parameter parameters
   /*  cutoffParam.Init(pod.knob1, 100, 20000, cutoffParam.LOGARITHMIC);
    pitchParam.Init(pod.knob2, 50, 5000, pitchParam.LOGARITHMIC);
    lfoParam.Init(pod.knob1, 0.25, 1000, lfoParam.LOGARITHMIC); */
    
    // start callback
    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    while(1) {}
}

//Updates values if knob had changed
void ConditionalParameter(float oldVal, float newVal, float &param, float update)
{
    if (abs(oldVal - newVal) > 0.0005)
    {
        param = update;
    }
}


//Controls Helpers
void UpdateEncoder()
{
    wave += pod.encoder.RisingEdge();
   

    //skip ramp since it sounds like saw
    if (wave == 3)
    {  
        wave = 4;
    }

   
    
    mode += pod.encoder.Increment();
    mode = (mode % 3 + 3) % 3;
}

void UpdateKnobs()
{
    k1 = pod.knob1.Process();
    k2 = pod.knob2.Process();

  
}

void UpdateLeds()
{
    pod.led1.Set(mode == 2, mode == 1, mode == 0);
    pod.led2.Set(0, selfCycle, selfCycle);
    
    oldk1 = k1;
    oldk2 = k2;
    
    pod.UpdateLeds();
}

void UpdateButtons()
{
    if (pod.button2.RisingEdge() )
    {
  
    }

    if (pod.button1.RisingEdge())
    {
        freeze = !freeze;
    }
}

void Controls()
{
    pod.UpdateAnalogControls();
    pod.DebounceControls();
    
    UpdateEncoder();

    UpdateKnobs();

    UpdateLeds();

    UpdateButtons();
}
