/*
  ==============================================================================

    This file contains the implementation details for a JUCE plugin editor.

    Copyright (C) 2021  Andrew Wilson, Robin Su, Aaron Hudson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

using juce::roundToInt;

//==============================================================================
SubsynthAudioProcessorEditor::SubsynthAudioProcessorEditor (SubsynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboard (audioProcessor.keyState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Set size of plugin and styling of interactive components
    setSize (width, roundToInt(0.665f * width));

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

    // Expose interactive elements to UI/Editor
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

// Listens for changes on the `slider` parameter and sets
// the Gain rotary's value.
//
// @param slider: A slider object that is triggering the change event
void SubsynthAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &gainSlide)
        audioProcessor.changeVolume (gainSlide.getValue());
}

// Listens for changes on the `combobox` parameter and sets
// the changeWaveform method in the AudioProcessor.
//
// @param slider: A combobox object that is triggering the change event
void SubsynthAudioProcessorEditor::comboBoxChanged (juce::ComboBox* combobox)
{
    if (combobox == &(waveSelect))
    {
        audioProcessor.changeWaveform (combobox->getSelectedId());
    }
}

// Listens for mouse clicks and drags and calls the changeADSREnv method
// at the end of the event.
//
// @param event: A mouse event triggering the change
void SubsynthAudioProcessorEditor::mouseDrag (const juce::MouseEvent&)
{
    audioProcessor.changeADSREnv (adsrSliders.getEnvelope());
}

// Calls the changeFilter method in the AudioProcessor to set filter type,
// cutoff frequency, and resonance value.
void SubsynthAudioProcessorEditor::filterChanged()
{
    audioProcessor.changeFilter (filterSelect.getSelectedId(), filterCutoff.getValue(), filterRes.getValue());
}

// Draws the content of the method on the GUI
//
// @param g: The graphics context that must be used to do the drawing operations.
void SubsynthAudioProcessorEditor::paint (juce::Graphics& g)
{   
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Main title
    g.setColour(juce::Colours::darkcyan);
    g.drawLine((0.0000f * width), (0.0000f * width), (1.0000f * width), (0.0000f * width), (0.0706f * width));
    g.setColour (juce::Colours::white);
    g.setFont (0.0294f * width);
    g.drawFittedText ("Subsynth", roundToInt(0.0000 * width), roundToInt(0.0000 * width), roundToInt(1.0000 * width), roundToInt(0.0353 * width), juce::Justification::centred, 1);

    // Component Titles
    g.setFont (0.0235f * width);
    g.drawText ("Wave", roundToInt(0.0618 * width), roundToInt(0.0353 * width), roundToInt(0.1059 * width), roundToInt(0.0353 * width), juce::Justification::centred);
    g.drawText ("Filter", roundToInt(0.1894 * width), roundToInt(0.0353 * width), roundToInt(0.1176 * width), roundToInt(0.0353 * width), juce::Justification::centred);
    g.drawText ("ADSR Envelope", roundToInt(0.3298 * width), roundToInt(0.0353 * width), roundToInt(0.4706 * width), roundToInt(0.0353 * width), juce::Justification::centred);
    g.drawText ("Gain", roundToInt(0.8235 * width), roundToInt(0.0353 * width), roundToInt(0.1176 * width), roundToInt(0.0353 * width), juce::Justification::centred);

    // Sub-component Titles
    g.setFont (0.0176f * width);
    g.drawText ("Cutoff", roundToInt(0.1894 * width), roundToInt(0.0941 * width), roundToInt(0.1176 * width), roundToInt(0.0353 * width), juce::Justification::centred);
    g.drawText ("Resonance", roundToInt(0.1894 * width), roundToInt(0.1471 * width), roundToInt(0.1176 * width), roundToInt(0.0353 * width), juce::Justification::centred);
}

// Sets the dimensions of the plug-in's top level children.
// Typically called when the plug-in's width or height changes.
void SubsynthAudioProcessorEditor::resized()
{
    // sets the position and size of the slider with arguments (x, y, width, height)
    
    // dynamically obtain Width for resizing purposes
    width = getWidth();

    // Wave Selector
    waveSelect.setBounds (roundToInt(0.0618 * width), roundToInt(0.0706 * width), roundToInt(0.1059 * width), roundToInt(0.0235 * width));
    
    // Keyboard
    keyboard.setBounds (roundToInt(0.0118 * width), roundToInt(0.2412 * width), roundToInt(0.9765 * width), roundToInt(0.1765 * width));
    
    // Filter Components
    filterSelect.setBounds (roundToInt(0.1894 * width), roundToInt(0.0706 * width), roundToInt(0.1176 * width), roundToInt(0.0235 * width));
    filterCutoff.setBounds (roundToInt(0.1894 * width), roundToInt(0.1118 * width), roundToInt(0.1176 * width), roundToInt(0.0588 * width));
    filterRes.setBounds (roundToInt(0.1894 * width), roundToInt(0.1647 * width), roundToInt(0.1176 * width), roundToInt(0.0588 * width));

    // Waveform Visualiser
    audioProcessor.wfVisualiser.setBounds (roundToInt(0.0118 * width), roundToInt(0.4235 * width), roundToInt(0.9765 * width), roundToInt(0.2353 * width));

    // ADSR Components
    adsrSliders.setBounds (roundToInt(0.3298 * width), roundToInt(0.0647 * width), roundToInt(0.4706 * width), roundToInt(0.1176 * width));

    // Gain Slider
    gainSlide.setBounds (roundToInt(0.8235 * width), roundToInt(0.0588 * width), roundToInt(0.1176 * width), roundToInt(0.1176 * width));
}

// Establishes GUI configuration for gain rotary
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
