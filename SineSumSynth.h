//
//
//  SineSumSynth.h
//  derived from TonicDemo by Nick Donaldson on 2/7/13
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

#ifndef SineSumSynth_h
#define SineSumSynth_h

#define NUM_SINES 10

#include "Tonic.h"

using namespace Tonic;

class SineSumSynth : public Synth{
  
public:
  
  SineSumSynth () {
    
    //ControlParameter pitch = addParameter("pitch",0); //original
	ControlParameter pitchmulti = addParameter("pitchmulti", 0.0).min(0.0).max(10.0); //spi
	ControlParameter pitchbase = addParameter("pitchbase", 110.0).min(20.0).max(10000.0); //spi

    Adder outputAdder;
    
    for (int s=0; s<NUM_SINES; s++){
      
     // ControlGenerator pitchGen = ((pitch * 220 + 220) * powf(2, (s - (NUM_SINES/2)) * 5.0f / 12.0f)); //original
		ControlGenerator pitchGen = ((pitchmulti * pitchbase + pitchbase) * powf(2, (s - (NUM_SINES / 2)) * 5.0f / 12.0f));

      outputAdder.input(SineWave().freq( pitchGen.smoothed() ));
      
    }
    
    Generator outputGen = outputAdder * ((1.0f/NUM_SINES) * 0.5f);
    
    setOutputGen(outputGen);

  }

};

//TONIC_REGISTER_SYNTH(SineSumSynth);

#endif
