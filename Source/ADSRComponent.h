/*
  ==============================================================================

    ADSRComponent.h
    Created: 15 May 2021 1:13:56pm
    Author:  Robin Su

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum ADSR_Element {
    attack,
    decay,
    sustain,
    release
};

class ADSRWheel : public juce::Component
{
public:
    ADSRWheel();
    ADSRWheel(const std::string& sliderName, ADSR_Element element);
    ADSRWheel(const ADSRWheel&);
    ADSRWheel& operator=(ADSRWheel&);
    ~ADSRWheel() = default;
    
    void resized() override;
    juce::Slider::RotaryParameters getParams() const;
    juce::String getLabelText() const;
    double getValue();
    ADSR_Element getType();
//    void sliderValueChanged(juce::Slider *slider) override;
    juce::Slider rotary;
private:
    
//    SubsynthAudioProcessor& audioProcessor;
    
    juce::Label rotaryLabel;
    ADSR_Element element;
    
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
    
    void sliderValueChanged(juce::Slider* slider) override;
    
    juce::ADSR::Parameters getEnvelope();

private:
//    juce::Grid adsrGrid;
    
    ADSRWheel attackRotary;
    ADSRWheel decayRotary;
    ADSRWheel sustainRotary;
    ADSRWheel releaseRotary;
    
    float attVal;
    float decVal;
    float susVal;
    float relVal;
    
};
