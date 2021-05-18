/*
  ==============================================================================

    ADSRComponent.h
    Created: 15 May 2021 1:13:56pm
    Author:  Robin Su

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum adsr_element {
    attack,
    decay,
    sustain,
    release
};

class ADSRWheel : public juce::Component
{
public:
    ADSRWheel();
    ADSRWheel(const std::string& sliderName, adsr_element);
    ADSRWheel(const ADSRWheel&);
    ADSRWheel& operator=(ADSRWheel&);
    ~ADSRWheel() = default;
    
    void resized() override;
    juce::Slider::RotaryParameters getParams() const;
    juce::String getLabelText() const;
    double getValue();
    adsr_element getType();
//    void sliderValueChanged(juce::Slider *slider) override;
    
private:
    
//    SubsynthAudioProcessor& audioProcessor;
    
    juce::Slider rotary;
    juce::Label rotaryLabel;
    adsr_element element;
    
    void setRotaryStyle();
};


class ADSRComponent : public juce::Component,
public juce::Slider::Listener {
public:
    ADSRComponent();
    ~ADSRComponent() = default;
    ADSRComponent(ADSRComponent&);
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider *slider) override;

private:
//    juce::Grid adsrGrid;
    
    ADSRWheel attackRotary;
    ADSRWheel decayRotary;
    ADSRWheel sustainRotary;
    ADSRWheel releaseRotary;

};
