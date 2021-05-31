/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubsynthAudioProcessorEditor::SubsynthAudioProcessorEditor (SubsynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboard(audioProcessor.keyState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 300);
    setupGain();
    
    waveSelect.addItem("Sine", 1);
    waveSelect.addItem("Square", 2);
    waveSelect.addItem("Saw", 3);
    waveSelect.addItem("Triangle", 4);
    waveSelect.setSelectedId(1);
        
    
    // Expose slider to UI/Editor
    addAndMakeVisible(&waveSelect);
    addAndMakeVisible(&keyboard);
    addAndMakeVisible(&adsrSliders);
    addAndMakeVisible(&gainSlide);
    addAndMakeVisible(&gainLabel);
    
    // Add listeners
    waveSelect.addListener(this);
    adsrSliders.addMouseListener(this, true);
    gainSlide.addListener(this);
}

SubsynthAudioProcessorEditor::~SubsynthAudioProcessorEditor()
{

}

void SubsynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlide)
        audioProcessor.changeVolume(gainSlide.getValue());
}

void SubsynthAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combobox)
{
    audioProcessor.changeWaveform(combobox->getSelectedId());
}

void SubsynthAudioProcessorEditor::mouseDrag(const juce::MouseEvent &event) {
    audioProcessor.changeADSREnv(adsrSliders.getEnvelope());
}
//==============================================================================
void SubsynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    g.drawFittedText("Subsynth", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void SubsynthAudioProcessorEditor::resized() {
    // sets the position and size of the slider with arguments (x, y, width, height)
    waveSelect.setBounds(10, 20, 90, 20);
    keyboard.setBounds(10, 200, getWidth() - 20, getHeight() - 200);
    
    // ADSR Components
    adsrSliders.setBounds(150, 50, 400, 100);
    
    // gain slider
    gainSlide.setBounds(700, 25, 100, 100);
}

// establish GUI configuration for gain rotary
void SubsynthAudioProcessorEditor::setupGain() {
    gainSlide.setSliderStyle(juce::Slider::Rotary);
    gainSlide.setRotaryParameters(juce::MathConstants<float>::pi, (juce::MathConstants<float>::pi * 3), true);
    gainSlide.setVelocityBasedMode(true);
    gainSlide.onValueChange = [this] { gainSlide.setValue(gainSlide.getValue(), juce::dontSendNotification); };
    gainSlide.setSkewFactor(2.0);
    gainSlide.setRange(juce::Range<double>(-50.0, 0.0), 2.0);
    gainSlide.setPopupDisplayEnabled(true, true, nullptr);
    gainSlide.setValue(-25.0)
    gainSlide.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainLabel.setText("Gain(dB)", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlide, true);
}
