/*
  ==============================================================================

    SynthSources.h
    Created: 22 Apr 2020 10:11:18am
    Author:  Joseph Cresswell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"


class SynthSources
{
public:
    SynthSources();
    ~SynthSources();
    
    void setSampleRate(float newSampleRate);
    void setType(float newType);
    
    void setFrequency(float frequency);
    
    float getNextSample();
    
private:
    int type = 1;
    
    Oscillator oscs;
    Random randomGen;
    
};
