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

    //freqSlide.setSliderStyle(juce::Slider::LinearBarVertical);
    //freqSlide.setRange(220.0f, 880.0f, 220.0f);
    //freqSlide.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    //freqSlide.setPopupDisplayEnabled(true, true, this);
    //freqSlide.setTextValueSuffix(" Hz");
    //freqSlide.setValue(440.0f);
    //freqSlide.setNumDecimalPlacesToDisplay(0);
    //freqLabel.setText("Frequency", juce::dontSendNotification);

    waveSelect.addItem("Sine", 1);
    waveSelect.addItem("Square", 2);
    waveSelect.addItem("Saw", 3);
    waveSelect.addItem("Triangle", 4);
    waveSelect.setSelectedId(1);
    
    setRotaryStyle(&attackRotary);
    setRotaryStyle(&decayRotary);
    setRotaryStyle(&sustainRotary);
    setRotaryStyle(&releaseRotary);
    attackLabel.setText("Attack", juce::dontSendNotification);
    decayLabel.setText("Decay", juce::dontSendNotification);
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    releaseLabel.setText("Release", juce::dontSendNotification);

    // Expose slider to UI/Editor
    //addAndMakeVisible(&freqSlide);
    //addAndMakeVisible(&freqLabel);
    addAndMakeVisible(&waveSelect);
    addAndMakeVisible(&keyboard);
    
    addAndMakeVisible(&attackRotary);
    addAndMakeVisible(&attackLabel);
    
    // Add listeners
    //freqSlide.addListener(this);
    waveSelect.addListener(this);
    attackRotary.addListener(this);
}

SubsynthAudioProcessorEditor::~SubsynthAudioProcessorEditor()
{

}

void SubsynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //audioProcessor.freqValue = freqSlide.getValue();
    audioProcessor.changeAttack(slider->getValue());
}

//void SubsynthAudioProcessorEditor::sliderValueChanged(juce::Slider* attRotary)
//{
//
//    audioProcessor.changeVolume(attRotary.getValue());
//}

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

    g.drawFittedText("Subsynth", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void SubsynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // 
    // sets the position and size of the slider with arguments (x, y, width, height)
    //freqSlide.setBounds(40, 30, 20, getHeight() - 60);
    //freqLabel.setBounds(10, 10, 90, 20);
    waveSelect.setBounds(10, 20, 90, 20);
    keyboard.setBounds(10, 150, getWidth() - 20, getHeight() - 150);
    
    // ADSR Components
    attackRotary.setBounds(200, 10, 100, 100);
    attackLabel.setBounds(200, 10, 50, 50);
//    attackRotary.setBounds(300, 0, 100, 100);
//    attackLabel.setBounds(300, 110, 50, 50);
//    attackRotary.setBounds(400, 0, 100, 100);
//    attackLabel.setBounds(400, 210, 50, 50);
//    attackRotary.setBounds(500, 0, 100, 100);
//    attackLabel.setBounds(500, 310, 50, 50);
}

void SubsynthAudioProcessorEditor::setRotaryStyle(juce::Slider* rotary)
{
    rotary->setSliderStyle(juce::Slider::Rotary);
    rotary->setRotaryParameters(0.0, juce::MathConstants<float>::twoPi, true);
    rotary->setVelocityBasedMode(true);
    rotary->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    rotary->setPopupDisplayEnabled(true, true, nullptr);
    rotary->setValue(0.1f);
    rotary->setRange(0.0f, 1.0f, 0.01f);
}
