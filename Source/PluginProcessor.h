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
    // Sawtooth wave oscillator, lambda return provides 2 breakpoints for
    juce::dsp::Oscillator<float> sawOsc { [](float x)
        {
            // https://docs.juce.com/master/group__juce__core-maths.html#ga8acdd3d518517bd5e3c0bd1922218bf9
            // Map the range between -Pi .. Pi to -1 .. 1 providing a linear ramp from -1 to 1
            return juce::jmap (
                x,
                -juce::MathConstants<float>::pi,
                juce::MathConstants<float>::pi,
                (float)-1,
                (float)1
                );
        }
    };

    juce::dsp::Gain<float> gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubsynthAudioProcessor)
};
