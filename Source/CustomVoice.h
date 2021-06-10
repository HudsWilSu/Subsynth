/*
  ==============================================================================

    This file contains the header information for a JUCE 
    SynthesiserVoice.

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
#pragma once

#include "CustomSound.h"
#include <JuceHeader.h>

class CustomVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound*) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numOutputChannels);

    void setADSR (juce::ADSR::Parameters params);
    void setWave (int waveformNum);
    void setGain (double gain);
    void setFilter (int filterNum, float cutoff, float resonance);

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
