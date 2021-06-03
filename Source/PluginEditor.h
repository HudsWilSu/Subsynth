/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

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

    void filterChanged();
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    SubsynthAudioProcessor& audioProcessor;

    // UI elements
    juce::Slider freqSlide;
//    juce::La bel freqLabel;
    juce::ComboBox waveSelect;

    juce::ComboBox filterSelect;
    juce::Slider filterCutoff;
    juce::Slider filterRes;
    
    // ADSR Envelope Components
    ADSRComponent adsrSliders;

    // Keyboard
    juce::MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubsynthAudioProcessorEditor)
};
