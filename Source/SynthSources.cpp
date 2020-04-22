/*
  ==============================================================================

    SynthSources.cpp
    Created: 22 Apr 2020 10:11:18am
    Author:  Joseph Cresswell

  ==============================================================================
*/

#include "SynthSources.h"


SynthSources::SynthSources(){}

SynthSources::~SynthSources(){}

void SynthSources::setSampleRate(float newSampleRate)
{
    oscs.setSampleRate(newSampleRate);
}

void SynthSources::setType(float newType)
{
    type = newType;
    if(type > 1 && type < 5)
    {
        oscs.setType(type - 1);
    }
}

void SynthSources::setFrequency(float frequency)
{
    oscs.setFrequency(frequency);
}


float SynthSources::getNextSample()
{
    if(type == 0)
    {
        return 0;
    }
    else if(type < 5)
    {
        return oscs.getNextSample();
    }
    else if(type == 5)
    {
        return (randomGen.nextFloat()*2 - 1);
    }
    
    return 0;
    
}
