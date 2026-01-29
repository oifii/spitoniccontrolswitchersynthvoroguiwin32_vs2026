//
//  EventsXznnSynth.h
//  based on TonicDemo
//
//  Created by xznnworldwide@gmail.com
//  Copyright (c) 2015 Carl W.B. Poirier. All rights reserved.
//

#ifndef EventsXznnSynth_h
#define EventsXznnSynth_h

#include "Tonic.h"
#include <functional>

using namespace Tonic;


#define INT_TO_STRING( x ) dynamic_cast< std::ostringstream & >( \
	(std::ostringstream() << std::dec << x)).str()

class EventsXznnSynth : public Synth{

public:
	const int NUM_VOICES = 5;

	EventsXznnSynth(){

		//#if TONIC_HAS_CPP_11
#if 1
		ControlParameter threshold = addParameter("threshold", -12.f).displayName("Threshold (dbFS)").min(-60.f).max(0.f);
		//ControlParameter ratio = addParameter("ratio", 2.0f).displayName("Ratio").min(1.0f).max(64.f).logarithmic(true); //original
		ControlParameter ratio = addParameter("ratio", 5.0f).displayName("Ratio").min(1.0f).max(64.f).logarithmic(true); //spi
		//ControlParameter attack = addParameter("attackTime", 0.001).displayName("Attack Time (s)").min(0.001).max(0.1).logarithmic(true); //original
		ControlParameter attack = addParameter("attackTime", 0.1).displayName("Attack Time (s)").min(0.001).max(0.1).logarithmic(true); //spi
		//ControlParameter release = addParameter("releaseTime", 0.05).displayName("Release Time (s)").min(0.01).max(0.08).logarithmic(true); //original
		ControlParameter release = addParameter("releaseTime", 0.01).displayName("Release Time (s)").min(0.01).max(0.08).logarithmic(true); //spi
		ControlParameter gain = addParameter("gain", 0.f).displayName("Makeup Gain (dbFS)").min(0.f).max(36.0f);
		ControlParameter bypass = addParameter("bypass", false).parameterType(ControlParameterTypeToggle); //original

		Generator allVoices;

		Generator noise = PinkNoise(); //original
		//Generator noise = 0.5* SineWave().freq(110) + 0.5* SineWave().freq(220); //spi
		//Generator noise = SineWave().freq(220); //spi
		//Generator noise = SawtoothWave().freq(220); //spi

		vector<float> scale;
		scale.push_back(0);
		scale.push_back(2);
		scale.push_back(4);
		scale.push_back(7);
		scale.push_back(11);

		for (int i = 0; i < NUM_VOICES; i++){


			////////////////////////////////////////////
			// Create the synthesis network
			////////////////////////////////////////////

			Generator voice;

			// Send a trigger message immediately, don't wait for the first beat to happen with the metrenome 
			ControlTrigger initialTrigger;
			initialTrigger.trigger();
			//ControlGenerator resetTrigger = initialTrigger + ControlMetro().bpm(ofRandom(10, 15)); //original - does not compile
			ControlGenerator resetTrigger = initialTrigger + ControlMetro().bpm(ControlRandom().min(10).max(15));

			ControlGenerator noiseTrigger = ControlMetro().bpm(ControlRandom().min(50).max(200).trigger(resetTrigger));

			ControlGenerator pulseLen = ControlRandom().min(0.1).max(0.5).trigger(resetTrigger);

			//ControlGenerator pulse = ControlPulse().length(pulseLen).input(noiseTrigger); //original - does not compile
			ControlGenerator pulse = ControlPulse().length(pulseLen).trigger(noiseTrigger);

			Generator env = ADSR(0.01, 0, 0.5, 0.01).decay(pulseLen * 0.5).trigger(pulse); //original
			//Generator env = ADSR(0.01, 0, 0.05, 0.01).decay(pulseLen * 0.5).trigger(pulse);

			voice = noise * env; //original
			//voice = noise ;

			ControlGenerator highPassAmount = ControlRandom().min(0).max(1).trigger(resetTrigger);

			ControlSnapToScale scaleSnapper = ControlSnapToScale().setScale(scale);
			scaleSnapper.input(40 + highPassAmount * 50); //original
			//scaleSnapper.input(10 + highPassAmount * 50); //spi
			ControlGenerator filterFreq = ControlMidiToFreq().input(scaleSnapper); //original
			//ControlGenerator filterFreq = ControlMidiToFreq().input(40 + highPassAmount * 50); //spi, bypass snap to scale
			ControlGenerator q = ControlRandom().min(0.5).max(1).trigger(resetTrigger);
			q = q * q * 50;

			//voice = HPF24().cutoff(filterFreq).input(voice).Q(q); //original
			voice = LPF24().cutoff(filterFreq).input(voice).Q(q); //spi

			float pan = (2 * (float)i / NUM_VOICES) - 1;
			voice = MonoToStereoPanner().pan(pan).input(voice) * (15.0 / NUM_VOICES);


			Compressor compressor = Compressor()
				.attack(attack)
				.release(0.06)
				.threshold(ControlDbToLinear().input(threshold))
				.ratio(ratio)
				.lookahead(0.001)
				.makeupGain(ControlDbToLinear().input(gain))
				.bypass(bypass)
				.input(voice);

			//allVoices = allVoices + voice;
			allVoices = allVoices + compressor;


		}

		//Generator allvoicesPlusReverb = Reverb().input(allVoices).density(1).dryLevel(0).wetLevel(0.2).decayTime(1); //original
		Generator allvoicesPlusReverb = Reverb().input(allVoices).density(1).dryLevel(0).wetLevel(0.2).decayTime(1); //original
		//Generator allvoicesPlusReverb = Reverb().input(allVoices).density(1).dryLevel(0).wetLevel(0.9).decayTime(1); //spi

		Compressor compressor2 = Compressor()
			.attack(attack)
			.release(0.06)
			.threshold(ControlDbToLinear().input(-2.0))
			.ratio(10.0)
			.lookahead(0.001)
			.makeupGain(ControlDbToLinear().input(gain))
			.bypass(bypass)
			.input(allvoicesPlusReverb);


		// set the synth's final output generator
		//setOutputGen(allVoices + allvoicesPlusReverb);
		setOutputGen(allVoices + compressor2);

#else

#pragma message("C++11 Support not available - EventsXznnSynth disabled")

#endif
	}
};

TONIC_REGISTER_SYNTH(EventsXznnSynth)

#endif
