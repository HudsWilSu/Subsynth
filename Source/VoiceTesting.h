/*
  ==============================================================================

    VoiceTesting.h
    Created: 4 Jun 2021 3:46:22pm
    Author:  Robin Su

  ==============================================================================
*/

#pragma once
#include "CustomVoice.h"

class VoiceTesting : public juce::UnitTest
{
public:
    
    VoiceTesting();
    void runTest() override;
    void filterTest();
    
private:
    juce::AudioBuffer<float> audioBufferActual;
    juce::AudioBuffer<float> audioBufferExpected;
    CustomVoice testVoice;
    // Sine wave oscillator
    juce::dsp::Oscillator<float> sineOsc{ [](float x) { return std::sin(x); } };
    
};
