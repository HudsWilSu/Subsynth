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

    waveSelect.addItem("Sine", 1);
    waveSelect.addItem("Square", 2);
    waveSelect.addItem("Saw", 3);
    waveSelect.addItem("Triangle", 4);
    waveSelect.setSelectedId(1);
    
    
    // Expose slider to UI/Editor
    //addAndMakeVisible(&freqSlide);
    //addAndMakeVisible(&freqLabel);
    addAndMakeVisible(&waveSelect);
    addAndMakeVisible(&keyboard);
    addAndMakeVisible(&adsrSliders);
    
    // Add listeners
    //freqSlide.addListener(this);
    waveSelect.addListener(this);
    adsrSliders.addMouseListener(this, true);
}

SubsynthAudioProcessorEditor::~SubsynthAudioProcessorEditor()
{

}

void SubsynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //audioProcessor.freqValue = freqSlide.getValue();
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

void SubsynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // 
    // sets the position and size of the slider with arguments (x, y, width, height)
    //freqSlide.setBounds(40, 30, 20, getHeight() - 60);
    //freqLabel.setBounds(10, 10, 90, 20);
    waveSelect.setBounds(10, 20, 90, 20);
    keyboard.setBounds(10, 200, getWidth() - 20, getHeight() - 200);
    
    // ADSR Components
    adsrSliders.setBounds(150, 50, 400, 100);
}
