/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "CustomVoice.h"
#include "WfVisualiser.h"
#include <JuceHeader.h>

//==============================================================================
/**
*/
class SubsynthAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    SubsynthAudioProcessor();
    ~SubsynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double, int) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout&) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int) override {};
    const juce::String getProgramName (int) override { return {}; };
    void changeProgramName (int, const juce::String&) override {};

    //==============================================================================
    void getStateInformation (juce::MemoryBlock&) override {};
    void setStateInformation (const void*, int) override {};

    //====== UI Component Callbacks ================================================
    void changeADSREnv(juce::ADSR::Parameters);
    void changeWaveform(int);
    void changeVolume(double);
    void changeFilter(int, double, double);
    
    void runTests();
    //==============================================================================
    // Public vars
    juce::MidiKeyboardState keyState;

    // Waveform Visualizer
    WaveformVisualiser wfVisualiser;

private:
    //==============================================================================
    juce::Synthesiser synth;
    int numVoices = 3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubsynthAudioProcessor)
};
