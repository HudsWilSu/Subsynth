/*
  ==============================================================================

    CustomVoice.cpp
    Created: 14 May 2021 3:54:14pm
    Author:  Andy

  ==============================================================================
*/

#include "CustomVoice.h"

// Indicates if this voice object is capable of playing the given sound.
//
// @param sound: A SynthesiserSound to be associated with this voice.
// @return True if sound is not a nullptr, else false
bool CustomVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<CustomSound*> (sound) != nullptr;
}

// Starts playing a note. Typically called automatically during the rendering callback.
//
// @param midiNoteNumber: The note to be played represented by its MIDI number.
// @param velocity: A value indicating how quickly the note was released 0 (slow) to 1 (fast).
// @param sound: The SynthesiserSound associated with this voice.
// @param currentPitchWheelPosition: What the pitch wheel position should be for this note.
void CustomVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    // midi input here
    osc->setFrequency (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    envelope.noteOn();
}

// Stops a playing note. Typically called automatically during the rendering callback.
//
// @param velocity: a value indicating how quickly the note was released 0 (slow) to 1 (fast)
// @param allowTailOff: a flag indicating whether a note wants to tail-off or stop immediately.
void CustomVoice::stopNote (float velocity, bool allowTailOff)
{
    envelope.noteOff();

    if (! allowTailOff)
    {
        clearCurrentNote();
    }
}

// A pure virtual function requiring basic implementation for SynthesizerVoice inheritence. NYI.
void CustomVoice::pitchWheelMoved (int newPitchWheelValue)
{
}

// A pure virtual function requiring basic implementation for SynthesizerVoice inheritence. NYI.
void CustomVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
}

// Initializes and configures the various components of the voice which must be
// done before the voice can be used.
//
// @param sampleRate: The sample rate to be used in initialization. Typically the target
// sample rate of the owning object.
// @param samplesPerBlock: the maximum expected number of samples that will be in each rendered block
// @param numOutputChannels: The number of output channels for the plugin
void CustomVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels)
{
    // Create a spec to hold prep info for dsp objects
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;

    sampleRateHolder = sampleRate;

    juce::ADSR::Parameters initADSR {
        0.1f, 0.1f, 0.1f, 0.1f
    };

    SVFilter.reset();
    SVFilter.prepare (spec);
    setFilter (1, 20000.0, 2.0);

    envelope.setSampleRate (sampleRate);
    envelope.setParameters (initADSR);

    sineOsc.prepare (spec);
    sqOsc.prepare (spec);
    sawOsc.prepare (spec);
    triOsc.prepare (spec);

    if (wave == 1)
    {
        osc = &sineOsc;
    }
    else if (wave == 2)
    {
        osc = &sqOsc;
    }
    else if (wave == 3)
    {
        osc = &sawOsc;
    }
    else
    {
        osc = &triOsc;
    }

    gain.prepare (spec);
    setGain (-25.0);
}

// Sets the attack, decay, sustain, release values of the volume envelope.
//
// @param params: A set of attack, decay sustain, release values in an ADSR::Parameters
// object for the volume envelope to be set to.
void CustomVoice::setADSR (juce::ADSR::Parameters parameters)
{
    envelope.reset();
    envelope.setParameters (parameters);
}

// Changes which active oscillator between sine, square, saw, and triangle the
// voice is using.
//
// @param waveformNum: An integer representation for sine, square, saw, and triangle
// waveforms.
void CustomVoice::setWave (int waveformNum)
{
    // set wave value
    wave = waveformNum;
    if (wave == 1)
    {
        osc = &sineOsc;
    }
    else if (wave == 2)
    {
        osc = &sqOsc;
    }
    else if (wave == 3)
    {
        osc = &sawOsc;
    }
    else
    {
        osc = &triOsc;
    }
}

// Sets the filter type, cutoff frequency, and resonance setting for the state
// variable filter data member.
//
// @param filterNum: An integer representation of the filter type to be set low pass,
// band pass, high pass.
// @param cutoff: The cutoff frequency for the state variable filter in Hz.
// @param resonance: The amount of resonance to be applied by the state variable filter
void CustomVoice::setFilter (int filterNum, float cutoff, float resonance)
{
    if (filterNum == 1)
    {
        SVFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;

        SVFilter.state->setCutOffFrequency (sampleRateHolder, cutoff, resonance);
    }

    else if (filterNum == 2)
    {
        SVFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass;

        SVFilter.state->setCutOffFrequency (sampleRateHolder, cutoff, resonance);
    }

    else if (filterNum == 3)
    {
        SVFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;

        SVFilter.state->setCutOffFrequency (sampleRateHolder, cutoff, resonance);
    }
}

// Calls the setGainDecibels method on the gain data member. Self-explanitory.
//
// @param gainVal: the decibel value to be set.
void CustomVoice::setGain (double gainVal)
{
    gain.setGainDecibels (gainVal);
}

// Produces/processes a block of audio samples into the output stream of the plug-in
//
// @param outputBuffer: An AudioBuffer object that will be sent to the output stream
// @param startSample: the index in which the rendered block should be inserted into
// the outputBuffer
// @param numSamples: The amount of samples that need to be rendered.
void CustomVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    // Initialize subset buffer
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    // Alias to chunk of audio buffer
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };

    // ProcessContextReplacing will fill audioBlock with processed data
    osc->process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    SVFilter.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));

    // Apply ADSR to entire subset buffer
    envelope.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples());

    // Add all samples from subset buffer back to output
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom (channel, startSample, synthBuffer, channel, 0, numSamples, 1.0f);

        if (! envelope.isActive())
        {
            clearCurrentNote();
            envelope.reset();
        }
    }
    
}

// Runs a set of unit-style tests related to methods changing DSP 
// component parameters. Must attach a debugger for proper function.
void CustomVoice::voiceTests()
{

    // Test filter
    float cutoff = 20.0f, resonance = 1.0f;
    sampleRateHolder = 48000.0;

    setFilter(1, cutoff, resonance);
    jassert(SVFilter.state->type == juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass);
    setFilter(2, cutoff, resonance);
    jassert(SVFilter.state->type == juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass);
    setFilter(3, cutoff, resonance);
    jassert(SVFilter.state->type == juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass);

    // Test oscillators
    setWave(2);
    jassert(osc == &sqOsc);
    setWave(3);
    jassert(osc == &sawOsc);
    setWave(4);
    jassert(osc == &triOsc);

    // Test ADSR
    const juce::ADSR::Parameters initADSR{
    0.1f, 0.1f, 0.1f, 0.1f
    };

    setADSR(initADSR);
    jassert(envelope.getParameters().attack == initADSR.attack);
    jassert(envelope.getParameters().decay == initADSR.decay);
    jassert(envelope.getParameters().sustain == initADSR.sustain);
    jassert(envelope.getParameters().release == initADSR.release);

    // Test gain
    double initGainDb = -10.0;
    
    setGain(initGainDb);
    jassert(gain.getGainDecibels() == initGainDb);
}
