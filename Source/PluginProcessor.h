/*
  ==============================================================================

    This file contains the header information for a JUCE plugin processor.

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
    void changeADSREnv (juce::ADSR::Parameters);
    void changeWaveform (int);
    void changeVolume (double);
    void changeFilter (int, double, double);

    void runTests();
    //==============================================================================
    // Public vars
    juce::MidiKeyboardState keyState;

    // Waveform Visualizer
    WaveformVisualiser wfVisualiser;

private:
    //==============================================================================
    juce::Synthesiser synth;
    int numVoices = 6;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubsynthAudioProcessor)
};
