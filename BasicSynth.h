//
//  BasicSynth.h
//  based on TonicDemo
//
//  Created by Stephane.Poirier@oifii.org
//  Copyright (c) 2015 Stephane Poirier. All rights reserved.
//

#ifndef BasicSynth_h
#define BasicSynth_h

#include "Tonic.h"
#include <functional>

using namespace Tonic;

class BasicSynth : public Synth{

public:
	BasicSynth(){

//#if TONIC_HAS_CPP_11
#if 1
		// create a new oscillator which we'll use for the actual audio signal
		SineWave tone = SineWave();

		// create a sine wave we'll use for some vibrato
		SineWave vibratoOsc = SineWave();
		vibratoOsc.freq(10);

		// you can use the regular arithmatic operators on Generators and their subclasses (SineWave extends Generator)
		float basePitch = 400;
		Generator frequency = basePitch + (vibratoOsc * basePitch * 0.01);

		// plug that frequency generator into the frequency slot of the main audio-producing sine wave
		tone.freq(frequency);

		// let's also create a tremelo effect
		SineWave tremeloSine = SineWave().freq(1);

		// set the synth's final output generator
		setOutputGen(tone * tremeloSine);

#else

#pragma message("C++11 Support not available - ControlSwitcherTestSynth disabled")

#endif
	}
};

TONIC_REGISTER_SYNTH(BasicSynth)

#endif
