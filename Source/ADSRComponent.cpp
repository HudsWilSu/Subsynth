/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 15 May 2021 1:13:56pm
    Author:  Robin Su

  ==============================================================================
*/

#include "ADSRComponent.h"
#include "PluginEditor.h"

using juce::roundToInt;

ADSRWheel::ADSRWheel()
{
    setRotaryStyle();
    addAndMakeVisible (&rotary);
    addAndMakeVisible (&rotaryLabel);
}

ADSRWheel::ADSRWheel (const std::string& sliderName, ADSR_Element elem)
{
    setRotaryStyle();
    rotaryLabel.setText (sliderName, juce::dontSendNotification);
    element = elem;
}

ADSRWheel::ADSRWheel (const ADSRWheel& oldObj)
{
    this->rotary.setRotaryParameters (oldObj.getParams());
    this->rotaryLabel.setText (oldObj.getLabelText(), juce::dontSendNotification);
}

ADSRWheel& ADSRWheel::operator= (ADSRWheel& oldObj)
{
    this->rotary.setRotaryParameters (oldObj.getParams());
    this->rotaryLabel.setFont (getWidth());
    this->rotaryLabel.setText (oldObj.getLabelText(), juce::dontSendNotification);

    return *this;
}

// Gets parameters of the associated rotary element
//
// @return The rotary parameters of its start and end angle
juce::Slider::RotaryParameters ADSRWheel::getParams() const
{
    return rotary.getRotaryParameters();
}

// Gets the text of the associated rotary's label
//
// @return The rotary's label text
juce::String ADSRWheel::getLabelText() const
{
    return rotaryLabel.getText();
}

// Gets the value of the rotary slider
//
// @return The value of the rotary slider
double ADSRWheel::getValue()
{
    return rotary.getValue();
}

// Gets the type that this slider represents (attack, decay,
// sustain, release)
//
// @return Which ADSR element this rotary represents
ADSR_Element ADSRWheel::getType()
{
    return this->element;
}

// Sets the rotary slider's style
void ADSRWheel::setRotaryStyle()
{
    rotary.setSliderStyle (juce::Slider::Rotary);
    rotary.setRotaryParameters (juce::MathConstants<float>::pi + 0.5, (juce::MathConstants<float>::pi * 3) - 0.5, true);
    rotary.setVelocityBasedMode (true);
    rotary.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    rotary.setPopupDisplayEnabled (true, true, nullptr);
    rotary.setValue (0.1f);
    rotary.setRange (0.0f, 1.0f, 0.01f);

    rotaryLabel.attachToComponent (&rotary, false);
    rotaryLabel.setJustificationType (juce::Justification::centred);
}

// Sets the dimensions of the ADSRWheel object. Typically called
// when the components's width or height changes.
void ADSRWheel::resized()
{
    float width = getWidth();
    rotaryLabel.setFont (roundToInt(0.1700 * width));
    rotary.setBounds (roundToInt(0.1000 * width), roundToInt(0.3000 * width), roundToInt(0.7500 * width), roundToInt(0.7500 * width));
}

//========================================================//
ADSRComponent::ADSRComponent()
{
    setSize (100, 4000);

    ADSRWheel newAttRotary ("Attack", attack);
    ADSRWheel newDecRotary ("Decay", decay);
    ADSRWheel newSusRotary ("Sustain", sustain);
    ADSRWheel newRelRotary ("Release", release);

    attackRotary = newAttRotary;
    decayRotary = newDecRotary;
    sustainRotary = newSusRotary;
    releaseRotary = newRelRotary;

    addAndMakeVisible (&attackRotary);
    addAndMakeVisible (&decayRotary);
    addAndMakeVisible (&sustainRotary);
    addAndMakeVisible (&releaseRotary);

    attackRotary.rotary.addListener (this);
    decayRotary.rotary.addListener (this);
    sustainRotary.rotary.addListener (this);
    releaseRotary.rotary.addListener (this);
}

ADSRComponent::ADSRComponent (ADSRComponent&)
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
}

// Sets the dimensions of the ADSRComponent object's children. 
// Typically called when the components's width or height changes.
void ADSRComponent::resized()
{
    float width = getWidth();
    
    attackRotary.setBounds (roundToInt(0.0000 * width), roundToInt(0.0000 * width), roundToInt(0.2500 * width), roundToInt(0.2500 * width));
    decayRotary.setBounds (roundToInt(0.2500 * width), roundToInt(0.0000 * width), roundToInt(0.2500 * width), roundToInt(0.2500 * width));
    sustainRotary.setBounds (roundToInt(0.5000 * width), roundToInt(0.0000 * width), roundToInt(0.2500 * width), roundToInt(0.2500 * width));
    releaseRotary.setBounds (roundToInt(0.7500 * width), roundToInt(0.0000 * width), roundToInt(0.2500 * width), roundToInt(0.2500 * width));
}

// Listens for changes on the `slider` parameter and sets 
// the appropriate ADSR rotary's value.
//
// @param slider: A slider object that is triggering the change event
void ADSRComponent::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &(attackRotary.rotary))
    {
        attVal = attackRotary.getValue();
    }
    else if (slider == &(decayRotary.rotary))
    {
        decVal = decayRotary.getValue();
    }
    else if (slider == &(sustainRotary.rotary))
    {
        susVal = sustainRotary.getValue();
    }
    else
    {
        relVal = releaseRotary.getValue();
    }
}

// Gets the values from each of the children attack, decay, sustain, 
// release ADSRWheels.
//
// @return The values bundled in a ADSR::Parameters object.
juce::ADSR::Parameters ADSRComponent::getEnvelope()
{
    juce::ADSR::Parameters params {
        attVal,
        decVal,
        susVal,
        relVal
    };

    return params;
}
