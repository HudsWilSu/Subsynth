/*
  ==============================================================================

    CustomVoice.h
    Created: 14 May 2021 3:54:14pm
    Author:  Andy

  ==============================================================================
*/

#include <JuceHeader.h>

class CustomVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound*) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numInputChannels);

private:
    // Sine wave oscillator
    juce::dsp::Oscillator<float> sineOsc{ [](float x) { return std::sin(x); } };
    // Square wave oscillator
    juce::dsp::Oscillator<float> sqOsc{ [](float x) { return x < 0.0f ? -1.0f : 1.0f; } };
    // Sawtooth wave oscillator
    juce::dsp::Oscillator<float> sawOsc{ [](float x) { return x / (2 * juce::MathConstants<float>::pi); } };
    // Triangle wave oscillator
    juce::dsp::Oscillator<float> triOsc{ [](float x) {
        if (x <= -juce::MathConstants<float>::pi / 2) {
            return  juce::jmap(x, -juce::MathConstants<float>::pi, -juce::MathConstants<float>::pi / 2, 0.0f, -1.0f);
        }
        else if (x > -juce::MathConstants<float>::pi / 2 && x <= juce::MathConstants<float>::pi / 2) {
            return juce::jmap(x, -juce::MathConstants<float>::pi / 2, juce::MathConstants<float>::pi / 2, -1.0f, 1.0f);
        }
        else {
            return juce::jmap(x, juce::MathConstants<float>::pi / 2, juce::MathConstants<float>::pi, 1.0f, 0.0f);
        }
    } };

    juce::dsp::Gain<float> gain;
    juce::ADSR envelope;
};