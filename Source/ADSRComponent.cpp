/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 15 May 2021 1:13:56pm
    Author:  Robin Su

  ==============================================================================
*/

#include "ADSRComponent.h"

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
    this->rotaryLabel.setText (oldObj.getLabelText(), juce::dontSendNotification);

    return *this;
}

juce::Slider::RotaryParameters ADSRWheel::getParams() const
{
    return rotary.getRotaryParameters();
}

juce::String ADSRWheel::getLabelText() const
{
    return rotaryLabel.getText();
}

double ADSRWheel::getValue()
{
    return rotary.getValue();
}

ADSR_Element ADSRWheel::getType()
{
    return this->element;
}

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

void ADSRWheel::resized()
{
    rotary.setBounds (10, 30, 75, 75);
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

void ADSRComponent::resized()
{
    attackRotary.setBounds (0, 0, 100, 100);
    decayRotary.setBounds (100, 0, 100, 100);
    sustainRotary.setBounds (200, 0, 100, 100);
    releaseRotary.setBounds (300, 0, 100, 100);
}

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
