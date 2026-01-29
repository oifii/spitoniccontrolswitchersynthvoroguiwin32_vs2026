//
//  BandlimitedOscillatorTestSynth.h
//  TonicDemo
//
//  Created by Nick Donaldson on 7/21/13.
//

#ifndef BandlimitedOscillatorTestSynth_h
#define BandlimitedOscillatorTestSynth_h

#include "Tonic.h"

using namespace Tonic;

class BandlimitedOscillatorTestSynth : public Synth
{

public:
  
  BandlimitedOscillatorTestSynth()
  {
    ControlParameter blend = addParameter("blend").min(0).max(1);
  
    Generator freqSweep = SineWave().freq(0.25) * 1000 + 1080;
    Generator smoothBlend = blend.smoothed();
    
    //Generator output = (SquareWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SquareWaveBL().freq(freqSweep) * smoothBlend); //original
	Generator output = (SineWave().freq(freqSweep) * (1.0 - smoothBlend)) + (SquareWaveBL().freq(freqSweep) * smoothBlend);
	setOutputGen(output * 0.25);
  }
  
};

TONIC_REGISTER_SYNTH(BandlimitedOscillatorTestSynth);

#endif