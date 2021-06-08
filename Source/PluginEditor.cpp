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
    
    setSize (width, 0.665 * width);

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
    g.drawLine(0.0000 * width, 0.0000 * width, 1.0000 * width, 0.0000 * width, 0.0706 * width);
    g.setColour (juce::Colours::white);
    g.setFont (0.0294 * width);
    g.drawFittedText ("Subsynth", 0.0000 * width, 0.0000 * width, 1.0000 * width, 0.0353 * width, juce::Justification::centred, 1);

    // Component Titles
    g.setFont (0.0235 * width);
    g.drawText ("Wave", 0.0618 * width, 0.0353 * width, 0.1059 * width, 0.0353 * width, juce::Justification::centred);
    g.drawText ("Filter", 0.1894 * width, 0.0353 * width, 0.1176 * width, 0.0353 * width, juce::Justification::centred);
    g.drawText ("ADSR Envelope", 0.3298 * width, 0.0353 * width, 0.4706 * width, 0.0353 * width, juce::Justification::centred);
    g.drawText ("Gain", 0.8235 * width, 0.0353 * width, 0.1176 * width, 0.0353 * width, juce::Justification::centred);

    // Sub-component Titles
    g.setFont (0.0176 * width);
    g.drawText ("Cutoff", 0.1894 * width, 0.0941 * width, 0.1176 * width, 0.0353 * width, juce::Justification::centred);
    g.drawText ("Resonance", 0.1894 * width, 0.1471 * width, 0.1176 * width, 0.0353 * width, juce::Justification::centred);
}

void SubsynthAudioProcessorEditor::resized()
{
    // sets the position and size of the slider with arguments (x, y, width, height)
    
    // dynamically obtain Width for resizing purposes
    width = getWidth();

    // Wave Selector
    waveSelect.setBounds (0.0618 * width, 0.0706 * width, 0.1059 * width, 0.0235 * width);
    
    // Keyboard
    keyboard.setBounds (0.0118 * width, 0.2412 * width, 0.9765 * width, 0.1765 * width);
    
    // Filter Components
    filterSelect.setBounds (0.1894 * width, 0.0706 * width, 0.1176 * width, 0.0235 * width);
    filterCutoff.setBounds (0.1894 * width, 0.1118 * width, 0.1176 * width, 0.0588 * width);
    filterRes.setBounds (0.1894 * width, 0.1647 * width, 0.1176 * width, 0.0588 * width);

    // Waveform Visualiser
    audioProcessor.wfVisualiser.setBounds (0.0118 * width, 0.4235 * width, 0.9765 * width, 0.2353 * width);

    // ADSR Components
    adsrSliders.setBounds (0.3298 * width, 0.0647 * width, 0.4706 * width, 0.1176 * width);

    // Gain Slider
    gainSlide.setBounds (0.8235 * width, 0.0588 * width, 0.1176 * width, 0.1176 * width);
}

// establish GUI configuration for gain rotary
void SubsynthAudioProcessorEditor::setGainStyle()
{
    // dynamically obtain width for resizing purposes
    width = getWidth();

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
}
