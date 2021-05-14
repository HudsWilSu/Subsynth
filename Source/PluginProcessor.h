/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

    //==============================================================================
    // Public vars
    float freqValue = 440.0f;
    int wave = 1;
    

private:
    //==============================================================================
    // Sine wave oscillator
    juce::dsp::Oscillator<float> sineOsc { [](float x) { return std::sin(x); }};
    // Square wave oscillator
    juce::dsp::Oscillator<float> sqOsc{ [](float x) { return x < 0.0f ? -1.0f : 1.0f; } };
    // Sawtooth wave oscillator
    juce::dsp::Oscillator<float> sawOsc{ [](float x) { return x / (2 * juce::MathConstants<float>::pi); } };
    // Triangle wave oscillator
    juce::dsp::Oscillator<float> triOsc{ [](float x) {
        if (x <= -juce::MathConstants<float>::pi/2) {
            return  juce::jmap(x, -juce::MathConstants<float>::pi, -juce::MathConstants<float>::pi/2, 0.0f, -1.0f);
        }
        else if (x > -juce::MathConstants<float>::pi / 2 && x <= juce::MathConstants<float>::pi / 2) {
            return juce::jmap(x, -juce::MathConstants<float>::pi/2, juce::MathConstants<float>::pi/2, -1.0f, 1.0f);
        }
        else {
            return juce::jmap(x, juce::MathConstants<float>::pi/2, juce::MathConstants<float>::pi, 1.0f, 0.0f);
        }
    } };

    juce::dsp::Gain<float> gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubsynthAudioProcessor)
};
