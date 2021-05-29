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
    //setSize (1000, 300);
    setSize (800, 450);

    waveSelect.addItem("Sine", 1);
    waveSelect.addItem("Square", 2);
    waveSelect.addItem("Saw", 3);
    waveSelect.addItem("Triangle", 4);
    waveSelect.setSelectedId(1);
    
    filterSelect.addItem("Low Pass", 1);
    filterSelect.addItem("Band Pass", 2);
    filterSelect.addItem("High Pass", 3);
    filterSelect.onChange = [this] { filterSelectChanged(); };
    filterSelect.setSelectedId(1);

    filterCutoff.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoff.setRange(20.0f, 20000.0f);
    filterCutoff.setValue(600.0f);
    filterCutoff.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterCutoff.setPopupDisplayEnabled(true, true, this);

    filterRes.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterRes.setRange(1.0f, 5.0f);
    filterRes.setValue(2.0f);
    filterRes.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterRes.setPopupDisplayEnabled(true, true, this);



    // Expose slider to UI/Editor
    //addAndMakeVisible(&freqSlide);
    //addAndMakeVisible(&freqLabel);
    addAndMakeVisible(&waveSelect);
    addAndMakeVisible(&keyboard);
    addAndMakeVisible(&adsrSliders);
    addAndMakeVisible(&filterSelect);
    addAndMakeVisible(&filterCutoff);
    addAndMakeVisible(&filterRes);
    
    // Add listeners
    //freqSlide.addListener(this);
    waveSelect.addListener(this);
    filterSelect.addListener(this);
    filterCutoff.addMouseListener(this, true);
    filterRes.addMouseListener(this, true);

    // Waveform Visualiser
    addAndMakeVisible(&p.wfVisualiser);
    p.wfVisualiser.setBounds(10, 320, getWidth() - 20, 100); // add to resized() below - figure out how to access p there?

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

void SubsynthAudioProcessorEditor::filterSelectChanged() {
    audioProcessor.changeFilter(filterSelect.getSelectedId());
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
    keyboard.setBounds(10, 160, getWidth() - 20, 150);
    filterSelect.setBounds(110, 20, 90, 20);
    filterCutoff.setBounds(210, 20, 50, 50);
    filterRes.setBounds(270, 20, 50, 50);

    // ADSR Components
    adsrSliders.setBounds(150, 50, 400, 100);
}
