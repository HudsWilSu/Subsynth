/*
  ==============================================================================

    WfVisualiser.h
    Created: 21 May 2021 11:44:04am
    Author:  Aaron

  ==============================================================================
*/

#include <JuceHeader.h>
#pragma once

class WaveformVisualiser : public juce::AudioVisualiserComponent {
public:
    // constructor
    WaveformVisualiser() : AudioVisualiserComponent(2) // number of channels getTotalNumInputChannels()
    {
        setBufferSize(512); // buffer size
        setSamplesPerBlock(2); // samples per block
        setColours(juce::Colours::blueviolet, juce::Colours::lightgoldenrodyellow);
    }
};