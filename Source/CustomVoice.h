/*
  ==============================================================================

    CustomVoice.h
    Created: 14 May 2021 3:54:14pm
    Author:  Andy

  ==============================================================================
*/
#pragma once

#include "CustomSound.h"
#include <JuceHeader.h>

class CustomVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound*) override;
    void startNote (int, float, juce::SynthesiserSound*, int) override;
    void stopNote (float, bool) override;
    void pitchWheelMoved (int) override {};
    void controllerMoved (int, int) override {};
    void renderNextBlock (juce::AudioBuffer<float>&, int, int) override;
    void prepareToPlay (double, int, int);

    void setADSR (juce::ADSR::Parameters);
    void setWave (int);
    void setGain (double);
    void setFilter (int, double, double);

    double sampleRateHolder = 0;
    
    void voiceTests();

private:
    juce::dsp::Oscillator<float>* osc;
    // Sine wave oscillator
    juce::dsp::Oscillator<float> sineOsc { [] (float x)
                                           { return std::sin (x); } };
    // Square wave oscillator
    juce::dsp::Oscillator<float> sqOsc { [] (float x)
                                         { return x < 0.0f ? -1.0f : 1.0f; } };
    // Sawtooth wave oscillator
    juce::dsp::Oscillator<float> sawOsc { [] (float x)
                                          { return x / (2 * juce::MathConstants<float>::pi); } };
    // Triangle wave oscillator
    juce::dsp::Oscillator<float> triOsc { [] (float x)
                                          {
                                              if (x <= -juce::MathConstants<float>::pi / 2)
                                              {
                                                  return juce::jmap (x, -juce::MathConstants<float>::pi, -juce::MathConstants<float>::pi / 2, 0.0f, -1.0f);
                                              }
                                              else if (x > -juce::MathConstants<float>::pi / 2 && x <= juce::MathConstants<float>::pi / 2)
                                              {
                                                  return juce::jmap (x, -juce::MathConstants<float>::pi / 2, juce::MathConstants<float>::pi / 2, -1.0f, 1.0f);
                                              }
                                              else
                                              {
                                                  return juce::jmap (x, juce::MathConstants<float>::pi / 2, juce::MathConstants<float>::pi, 1.0f, 0.0f);
                                              }
                                          } };

    juce::dsp::Gain<float> gain;
    juce::ADSR envelope;
    int wave = 1;
    juce::AudioBuffer<float> synthBuffer;
    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>, juce::dsp::StateVariableFilter::Parameters<float>> SVFilter;
};
