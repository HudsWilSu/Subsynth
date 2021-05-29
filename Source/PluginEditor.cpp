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
    setSize (800, 565);

    waveSelect.addItem("Sine", 1);
    waveSelect.addItem("Square", 2);
    waveSelect.addItem("Saw", 3);
    waveSelect.addItem("Triangle", 4);
    waveSelect.setSelectedId(1);
    
    filterSelect.addItem("Low Pass", 1);
    filterSelect.addItem("Band Pass", 2);
    filterSelect.addItem("High Pass", 3);
    filterSelect.onChange = [this] { filterChanged(); };
    filterSelect.setSelectedId(1);

    filterCutoff.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    filterCutoff.setRange(20.0f, 20000.0f, 0.01f);
    filterCutoff.setValue(10000.0f);
    filterCutoff.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterCutoff.setPopupDisplayEnabled(true, true, this);
    filterCutoff.onDragEnd = [this] { filterChanged(); };
    
    filterRes.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    filterRes.setRange(1.0f, 5.0f, 0.1f);
    filterRes.setValue(2.0f);
    filterRes.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterRes.setPopupDisplayEnabled(true, true, this);
    filterRes.onDragEnd = [this] { filterChanged(); };



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
    p.wfVisualiser.setBounds(10, 360, getWidth() - 20, 200); // add to resized() below - figure out how to access p there?

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
    if (combobox == &(waveSelect)) {
        audioProcessor.changeWaveform(combobox->getSelectedId());
    }
}

void SubsynthAudioProcessorEditor::mouseDrag(const juce::MouseEvent &event) {
    audioProcessor.changeADSREnv(adsrSliders.getEnvelope());
}

void SubsynthAudioProcessorEditor::filterChanged() {
    audioProcessor.changeFilter(filterSelect.getSelectedId(), filterCutoff.getValue(), filterRes.getValue());
}

//==============================================================================
void SubsynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Main title
    g.setColour (juce::Colours::white);
    g.setFont (25.0f);

    g.drawFittedText("Subsynth", 0, 0, getWidth(), 30, juce::Justification::centred, 1);

    // Component Titles
    g.setFont(20.0f);

    g.drawText("Wave", 10, 25, 90, 30, juce::Justification::centred);
    g.drawText("Filter", 110, 25, 100, 30, juce::Justification::centred);
    g.drawText("ADSR Envelope", 220, 25, 400, 30, juce::Justification::centred);

    // Sub-component Titles
    g.setFont(15.0f);

    g.drawText("Cutoff", 110, 75, 100, 30, juce::Justification::centred);
    g.drawText("Resonance", 110, 115, 100, 30, juce::Justification::centred);
}

void SubsynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // 
    // sets the position and size of the slider with arguments (x, y, width, height)
    //freqSlide.setBounds(40, 30, 20, getHeight() - 60);
    //freqLabel.setBounds(10, 10, 90, 20);
    waveSelect.setBounds(10, 55, 90, 20);
    keyboard.setBounds(10, 200, getWidth() - 20, 150);
    filterSelect.setBounds(110, 55, 100, 20);
    filterCutoff.setBounds(110, 85, 100, 50);
    filterRes.setBounds(110, 125, 100, 50);

    // ADSR Components
    //adsrSliders.setBounds(150, 50, 400, 100);
    adsrSliders.setBounds(220, 55, 400, 100);
}
