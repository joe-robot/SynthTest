/*
  ==============================================================================

    SynthSources.h
    Definition of synth source using oscillator class and random number objects
    Created: 15 Apr 2020
    Author:  B159113

  ==============================================================================
*/

#include "SynthSources.h"


SynthSources::SynthSources(){}

SynthSources::~SynthSources(){}

void SynthSources::setSampleRate(float newSampleRate)
{
    oscs.setSampleRate(newSampleRate);  //Setting sample rate of oscillator
}

void SynthSources::setType(float newType)
{
    type = newType;         //Updating type
    if(type > 0 && type < 5)    //If type in oscillator range
    {
        oscs.setType(type - 1); //Then set oscillator type
    }
}

void SynthSources::setFrequency(float frequency)
{
    oscs.setFrequency(frequency);   //Update oscillator frequency
}


float SynthSources::getNextSample()
{
    if(type == 0)   //If set to no source then return 0
    {
        return 0;
    }
    else if(type < 5)   //If set to oscillator resource get next sample
    {
        return oscs.getNextSample();
    }
    else if(type == 5)    //If noise source generate random number bettween 0 and 1
    {
        return (randomGen.nextFloat()*2 - 1);
    }
    
    return 0;
    
}
