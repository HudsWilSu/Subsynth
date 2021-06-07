/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 15 May 2021 1:13:56pm
    Author:  Robin Su

  ==============================================================================
*/

#include "ADSRComponent.h"
#include "PluginEditor.h"

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
    rotary.setRotaryParameters (0.0, juce::MathConstants<float>::twoPi, true);
    rotary.setVelocityBasedMode (true);
    rotary.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    rotary.setPopupDisplayEnabled (true, true, nullptr);
    rotary.setValue (0.1f);
    rotary.setRange (0.0f, 1.0f, 0.01f);
    rotaryLabel.attachToComponent (&rotary, false);
}

void ADSRWheel::resized()
{
    float width = getWidth();
    rotary.setBounds (0.1000 * width, 0.3000 * width, 0.7500 * width, 0.7500 * width);
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
    float width = getWidth();
    
    attackRotary.setBounds (0.0000 * width, 0.0000 * width, 0.2500 * width, 0.2500 * width);
    decayRotary.setBounds (0.2500 * width, 0.0000 * width, 0.2500 * width, 0.2500 * width);
    sustainRotary.setBounds (0.5000 * width, 0.0000 * width, 0.2500 * width, 0.2500 * width);
    releaseRotary.setBounds (0.7500 * width, 0.0000 * width, 0.2500 * width, 0.2500 * width);
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
