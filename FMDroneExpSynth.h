//
//
//  FMDroneExpSynth.h
//  derived from TonicDemo by Nick Donaldson on 2/15/13
//  reference: https://groups.google.com/forum/#!forum/tonic_audio_users
//
//  Created by Stephane.Poirier@oifii.org
//  Copyright (c) 2015 Stephane Poirier. All rights reserved.
//



/*
Because of the fancy TONIC_REGISTER_SYNTH macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to TONIC_REGISTER_SYNTH.
*/

#ifndef FMDroneExpSynth_h
#define FMDroneExpSynth_h

#include "Tonic.h"

using namespace Tonic;

class FMDroneExpSynth : public Synth {


public:
	FMDroneExpSynth(){

		//ControlParameter volume = addParameter("volume", -12.f).displayName("Volume (dbFS)").min(-60.f).max(0.f); //original
		ControlParameter volume = addParameter("volume", -0.0f).displayName("Volume (dbFS)").min(-60.f).max(0.f); //spi
		//ControlParameter carrierPitch = addParameter("carrierPitch", 28.f).displayName("Carrier Pitch").min(20.f).max(32.f); //original
		ControlParameter carrierPitch = addParameter("carrierPitch", 32.f).displayName("Carrier Pitch").min(20.f).max(64.f); //spi
		//ControlParameter modIndex = addParameter("modIndex", 0.25f).displayName("FM Amount").min(0.f).max(1.0f); //original
		ControlParameter modIndex = addParameter("modIndex", 1.0f).displayName("FM Amount").min(0.f).max(1.0f); //spi
		//ControlParameter lfoAmt = addParameter("lfoAmt", 0.5f).displayName("LFO Amount").min(0.f).max(1.f); //original
		ControlParameter lfoAmt = addParameter("lfoAmt", 1.0f).displayName("LFO Amount").min(0.f).max(1.f); //spi

		Generator rCarrierFreq = ControlMidiToFreq().input(carrierPitch).smoothed();
		Generator rModFreq = rCarrierFreq * 4.0f;
		
		Generator outputGen = SineWave()
			.freq(rCarrierFreq
			+ (
			SineWave().freq(rModFreq) *
			rModFreq *
			(modIndex.smoothed() * (1.0f + SineWave().freq((LFNoise().setFreq(0.5f) + 1.f) * 2.f + 0.2f) * (lfoAmt * 0.5f).smoothed()))
			)
			) * ControlDbToLinear().input(volume).smoothed() * ((SineWave().freq(0.15f) + 1.f) * 0.75f + 0.25);
		/*
		Generator outputGen = TriangleWave()
			.freq(rCarrierFreq
			+ (
			TriangleWave().freq(rModFreq) *
			rModFreq *
			(modIndex.smoothed() * (1.0f + SineWave().freq((LFNoise().setFreq(0.5f) + 1.f) * 2.f + 0.2f) * (lfoAmt * 0.5f).smoothed()))
			)
			) * ControlDbToLinear().input(volume).smoothed() * ((SineWave().freq(0.15f) + 1.f) * 0.75f + 0.25);
		*/
		setOutputGen(outputGen);

	}

};

//TONIC_REGISTER_SYNTH(FMDroneExpSynth);

#endif