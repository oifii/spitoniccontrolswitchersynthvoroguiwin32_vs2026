//
//  BandlimitedOscillatorExpSynth.h
//  derived from TonicDemo by Nick Donaldson on 7/21/13
//  reference: https://groups.google.com/forum/#!forum/tonic_audio_users
//
//  Created by Stephane.Poirier@oifii.org
//  Copyright (c) 2015 Stephane Poirier. All rights reserved.
//

#ifndef BandlimitedOscillatorExpSynth_h
#define BandlimitedOscillatorExpSynth_h

#include "Tonic.h"

using namespace Tonic;

class BandlimitedOscillatorExpSynth : public Synth
{

public:

	BandlimitedOscillatorExpSynth()
	{
		//ControlParameter blend = addParameter("blend").min(0).max(1);
		ControlParameter blend = addParameter("blend", 0.5f).min(0).max(1);

		//Generator freqSweep = SineWave().freq(0.25) * 1000 + 1080; //original
		//Generator freqSweep = SineWave().freq(1) * 50 + 100;
		Generator freqSweep = SineWave().freq(5) * 50 + 100;
		//Generator smoothBlend = blend.smoothed(); //original
		Generator smoothBlend = blend.smoothed(.05);

		//Generator output = (SquareWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SquareWaveBL().freq(freqSweep) * smoothBlend); //original
		//Generator output = (SineWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SineWave().freq(freqSweep) * smoothBlend);
		//Generator output = (TriangleWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SquareWaveBL().freq(freqSweep) * smoothBlend);
		Generator output = (SquareWave().freq(80) * (1.0 - smoothBlend)) + (SquareWave().freq(freqSweep) * smoothBlend); 
		setOutputGen(output * 0.25);
	}

};

//TONIC_REGISTER_SYNTH(BandlimitedOscillatorExpSynth);

#endif