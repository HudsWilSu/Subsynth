/*
  ==============================================================================

    CustomVoice.cpp
    Created: 14 May 2021 3:54:14pm
    Author:  Andy

  ==============================================================================
*/

#include "CustomVoice.h"

bool CustomVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<CustomSound*>(sound) != nullptr;
}

void CustomVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    // midi input here
    osc->setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    envelope.noteOn();
}

void CustomVoice::stopNote(float velocity, bool allowTailOff) {
    envelope.noteOff();
    
    if (!allowTailOff) {
        clearCurrentNote();
    }
}

void CustomVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void CustomVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void CustomVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels) {
    // Create a spec to hold prep info for dsp objects
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;

    juce::ADSR::Parameters initADSR{
        0.1f, 0.1f, 0.1f, 0.1f
    };

    envelope.setSampleRate(sampleRate);
    envelope.setParameters(initADSR);
    
    sineOsc.prepare(spec);
    sqOsc.prepare(spec);
    sawOsc.prepare(spec);
    triOsc.prepare(spec);

    if (wave == 1) {
        osc = &sineOsc;
    }
    else if (wave == 2) {
        osc = &sqOsc;
    }
    else if (wave == 3) {
        osc = &sawOsc;
    }
    else {
        osc = &triOsc;
    }
    
    gain.prepare(spec);
    gain.setGainLinear(0.1f); // should be between 0 and 1
}

// set ADSR envelope
void CustomVoice::setADSR(juce::ADSR::Parameters parameters) {
    envelope.reset();
    envelope.setParameters(parameters);
}

void CustomVoice::setWave(int waveformNum) {
    // set wave value
    wave = waveformNum;
    if (wave == 1) {
        osc = &sineOsc;
    }
    else if (wave == 2) {
        osc = &sqOsc;
    }
    else if (wave == 3) {
        osc = &sawOsc;
    }
    else {
        osc = &triOsc;
    }
}

void CustomVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {

    // ALL AUDIO PROCESSING CODE HERE

    // Initialize subset buffer
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // Alias to chunk of audio buffer
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    // ProcessContextReplacing will fill audioBlock with processed data
    osc->process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    // Apply ADSR to entire subset buffer
    envelope.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    // Add all samples from subset buffer back to output
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples, 1.0f);

        if (!envelope.isActive()) {
            clearCurrentNote();
            envelope.reset();
        }
    }
}
