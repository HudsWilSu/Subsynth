/*
  ==============================================================================

    CustomVoice.cpp
    Created: 14 May 2021 3:54:14pm
    Author:  Andy

  ==============================================================================
*/

#include "CustomVoice.h"

bool CustomVoice::canPlaySound(juce::SynthesiserSound*) {
    return true;
}

void CustomVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    // midi input here
    sineOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    envelope.noteOn();
}

void CustomVoice::stopNote(float velocity, bool allowTailOff) {
    envelope.noteOff();
}

void CustomVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void CustomVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void CustomVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numInputChannels) {
    // Create a spec to hold prep info for oscillator
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numInputChannels;

    envelope.setSampleRate(sampleRate);
    sineOsc.prepare(spec);
    sqOsc.prepare(spec);
    sawOsc.prepare(spec);
    triOsc.prepare(spec);
    gain.prepare(spec);

    //sineOsc.setFrequency(freqValue);
    //sqOsc.setFrequency(freqValue);
    //sawOsc.setFrequency(freqValue);
    //triOsc.setFrequency(freqValue);
    gain.setGainLinear(0.1f); // should be between 0 and 1
}

// adsr functions
juce::ADSR::Parameters CustomVoice::setADSRParams(float att, float dec, float sus, float rel) {
    juce::ADSR::Parameters params = {
        att,
        dec,
        sus,
        rel
    };
    
    return params;
}

void CustomVoice::setAttack(float sliderVal) {
    // set attack value
    params.attack = sliderVal;
}

void CustomVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) {
    // ALL AUDIO PROCESSING CODE HERE
    //juce::dsp::Oscillator<float>* osc;

    //if (wave == 1) {
    //    osc = &sineOsc;
    //}
    //else if (wave == 2) {
    //    osc = &sqOsc;
    //}
    //else if (wave == 3) {
    //    osc = &sawOsc;
    //}
    //else {
    //    osc = &triOsc;
    //}

    //// Check slider for changes
    //osc->setFrequency(freqValue);
    //// Alias to chunk of audio buffer
    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    // ProcessContextReplacing will fill audioBlock with processed data
    sineOsc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    params = setADSRParams(params.attack, 0.1f, 0.1f, 1.0f);
    envelope.setParameters(params);

    envelope.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}
