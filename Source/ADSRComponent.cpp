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
    
    rotaryLabel.setColour(1, juce::Colours::white);
//    rotary.addListener(rotary);
}

ADSRWheel::ADSRWheel(const std::string& sliderName, adsr_element elem) {
    setRotaryStyle();
    rotaryLabel.setText(sliderName, juce::dontSendNotification);
    element = elem;
//    addAndMakeVisible(&rotary);
//    addAndMakeVisible(&rotaryLabel);
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

adsr_element ADSRWheel::getType(){
    return this->element;
}

void ADSRWheel::setRotaryStyle() {
    rotary.setSliderStyle(juce::Slider::Rotary);
    rotary.setRotaryParameters(0.0, juce::MathConstants<float>::twoPi, true);
    rotary.setVelocityBasedMode(true);
    rotary.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    rotary.setPopupDisplayEnabled(true, true, nullptr);
    rotary.setValue(0.1f);
    rotary.setRange(0.0f, 1.0f, 0.01f);
    rotaryLabel.attachToComponent(&rotary, false);
}

void ADSRWheel::resized() {
    
    rotary.setBounds(10, 30, 75, 75);
}

//void ADSRWheel::sliderValueChanged(juce::Slider *slider) {
//    if (slider == &rotary) {
//        this->setValue(rotary.getValue());
//    }
//}


//========================================================//
ADSRComponent::ADSRComponent() {
    
    setSize(100, 4000);
    
    ADSRWheel newAttRotary("Attack", attack);
    ADSRWheel newDecRotary("Decay", decay);
    ADSRWheel newSusRotary("Sustain", sustain);
    ADSRWheel newRelRotary("Release", release);
    
    attackRotary = newAttRotary;
    decayRotary = newDecRotary;
    sustainRotary = newSusRotary;
    releaseRotary = newRelRotary;
    
    addAndMakeVisible(&attackRotary);
    addAndMakeVisible(&decayRotary);
    addAndMakeVisible(&sustainRotary);
    addAndMakeVisible(&releaseRotary);
}

ADSRComponent::ADSRComponent(ADSRComponent&) {
    
}

void ADSRComponent::paint(juce::Graphics& g) {
    
}

void ADSRComponent::resized() {
    attackRotary.setBounds(0, 0, 100, 100);
    decayRotary.setBounds(100, 0, 100, 100);
    sustainRotary.setBounds(200, 0, 100, 100);
    releaseRotary.setBounds(300, 0, 100, 100);
    
//    juce::Rectangle<int> grid {0,0, 200, 2000};
//    adsrGrid.performLayout(grid);
}

void ADSRComponent::sliderValueChanged(juce::Slider *slider) {
    
}
