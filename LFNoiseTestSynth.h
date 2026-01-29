//
//
//  LFNoiseTest.h
//  derived from TonicDemo by Morgan Packard on 2/26/13
//  reference: https://groups.google.com/forum/#!forum/tonic_audio_users
//
//  Created by Stephane.Poirier@oifii.org
//  Copyright (c) 2016 Stephane Poirier. All rights reserved.
//
//

//

/*
Because of the fancy TONIC_REGISTER_SYNTH macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using 
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to TONIC_REGISTER_SYNTH.
*/

#ifndef LFNoiseTestSynth_h
#define LFNoiseTestSynth_h

#include "Tonic.h"

using namespace Tonic;

class LFNoiseTestSynth : public Synth{
public:
	LFNoiseTestSynth()
	{
		ControlParameter pitch = addParameter("sinefreq", 500).min(10.0).max(10000.0); //spi

		Generator outputGen = SineWave().freq(
		//500 + 500 * LFNoise().setFreq( //original
		pitch + pitch * LFNoise().setFreq(
		//addParameter("noiseFreq", 100)) //original
		addParameter("noisefreq", 100).min(1.0).max(10000.0)) //spi
		)
		//* 0.3; //original
		*addParameter("vol", 0.3).min(0.0).max(1.0); //spi
    
		setOutputGen(outputGen);
	}
};

//TONIC_REGISTER_SYNTH(LFNoiseTestSynth);


#endif
