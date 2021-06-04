/*
  ==============================================================================

    VoiceTesting.cpp
    Created: 3 Jun 2021 3:07:46pm
    Author:  Robin Su

  ==============================================================================
*/
#include "VoiceTesting.h"

enum waveType {
    Sine = 1,
    Square = 2,
    Saw = 3,
    Tri = 4
};

void VoiceTesting::runTest()
{
    beginTest("voiceRenderNextBlockTest");
    
}


void VoiceTesting::filterTest()
{
    int filterNum = 1;
    float cutoff = 20.0f, resonance = 1.0f;
    
    testVoice.setFilter(filterNum, cutoff, resonance);
    
    jassert(testVoice.getFilterType() == juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass);
}
    
    
//    void initClass()
//    {
//        audioBufferActual.setSize(2, 512);
//        audioBufferExpected.setSize(2, 512);
//
//        testVoice.setADSRParams(0.5, 0.5, 0.5, 0.5);
//        testVoice.setWave((int)Sine);
//    }
//
//    void renderNextBlock_test()
//    {
//        int startSample = 0;
//
//        juce::dsp::AudioBlock<float> audioBlock{ audioBufferExpected };
//        sineOsc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
//
//
//        initClass();
//
//        testVoice.renderNextBlock(audioBufferExpected, startSample, audioBufferActual.getNumSamples());
//
//
//    }


