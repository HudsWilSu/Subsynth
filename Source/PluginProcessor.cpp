/*
  ==============================================================================

    This file contains the implementation details for a JUCE plugin processor.

    Copyright (C) 2021  Andrew Wilson, Robin Su, Aaron Hudson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubsynthAudioProcessor::SubsynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
#endif
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    // Initialization adapted from tapSynth code by The Audio Programmer
    // https://github.com/TheAudioProgrammer/tapSynth/blob/main/Source/PluginProcessor.cpp

    synth.addSound (new CustomSound());

    for (int i = 0; i < numVoices; i++)
    {
        synth.addVoice (new CustomVoice());
    }
}

SubsynthAudioProcessor::~SubsynthAudioProcessor()
{
}

// Returns the name of this processor.
//
// @return The name of the processor as a String obj.
const juce::String SubsynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

// Returns true if the processor wants MIDI messages.
bool SubsynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

// Returns true if the processor produces MIDI messages.
bool SubsynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

// Returns true if this is a MIDI effect plug-in and does no audio processing.
bool SubsynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

// Returns the length of the processor's tail, in seconds. No tail used (0 seconds).
double SubsynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

// Returns the number of preset programs the processor supports. Always returns at least 1.
int SubsynthAudioProcessor::getNumPrograms()
{
    return 1;
}

// Returns the number of the currently active program. Programs NYI, returns 0.
int SubsynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

//==============================================================================

// Called before playback starts, to let the processor prepare itself.
//
// @param sampleRate: Target sample rate
// @param samplesPerBlock: A strong hint about the maximum number of samples
// that will be provided in each block.
void SubsynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    for (int i = 0; i < synth.getNumVoices(); i++)
        (dynamic_cast<CustomVoice*> (synth.getVoice (i)))->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());

    wfVisualiser.clear();
    runTests();
}

// Called after playback has stopped, to let the object free up any
// resources it no longer needs.
void SubsynthAudioProcessor::releaseResources()
{
    keyState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
// Callback to query if the AudioProcessor supports a specific layout.
//
// @param layouts: The bus layout to check
bool SubsynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

// Renders the next audio block
//
// @param buffer: The buffer obj to use for rendering
// @param midiMessages: The collected MIDI messages associated with this buffer.
void SubsynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (buffer.getNumSamples() == 0)
    {
        return;
    }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    // Scan midi buffer and add any messages generated by onscreen keyboard to buffer
    // injectIndirectEvents bool (last argument) must be true
    keyState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    wfVisualiser.pushBuffer (buffer);
}

//==============================================================================

// Processor subclass must override this and return true if it can create an editor component.
bool SubsynthAudioProcessor::hasEditor() const
{
    return true;
}

// Creates the processor's GUI via creating an AudioProcessorEditor
juce::AudioProcessorEditor* SubsynthAudioProcessor::createEditor()
{
    return new SubsynthAudioProcessorEditor (*this);
}

//============================= UI Callbacks ========================================

// Calls the setADSR CustomVoice method to change the attack, decay, sustain, release
// values of the volume envelope on each voice instantiated within the synth data member.
//
// @param params: A set of attack, decay sustain, release values in an ADSR::Parameters
// object for the volume envelope to be set to.
void SubsynthAudioProcessor::changeADSREnv (juce::ADSR::Parameters params)
{
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        dynamic_cast<CustomVoice*> (synth.getVoice (i))->setADSR (params);
    }
}

// Calls the setWave CustomVoice method to change the waveform being produced by
// the oscillator in each voice of the synth data member.
//
// @param waveformNum: An integer representation for sine, square, saw, and triangle
// waveforms.
void SubsynthAudioProcessor::changeWaveform (int waveformNum)
{
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        dynamic_cast<CustomVoice*> (synth.getVoice (i))->setWave (waveformNum);
    }
}

// Calls the setGain CustomVoice method to change the gain being applied to
// the audio buffer of each voice of the synth data member
//
// @param gain: A number, in decibels, the gain voice data member should be set to.
void SubsynthAudioProcessor::changeVolume (double gain)
{
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        dynamic_cast<CustomVoice*> (synth.getVoice (i))->setGain (gain);
    }
}

// Calls the setFilter CustomVoice method to change the filter type, cutoff frequency,
// and resonance of the state variable filter of each voice in the synth data member
//
// @param filterNum: An integer representation of the filter type to be set; options
// include low pass, band pass, and high pass.
// @param cutoff: The cutoff frequency for the state variable filter in Hz.
// @param resonance: The amount of resonance to be applied by the state variable filter
void SubsynthAudioProcessor::changeFilter (int filterNum, double cutoff, double resonance)
{
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        dynamic_cast<CustomVoice*> (synth.getVoice (i))->setFilter (filterNum, cutoff, resonance);
    }
}

// Runs a set of unit-style tests related to methods changing DSP
// component parameters. Must attach a debugger for proper function.
void SubsynthAudioProcessor::runTests()
{
    CustomVoice testVoice;
    testVoice.voiceTests();
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubsynthAudioProcessor();
}
