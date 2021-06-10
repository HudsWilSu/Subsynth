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

#include "ADSRComponent.h"
#include "PluginProcessor.h"
#include "WfVisualiser.h"
#include <JuceHeader.h>

//==============================================================================
/**
*/
class SubsynthAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     private juce::Slider::Listener,
                                     private juce::ComboBox::Listener
{
public:
    SubsynthAudioProcessorEditor (SubsynthAudioProcessor&);
    ~SubsynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
 
private:
    // sets the initial width of the plug-in window, with other components dynamically sized
    int width = 850;

    void sliderValueChanged (juce::Slider*) override;
    void comboBoxChanged (juce::ComboBox*) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void setGainStyle();
    void filterChanged();

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
