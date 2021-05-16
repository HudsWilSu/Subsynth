/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 15 May 2021 1:13:56pm
    Author:  Robin Su

  ==============================================================================
*/

#include "ADSRComponent.h"

ADSRWheel::ADSRWheel() {
    setRotaryStyle();
    addAndMakeVisible(&rotary);
    addAndMakeVisible(&rotaryLabel);
    rotary.addListener(this);
}

ADSRWheel::ADSRWheel(const std::string& sliderName) {
    setRotaryStyle();
    rotaryLabel.setText(sliderName, juce::dontSendNotification);
    addAndMakeVisible(&rotary);
    addAndMakeVisible(&rotaryLabel);
}

ADSRWheel::ADSRWheel(const ADSRWheel& oldObj) {
    this->rotary.setRotaryParameters(oldObj.getParams());
    this->rotaryLabel.setText(oldObj.getLabelText(), juce::dontSendNotification);
}

ADSRWheel& ADSRWheel::operator=(ADSRWheel& oldObj) {
    this->rotary.setRotaryParameters(oldObj.getParams());
    this->rotaryLabel.setText(oldObj.getLabelText(), juce::dontSendNotification);
    
    return *this;
}

juce::Slider::RotaryParameters ADSRWheel::getParams() const {
    return rotary.getRotaryParameters();
}

juce::String ADSRWheel::getLabelText() const {
    return rotaryLabel.getText();
}

double ADSRWheel::getValue() {
    return rotary.getValue();
}

void ADSRWheel::setRotaryStyle() {
    rotary.setSliderStyle(juce::Slider::Rotary);
    rotary.setRotaryParameters(0.0, juce::MathConstants<float>::twoPi, true);
    rotary.setVelocityBasedMode(true);
    rotary.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    rotary.setPopupDisplayEnabled(true, true, nullptr);
    rotary.setValue(0.1f);
    rotary.setRange(0.0f, 1.0f, 0.01f);
}

void ADSRWheel::resized() {
    
    rotary.setBounds(0, 0, 100, 100);
    rotaryLabel.setBounds(0, 10, 100, 100);
}

void ADSRWheel::sliderValueChanged(juce::Slider *slider) {
    rotary.getValue();
}

double ADSRWheel::sliderChanged(juce::Slider *slider) {
    return rotary.getValue();
}

//========================================================//
ADSRComponent::ADSRComponent() {
    
    ADSRWheel newAttRotary("Attack");
    ADSRWheel newDecRotary("Decay");
    ADSRWheel newSusRotary("Sustain");
    ADSRWheel newRelRotary("Release");
    
    attackRotary = newAttRotary;
    decayRotary = newDecRotary;
    sustainRotary = newSusRotary;
    releaseRotary = newRelRotary;
    
    addAndMakeVisible(&attackRotary);
    addAndMakeVisible(&decayRotary);
    addAndMakeVisible(&sustainRotary);
    addAndMakeVisible(&releaseRotary);
    
    attackRotary.addListener(this);
}

ADSRComponent::ADSRComponent(ADSRComponent&) {
    
}

void ADSRComponent::resized() {
    attackRotary.setBounds(0, 0, 150, 150);
//    decayRotary.setBounds(100, 0, 150, 150);
//    sustainRotary.setBounds(150, 150, 150, 150);
    releaseRotary.setBounds(100, 0, 150, 150);
}

void ADSRComponent::sliderValueChanged(juce::Slider *slider) {
    printf("Implement me!");
}
