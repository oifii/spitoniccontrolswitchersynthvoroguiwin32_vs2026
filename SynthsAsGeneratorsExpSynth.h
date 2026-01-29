//
//  SynthsAsGeneratorsExpSynth.h
//  derived from TonicDemo by Nick Donaldson on 5/23/13
//  reference: https://groups.google.com/forum/#!forum/tonic_audio_users
//
//  Created by Stephane.Poirier@oifii.org
//  Copyright (c) 2015 Stephane Poirier. All rights reserved.
//


#ifndef SynthsAsGeneratorsExpSynth_h
#define SynthsAsGeneratorsExpSynth_h

#include "Tonic.h"

using namespace Tonic;

class SynthsAsGeneratorsExpSynth : public Synth
{
public:

	SynthsAsGeneratorsExpSynth(){

		// Add two synths together and put them both through some delay
		Synth noiseSynth = SynthFactory::createInstance("FilteredNoiseSynth");
		Synth octaverSynth = SynthFactory::createInstance("ControlSnapToScaleExpSynth");

		// get all the control parameters and add them to this synth
		addParametersFromSynth(noiseSynth);
		addParametersFromSynth(octaverSynth);

		//StereoDelay delay = StereoDelay(0.5f,0.5f).delayTimeLeft(0.2).delayTimeRight(0.3).dryLevel(1.0f).wetLevel(0.3f).feedback(0.3f); //original
		StereoDelay delay = StereoDelay(0.5f, 0.5f).delayTimeLeft(0.2).delayTimeRight(0.3).dryLevel(1.0f).wetLevel(0.8f).feedback(0.3f);

		setOutputGen(((noiseSynth * 0.5 + octaverSynth) >> delay) * 0.8);

	}


};

TONIC_REGISTER_SYNTH(SynthsAsGeneratorsExpSynth);

#endif