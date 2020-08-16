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

//--------------------------------------------------------------------------
using namespace daisysp;
using namespace daisy;

static DaisyPod pod;

bool freeze = false;
bool demoMode = true;
bool stopSound = false;

float oldk1, oldk2, k1, k2;

//--------------------------------------------------------------------------

void ConditionalParameter(float oldVal, float newVal, float &param,
		float update);

//--------------------------------------------------------------------------

void Controls();

//--------------------------------------------------------------------------

static void AudioCallback(float *in, float *out, size_t size) {
	Controls();

	float yL, yR;

		// for (size_t i = 0; i < size; i += 2) {
		// 	computeSound(&yL, &yR);

		// 	// left out
		// 	out[i] = yL;

		// 	// right out
		// 	out[i + 1] = yR;
		// }

	if (!stopSound) {
		for (size_t i = 0; i < size; i += 2) {
			computeSound(&yL, &yR);

			// left out
			out[i] = yL;

			// right out
			out[i + 1] = yR;
		}
	} else {
		for (size_t i = 0; i < size; i += 2) {
			// left out
			out[i] = 0;

			// right out
			out[i + 1] = 0;
		}
	}
}

//**************************************************************************

int main(void) {
	// Set global variables
	oldk1 = oldk2 = 0;
	k1 = k2 = 0;

	//Init everything
	pod.Init();
	//sample_rate = pod.AudioSampleRate();

	/* Initialize the on-board random number generator ! */
	randomGen_init();
	/* dekrispator synth init  */
	Synth_Init();

	// start callback
	pod.StartAdc();
	pod.StartAudio(AudioCallback);

	while (1) {
	}
}

//Updates values if knob had changed
void ConditionalParameter(float oldVal, float newVal, float &param,
		float update) {
	if (abs(oldVal - newVal) > 0.0005) {
		param = update;
	}
}

//Controls Helpers
void UpdateEncoder() {

}

void UpdateKnobs() {
	k1 = pod.knob1.Process();
	k2 = pod.knob2.Process();
}

void UpdateLeds() {
	oldk1 = k1;
	oldk2 = k2;
	pod.led1.Set(freeze, freeze, freeze);
	pod.led2.Set(stopSound, !stopSound, 0);
	pod.UpdateLeds();
}

void UpdateButtons() {
	if (pod.button1.RisingEdge()) {
		freeze = !freeze;
	}
	if (pod.button2.RisingEdge()) {
		stopSound = !stopSound;
	}
}

void Controls() {
	pod.UpdateAnalogControls();
	pod.DebounceControls();
	UpdateEncoder();
	UpdateKnobs();
	UpdateButtons();
	UpdateLeds();
}

/*--------------------------------END ------------------------------------*/

