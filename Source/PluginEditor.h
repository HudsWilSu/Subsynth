/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
    void setAttackRotary(juce::Slider*);
    void setRotaryStyle(juce::Slider* attackRotary);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SubsynthAudioProcessor& audioProcessor;

    // UI elements
    //juce::Slider freqSlide;
    //juce::Label freqLabel;
    juce::ComboBox waveSelect;
    
    // ADSR Envelope Components
    juce::Slider attackRotary;
    juce::Slider decayRotary;
    juce::Slider sustainRotary;
    juce::Slider releaseRotary;
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;

    // Keyboard
    juce::MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubsynthAudioProcessorEditor)
};
