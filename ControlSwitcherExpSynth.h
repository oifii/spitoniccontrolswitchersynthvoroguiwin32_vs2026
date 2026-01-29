//
//  ControlSwitcherExpSynth.h
//  based on a TonicDemo
//
//  Created by Stephane Poirier on 2015dec02.
//  Copyright (c) 2015 Stephane Poirier. All rights reserved.
//

#ifndef ControlSwitcherExpSynth_h
#define ControlSwitcherExpSynth_h

#include "Tonic.h"
#include <functional>

using namespace Tonic;

class ControlSwitcherExpSynth : public Synth{

public:
	ControlSwitcherExpSynth(){

		//#if TONIC_HAS_CPP_11
#if 1
		//spi, begin
		//ControlMetro metro = ControlMetro().bpm(80 * 4); //original
		ControlParameter bpmparam = addParameter("bpm", 320).min(1).max(1000);
		ControlMetro metro = ControlMetro().bpm(bpmparam);
		//spi, end
		ControlGenerator modeSwitch = ControlMetro().bpm(4);
		//ControlGenerator modeSwitch = ControlMetro().bpm(4 * 4);

		//const int numSteps = 4;
		const int numSteps = 16;
		//const int numSteps = 100;

		ControlGenerator phraseStart = ControlRandom().min(0).max(5).trigger(modeSwitch);

		ControlGenerator phraseLen = ControlRandom().min(3).max(11).trigger(modeSwitch);

		ControlStepper step = ControlStepper()
			.start(phraseStart)
			.end(phraseStart + phraseLen - 1)
			.trigger(metro);

		ControlGenerator clickVol = addParameter("clickVol", 0.0).min(0.0).max(1.0);
		ADSR clickEnv = ADSR(0, 0.01, 0, 0).trigger(metro);
		Generator clickIntensity = (1 + clickVol).smoothed();
		//Generator click = SineWave().freq(300 + 1000 * clickEnv) * clickEnv * 0.03; //original
		Generator click = SineWave().freq(300 + 1000 * clickEnv) * clickEnv * 10 * clickVol;

		ControlSwitcher fmAmount = ControlSwitcher().inputIndex(step);
		ControlSwitcher sustain = ControlSwitcher().inputIndex(step);
		ControlSwitcher decay = ControlSwitcher().inputIndex(step);
		ControlSwitcher spreadSeq = ControlSwitcher().inputIndex(step);

		for (int i = 0; i < numSteps; i++){
			fmAmount.addInput(randomFloat(0, 10));
			sustain.addInput(randomFloat(0, 0.7));
			decay.addInput(randomFloat(0.03, 0.1));
			spreadSeq.addInput(randomFloat(0, 0.5));
		}
		//spi, begin
		ControlParameter freqparam = addParameter("freq", 50).min(10).max(1000);
		//Generator freq = ControlValue(50).smoothed(); //original
		Generator freq = freqparam.smoothed();
		//spi, end
		Generator tremelo = 1 + (SineWave().freq(15) *  ADSR(0, 0.5, 0, 0).trigger(modeSwitch));
		Generator bassEnv = ADSR(0.001, 0.1, 0, 0).decay(decay).legato(true).sustain(sustain * sustain).trigger(metro);
		//Generator bassEnv = ADSR(0.1, 0.1, 0, 0).decay(decay).legato(true).sustain(sustain * sustain).trigger(metro);
		ControlGenerator spread = ControlRandom().min(0).max(0.5).trigger(modeSwitch);// * spreadSeq;
		ControlGenerator wave = ControlRandom().min(0.4).max(0.9).trigger(modeSwitch);

		auto makeBass = [&](Generator bassFreq){
			return RectWave()
				.pwm(wave + 0.04 * bassEnv)
				.freq(
				bassFreq
				+ bassFreq
				* SineWave() //original
				//* SawtoothWave() //spi
				//* RectWave() //spi
				.freq(bassFreq * 1.99)
				* (
				fmAmount
				* 0.7
				//+ addParameter("addtlFM") //original
				+ addParameter("addtlFM", 0.0).min(0.0).max(10.0) //spi
				).smoothed()
				);
		};

		Generator bass =
			(
			makeBass(freq)
			>> MonoToStereoPanner().pan(-1 * spread)
			)
			+
			(
			makeBass(freq * 1.02) //original
			//makeBass(freq * 1.10) //spi
			>> MonoToStereoPanner().pan(1 * spread)
			)

			//>> HPF12().cutoff(addParameter("hpf") * 1000)
			>> LPF12().cutoff(5500) //original
			//>> LPF12().cutoff(11000)
			//>> StereoDelay(1.1, 1.2).wetLevel(0.1); //original
		    >> StereoDelay(1.1, 1.2).wetLevel(0.8); //spi
		Generator bassWithAmp = bass * bassEnv * tremelo;
		setOutputGen(bassWithAmp + click); //original
		//setOutputGen(bassWithAmp); 
		//setOutputGen(click);
#else

#pragma message("C++11 Support not available - ControlSwitcherTestSynth disabled")

#endif
	}
};

//TONIC_REGISTER_SYNTH(ControlSwitcherExpSynth)

#endif
