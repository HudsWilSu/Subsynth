/*
  ==============================================================================

    CustomSound.h
    Created: 14 May 2021 3:45:21pm
    Author:  Andy

  ==============================================================================
*/

#include <JuceHeader.h>

class CustomSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override { return true; };
    bool appliesToChannel (int midiChannel) override { return true; };
};
