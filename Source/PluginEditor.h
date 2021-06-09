/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

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

    void sliderValueChanged (juce::Slider* slider) override;
    void comboBoxChanged (juce::ComboBox* combobox) override;
    void mouseDrag (const juce::MouseEvent& event) override;
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
