/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubsynthAudioProcessorEditor::SubsynthAudioProcessorEditor (SubsynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    freqSlide.setSliderStyle(juce::Slider::LinearBarVertical);
    freqSlide.setRange(220.0f, 880.0f, 220.0f);
    freqSlide.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    freqSlide.setPopupDisplayEnabled(true, true, this);
    freqSlide.setTextValueSuffix(" Hz");
    freqSlide.setValue(440.0f);
    freqSlide.setNumDecimalPlacesToDisplay(0);
    freqLabel.setText("Frequency", juce::dontSendNotification);

    waveSelect.addItem("Sine", 1);
    waveSelect.addItem("Square", 2);
    waveSelect.addItem("Saw", 3);
    waveSelect.addItem("Triangle", 4);
    waveSelect.setSelectedId(1);

    // Expose slider to UI/Editor
    addAndMakeVisible(&freqSlide);
    addAndMakeVisible(&freqLabel);
    addAndMakeVisible(&waveSelect);

    // Add listeners
    freqSlide.addListener(this);
    waveSelect.addListener(this);

}

SubsynthAudioProcessorEditor::~SubsynthAudioProcessorEditor()
{
    // This is a new comment to test GIT
    // This comment is to test branching in VS2019
}

void SubsynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.freqValue = freqSlide.getValue();
}

void SubsynthAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combobox)
{
    audioProcessor.wave = waveSelect.getSelectedId();
}

//==============================================================================
void SubsynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    g.drawFittedText("Lil' Babby's First Plugin", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void SubsynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // 
    // sets the position and size of the slider with arguments (x, y, width, height)
    freqSlide.setBounds(40, 30, 20, getHeight() - 60);
    freqLabel.setBounds(10, 10, 90, 20);
    waveSelect.setBounds(100, 100, 90, 20);
}
