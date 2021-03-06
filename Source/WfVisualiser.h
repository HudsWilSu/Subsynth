/*
  ==============================================================================

    This file contains the header and implementation information for a JUCE 
    AudioVisualiserComponent

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

#include <JuceHeader.h>
#pragma once

class WaveformVisualiser : public juce::AudioVisualiserComponent
{
public:
    // constructor
    WaveformVisualiser() : AudioVisualiserComponent (2) // number of channels getTotalNumInputChannels()
    {
        setBufferSize (512); // buffer size
        setSamplesPerBlock (2); // samples per block
        setColours (juce::Colours::blueviolet, juce::Colours::lightgoldenrodyellow);
    }
};