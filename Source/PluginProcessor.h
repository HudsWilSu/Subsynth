/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomVoice.h"
#include "CustomSound.h"
#include "WfVisualiser.h"
#include "Filter.h"

//==============================================================================
/**
*/
class SubsynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SubsynthAudioProcessor();
    ~SubsynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //====== UI Component Callbacks ================================================
    void changeADSREnv(juce::ADSR::Parameters);
    void changeWaveform(int waveformNum);
    void changeFilter(int filterNum);

    //==============================================================================
    // Public vars
    float freqValue = 440.0f;
    juce::MidiKeyboardState keyState;
    
    // Waveform Visualizer
    WaveformVisualiser wfVisualiser;

private:
    //==============================================================================
    juce::Synthesiser synth;
    //juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, struct juce::dsp::IIR::Coefficients<float>> IIRFilter;
    //Filters filters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubsynthAudioProcessor)
};
