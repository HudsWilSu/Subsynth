/*
  ==============================================================================

    This file contains the implementation information for a JUCE 
    component containing Attack, Decay, Sustain, Release GUI logic.

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

#include "ADSRComponent.h"

ADSRWheel::ADSRWheel() {
    setRotaryStyle();
    addAndMakeVisible(&rotary);
    addAndMakeVisible(&rotaryLabel);
}

ADSRWheel::ADSRWheel(const std::string& sliderName, ADSR_Element elem) {
    setRotaryStyle();
    rotaryLabel.setText(sliderName, juce::dontSendNotification);
    element = elem;
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

ADSR_Element ADSRWheel::getType(){
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
    
    attackRotary.rotary.addListener(this);
    decayRotary.rotary.addListener(this);
    sustainRotary.rotary.addListener(this);
    releaseRotary.rotary.addListener(this);
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

}

void ADSRComponent::sliderValueChanged(juce::Slider *slider) {
    if (slider == &(attackRotary.rotary)) {
        attVal = attackRotary.getValue();
        
    }
    else if (slider == &(decayRotary.rotary)) {
        decVal = decayRotary.getValue();
    }
    else if (slider == &(sustainRotary.rotary)) {
        susVal = sustainRotary.getValue();
    }
    else {
        relVal = releaseRotary.getValue();
    }
    
}
   
juce::ADSR::Parameters ADSRComponent::getEnvelope() {
    juce::ADSR::Parameters params {
        attVal,
        decVal,
        susVal,
        relVal
    };
    
    return params;
}
    

