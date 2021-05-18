/*
  ==============================================================================

    ADSRComponent.h
    Created: 15 May 2021 1:13:56pm
    Author:  Robin Su

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ADSRWheel : public juce::Slider,
                  public juce::Slider::Listener
{
public:
    ADSRWheel();
    ADSRWheel(const std::string& sliderName);
    ADSRWheel(const ADSRWheel&);
    ADSRWheel& operator=(ADSRWheel&);
    ~ADSRWheel() = default;
    
    void resized() override;
    juce::Slider::RotaryParameters getParams() const;
    juce::String getLabelText() const;
    double getValue();
    void sliderValueChanged(juce::Slider *slider) override;
    
private:
    juce::Slider rotary;
    juce::Label rotaryLabel;
    
    void setRotaryStyle();
};


class ADSRComponent : public juce::Component {
public:
    ADSRComponent();
    ~ADSRComponent() = default;
    ADSRComponent(ADSRComponent&);
    void resized() override;

private:
    ADSRWheel attackRotary;
    ADSRWheel decayRotary;
    ADSRWheel sustainRotary;
    ADSRWheel releaseRotary;

};
