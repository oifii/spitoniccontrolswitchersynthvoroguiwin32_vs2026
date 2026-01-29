//
//  StepSequencerSynth.h
//  based on TonicDemo
//
//  Created by Stephane.Poirier@oifii.org
//  Copyright (c) 2015 Stephane Poirier. All rights reserved.
//

#ifndef StepSequencerSynth_h
#define StepSequencerSynth_h

#include "Tonic.h"
#include <functional>

using namespace Tonic;

#define INT_TO_STRING( x ) dynamic_cast< std::ostringstream & >( \
	(std::ostringstream() << std::dec << x)).str()

class StepSequencerSynth : public Synth{

public:
	StepSequencerSynth(){

		//#if TONIC_HAS_CPP_11
#if 1
		const int NUM_STEPS = 8;

		// synth paramters are like instance variables -- they're values you can set later, by
		// cally synth.setParameter()
		ControlGenerator bpm = addParameter("tempo",100).min(50).max(300);
		ControlGenerator transpose = addParameter("transpose", 0).min(-6).max(6);

		// ControlMetro generates a "trigger" message at a given bpm. We multiply it by four because we
		// want four 16th notes for every beat
		ControlGenerator metro = ControlMetro().bpm(4 * bpm);

		// ControlStepper increments a value every time it's triggered, and then starts at the beginning again
		// Here, we're using it to move forward in the sequence
		ControlGenerator step = ControlStepper().end(NUM_STEPS).trigger(metro);

		// ControlSwitcher holds a list of ControlGenerators, and routes whichever one the inputIndex is pointing
		// to to its output.
		ControlSwitcher pitches = ControlSwitcher().inputIndex(step);
		ControlSwitcher cutoffs = ControlSwitcher().inputIndex(step);
		ControlSwitcher glides = ControlSwitcher().inputIndex(step);

		// stick a bunch of random values into the pitch and cutoff lists
		for (int i = 0; i < NUM_STEPS; i++){
			ControlGenerator pitchForThisStep = addParameter("step" + INT_TO_STRING(i) + "Pitch", randomFloat(10, 80)).min(10).max(80);
			pitches.addInput(pitchForThisStep);

			ControlGenerator cutoff = addParameter("step" + INT_TO_STRING(i) + "Cutoff", 500).min(30).max(1500);
			cutoffs.addInput(cutoff);

			ControlGenerator glide = addParameter("step" + INT_TO_STRING(i) + "Glide", 0).min(0).max(0.1);
			glides.addInput(glide);

		}

		// Define a scale according to steps in a 12-note octave. This is a pentatonic scale. Like using
		// just the black keys on a piano
		vector<float> scale;
		scale.push_back(0);
		scale.push_back(2);
		scale.push_back(3);
		scale.push_back(5);
		scale.push_back(7);
		scale.push_back(10);

		// ControlSnapToScale snaps a float value to the nearest scale value, no matter what octave its in
		ControlGenerator midiNote = transpose + ControlSnapToScale().setScale(scale).input(pitches);

		ControlGenerator frequencyInHertz = ControlMidiToFreq().input(midiNote);

		// now that we've done all that, we have a frequency signal that's changing 4x per beat
		Generator tone = RectWave().freq(frequencyInHertz.smoothed().length(glides));

		// create an amplitude signal with an ADSR envelope, and scale it down a little so it's not scary loud
		Generator amplitude = ADSR(0.01, 0.1, 0, 0).trigger(metro) * 0.3;

		// create a filter, and feed the cutoff sequence in to it
		LPF24 filter = LPF24().cutoff(cutoffs).Q(0.1);
		filter.input(tone * amplitude);

		// rout the output of the filter to the synth's main output
		setOutputGen(filter);

#else

#pragma message("C++11 Support not available - ControlSwitcherTestSynth disabled")

#endif
	}
};

TONIC_REGISTER_SYNTH(StepSequencerSynth)

#endif
