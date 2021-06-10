/*
  ==============================================================================

    This file contains the header information for a JUCE 
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

#pragma once
#include <JuceHeader.h>

enum ADSR_Element
{
    attack,
    decay,
    sustain,
    release
};

class ADSRWheel : public juce::Component
{
public:
    juce::Slider rotary;

    ADSRWheel();
    ADSRWheel (const std::string& sliderName, ADSR_Element element);
    ADSRWheel (const ADSRWheel&);
    ADSRWheel& operator= (ADSRWheel&);
    ~ADSRWheel() = default;

    void resized() override;
    juce::Slider::RotaryParameters getParams() const;
    juce::String getLabelText() const;
    double getValue();
    ADSR_Element getType();

private:
    juce::Label rotaryLabel;
    ADSR_Element element;

    void setRotaryStyle();
};

class ADSRComponent : public juce::Component,
                      public juce::Slider::Listener
{
public:
    ADSRComponent();
    ~ADSRComponent() = default;
    ADSRComponent (ADSRComponent&);

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;

    juce::ADSR::Parameters getEnvelope();

private:
    ADSRWheel attackRotary;
    ADSRWheel decayRotary;
    ADSRWheel sustainRotary;
    ADSRWheel releaseRotary;

    juce::ADSR::Parameters params;

    float attVal;
    float decVal;
    float susVal;
    float relVal;
};
