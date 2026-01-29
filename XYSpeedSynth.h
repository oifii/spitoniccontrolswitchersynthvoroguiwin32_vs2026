//
//  XYSpeedSynth.h
//  derived from TonicDemo by Morgan Packard on 4/8/13
//  reference: https://groups.google.com/forum/#!forum/tonic_audio_users
//
//  Created by Stephane.Poirier@oifii.org
//  Copyright (c) 2015 Stephane Poirier. All rights reserved.
//
#ifndef XYSpeedSynth_h
#define XYSpeedSynth_h

#include "Tonic.h"

using namespace Tonic;

class XYSpeedSynth : public Synth{

  public:
  XYSpeedSynth(){
    //Generator speed = ControlXYSpeed().x(addParameter("x")).y(addParameter("y")).smoothed(); //original
	Generator speed = ControlXYSpeed().x(addParameter("x", 0.0).min(0.0).max(1.0)).y(addParameter("y", 0.0).min(0.0).max(1.0)).smoothed();
	Generator outputGen = RectWave()
      .freq(100 + 20 * speed)
      .pwm( 0.05 + (SineWave().freq(0.1) + 1) * 0.2)
    * SineWave()
        .freq(1 + 20 * speed)
    >> LPF12().cutoff(100 + 6000 * speed)
    >> StereoDelay(0.1, 0.15).wetLevel(0.1);
    ;
    
    setOutputGen(outputGen);
  }

};


//TONIC_REGISTER_SYNTH(XYSpeedSynth);

#endif
