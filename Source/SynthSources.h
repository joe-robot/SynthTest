/*
  ==============================================================================

    SynthSources.h
    Definition of synth source using oscillator class and random number objects
    Created: 20 Apr 2020
    Author:  B159113

  ==============================================================================
*/

#pragma once

//Including required files
#include <JuceHeader.h>
#include "Oscillator.h"

// =================================
// =================================
// Synth Sources

/*!
 @class SynthSources
 @abstract sets the appropriate sources for selection from a drop down menu
 @discussion multiple Synth sources for each source in the synth
 
 @namespace none
 @updated 2020-04-24
 */
class SynthSources
{
public:
    //==============================================================================
    /** Constructor*/
    SynthSources();
    /** Destructor*/
    ~SynthSources();
    //==============================================================================
    
    /**
     * Sets the sample Rate of the synth source
     *
     * @param sampleRate is the sampleRate in samples / s
     *
    */
    void setSampleRate(float newSampleRate);
    
    /**
     * Sets the type selected
     *
     * @param newType - changes the source type based on input they are as follows
     *        0 - No Source
     *        1 - Sine wave Source
     *        2 - Square wave Source
     *        3 - Triangle wave Source
     *        4 - Saw wave Source
     *        5 - Noise Source
     *
    */
    void setType(float newType);
    
    /**
     * Sets the frequnecy of the source
     *
     * @param frequency in Hz
     *
    */
    void setFrequency(float frequency);
    
    /**
     * Gets next sample from the source
     *
     * @return next sample from source
     *
    */
    float getNextSample();
    
private:
    int type = 1;   //Intial type set to sine
    
    Oscillator oscs;    //Oscillator object created for wave generation
    Random randomGen;   //Random generator object created for noise sources
    
};
