/*
  ==============================================================================

    Oscillator.h
    This class creates calualtes the next sample of an oscillator. The type of
    oscillator can be switched bettween 7 possible types.
    Created: 31 Jan 2020 3:40:51pm
    Author:  B159113

  ==============================================================================
*/

#ifndef Oscillator_h   //This checks if the oscillator class has been already defined if not it defines it
#define Oscillator_h

#include <stdio.h>  //Including the standard library
#include <cmath>    //Including the math library to calculate maths operations

// =================================
// =================================
// Oscillator

/*!
 @class Oscillator
 @abstract source for generating waves of different types
 @discussion used as main sound sources for synth
 
 @namespace none
 @updated 2020-04-24
 */
class Oscillator
{
public:
    //==============================================================================
    /** Constructors*/
    Oscillator();
    Oscillator(float newFrequency, float newSampleRate, int newType);
    /** Destructor*/
    ~Oscillator();
    
    //==============================================================================
    
    /**
     * Sets the frequnecy of the oscillator
     *
     * @param newFrequency in Hz
     *
    */
    void setFrequency(float newFrequency);
    
    /**
     * Sets the sample Rate of the oscillator
     *
     * @param newSampleRate in samples / s
     *
     */
    void setSampleRate(float newSampleRate);
    
    /**
     * Gets the next sample from the oscillator
     *
     * @return the next sample of the oscillator
     *
    */
    float getNextSample();
    
    /**
     * Sets the type of the oscillator
     *
     * @param newType is a value from 0- 6
     * 0 = sine Oscillator
     * 1 = square wave Oscillator
     * 2 = triangle wave Oscillator
     * 3 = phasor
     * 4 = a oscillator with a sine curve for the first half and 0 after
     * 5 = a oscillator with a sine curve for middle 2 quaters and 0 otherwise
     * 6 = a oscillator with a sine curve for final half and 0 otherwise
     *
    */
    void setType(int newType);
    
private:
    ///Initilising required attributes
    float frequency = 440.0f;       //Frequency of oscillator
    float sampleRate = 44100.0f;    //SampleRate of oscillator
    float phasePos = 0.0f;  //Phase position which is current position in the wave
    float phaseDelta = 0.0f; //Phase delta is amount the phase changes per sample
    int type = 0;             //The type of oscillatot
    
    /**
     * Gets the next sample from the sine oscillator
     *
     * @return the next sample of the oscillator
     *
    */
    float sinWave();
    
    /**
     * Gets the next sample from the square oscillator
     *
     * @return the next sample of the oscillator
     *
    */
    float squareWave();
    
    /**
     * Gets the next sample from the triangle oscillator
     *
     * @return the next sample of the oscillator
     *
    */
    float triWave();
    
    /**
     * Gets the next sample from the phasor
     *
     * @return the next sample of the oscillator
     *
    */
    float phasor();
    
    /**
     * Gets the next sample from the sine oscillator that is 0 until half way through where it has a positive curve
     *
     * @return the next sample of the oscillator
     *
    */
    float singleEndBump();
    
    /**
     * Gets the next sample from the sine oscillator that is 0 after half way through
     *
     * @return the next sample of the oscillator
     *
    */
    float singleInitialBump();
    
    /**
     * Gets the next sample from the sine oscillator that is 0 until quarter way through where it has a positive curve until 3 quarters of the way through
     *
     * @return the next sample of the oscillator
     *
    */
    float singleMiddleBump();
    
};

#endif /*Oscillator.h*/
