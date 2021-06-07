/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SubsynthAudioProcessorEditor::SubsynthAudioProcessorEditor (SubsynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboard (audioProcessor.keyState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    ///setSize (1000, 300);
    setSize (850, 565);

    setGainStyle();

    waveSelect.addItem ("Sine", 1);
    waveSelect.addItem ("Square", 2);
    waveSelect.addItem ("Saw", 3);
    waveSelect.addItem ("Triangle", 4);
    waveSelect.setSelectedId (1);

    filterSelect.addItem ("Low Pass", 1);
    filterSelect.addItem ("Band Pass", 2);
    filterSelect.addItem ("High Pass", 3);
    filterSelect.onChange = [this]
    { filterChanged(); };
    filterSelect.setSelectedId (1);

    filterCutoff.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    filterCutoff.setRange (20.0f, 20000.0f, 1.0f);
    filterCutoff.setSkewFactorFromMidPoint (5000);
    filterCutoff.setValue (10000.0f);
    filterCutoff.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    filterCutoff.setPopupDisplayEnabled (true, true, this);
    filterCutoff.onDragEnd = [this]
    { filterChanged(); };
    filterCutoff.setTextValueSuffix (" Hz");

    filterRes.setSliderStyle (juce::Slider::SliderStyle::LinearHorizontal);
    filterRes.setRange (1.0f, 5.0f, 0.1f);
    filterRes.setValue (2.0f);
    filterRes.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    filterRes.setPopupDisplayEnabled (true, true, this);
    filterRes.onDragEnd = [this]
    { filterChanged(); };

    // Expose slider to UI/Editor
    addAndMakeVisible (&waveSelect);
    addAndMakeVisible (&keyboard);
    addAndMakeVisible (&adsrSliders);
    addAndMakeVisible (&gainSlide);
    addAndMakeVisible (&gainLabel);
    addAndMakeVisible (&filterSelect);
    addAndMakeVisible (&filterCutoff);
    addAndMakeVisible (&filterRes);

    // Add listeners
    waveSelect.addListener (this);
    filterSelect.addListener (this);
    filterCutoff.addMouseListener (this, true);
    filterRes.addMouseListener (this, true);
    adsrSliders.addMouseListener (this, true);
    gainSlide.addListener (this);

    // Waveform Visualiser
    addAndMakeVisible (&p.wfVisualiser);

    // Setup color scheme of interactive elements
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colours::blueviolet);
    getLookAndFeel().setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::lightgoldenrodyellow);
    getLookAndFeel().setColour (juce::Slider::trackColourId, juce::Colours::lightgoldenrodyellow);
}

SubsynthAudioProcessorEditor::~SubsynthAudioProcessorEditor()
{
}

void SubsynthAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &gainSlide)
        audioProcessor.changeVolume (gainSlide.getValue());
}

void SubsynthAudioProcessorEditor::comboBoxChanged (juce::ComboBox* combobox)
{
    if (combobox == &(waveSelect))
    {
        audioProcessor.changeWaveform (combobox->getSelectedId());
    }
}

void SubsynthAudioProcessorEditor::mouseDrag (const juce::MouseEvent& event)
{
    audioProcessor.changeADSREnv (adsrSliders.getEnvelope());
}

void SubsynthAudioProcessorEditor::filterChanged()
{
    audioProcessor.changeFilter (filterSelect.getSelectedId(), filterCutoff.getValue(), filterRes.getValue());
}

//==============================================================================
void SubsynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Main title
    g.setColour(juce::Colours::darkcyan);
    g.drawLine(0.0f, 0.0f, getWidth(), 0.0f, 60);
    g.setColour (juce::Colours::white);
    g.setFont (25.0f);
    g.drawFittedText ("Subsynth", 0, 0, getWidth(), 30, juce::Justification::centred, 1);

    // Component Titles
    g.setFont (20.0f);
    g.drawText ("Wave", 10, 30, 90, 30, juce::Justification::centred);
    g.drawText ("Filter", 110, 30, 100, 30, juce::Justification::centred);
    g.drawText ("ADSR Envelope", 220, 30, 400, 30, juce::Justification::centred);
    g.drawText ("Gain", 700, 30, 100, 30, juce::Justification::centred);

    // Sub-component Titles
    g.setFont (15.0f);
    g.drawText ("Cutoff", 110, 80, 100, 30, juce::Justification::centred);
    g.drawText ("Resonance", 110, 125, 100, 30, juce::Justification::centred);
}

void SubsynthAudioProcessorEditor::resized()
{
    // sets the position and size of the slider with arguments (x, y, width, height)
    
    // Wave Selector
    waveSelect.setBounds (10, 60, 90, 20);
    
    // Keyboard
    keyboard.setBounds (10, 205, getWidth() - 20, 150);
    
    // Filter Components
    filterSelect.setBounds (110, 60, 100, 20);
    filterCutoff.setBounds (110, 95, 100, 50);
    filterRes.setBounds (110, 140, 100, 50);

    // Waveform Visualiser
    audioProcessor.wfVisualiser.setBounds (10, 360, getWidth() - 20, 200);

    // ADSR Components
    adsrSliders.setBounds (220, 55, 400, 100);

    // Gain Slider
    gainSlide.setBounds (700, 65, 100, 100);
}

// establish GUI configuration for gain rotary
void SubsynthAudioProcessorEditor::setGainStyle()
{
    gainSlide.setSliderStyle (juce::Slider::Rotary);
    gainSlide.setRotaryParameters (juce::MathConstants<float>::pi + 0.5 , (juce::MathConstants<float>::pi * 3) - 0.5, true);
    gainSlide.setVelocityBasedMode (true);
    gainSlide.onValueChange = [this]
    { gainSlide.setValue (gainSlide.getValue(), juce::dontSendNotification); };
    gainSlide.setSkewFactor (2.0);
    gainSlide.setRange (juce::Range<double> (-50.0, 0.0), 2.0);
    gainSlide.setPopupDisplayEnabled (true, true, this);
    gainSlide.setValue (-25.0);
    gainSlide.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    gainSlide.setTextValueSuffix (" dB");
    adsrSliders.setBounds (220, 55, 400, 100);
}
