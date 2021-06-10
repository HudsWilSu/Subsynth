/*
  ==============================================================================

    This file contains the header information for a JUCE plugin editor.

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

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ADSRComponent.h"
#include "WfVisualiser.h"

//==============================================================================
/**
*/
class SubsynthAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener, private juce::ComboBox::Listener
{
public:
    SubsynthAudioProcessorEditor (SubsynthAudioProcessor&);
    ~SubsynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox * combobox) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    
    void setupGain();
    
    void filterChanged();
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SubsynthAudioProcessor& audioProcessor;

    // UI elements
    juce::ComboBox waveSelect;

    juce::ComboBox filterSelect;
    juce::Slider filterCutoff;
    juce::Slider filterRes;
    
    // ADSR Envelope Components
    ADSRComponent adsrSliders;
    // Gain slider and label
    juce::Slider gainSlide;
    juce::Label gainLabel;
    // Keyboard
    juce::MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubsynthAudioProcessorEditor)
};
