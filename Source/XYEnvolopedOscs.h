/*
  ==============================================================================

    XYEnvolopedOscs.h
    Generates a 4 source sound source that changes bettween 4 sources using an
    input X, Y value
    Created: 19 Apr 2020
    Author:  B159113

  ==============================================================================
*/

#pragma once

//including required files
#include <JuceHeader.h>
#include "SmoothChanger.h"
#include "SynthSources.h"

// =================================
// =================================
// XY Envoloped Oscillators

/*!
 @class XYEnvolopedOscs
 @abstract generates 4 sources that take an input XY value to calculate their amplitude
 @discussion called by synth voice to generate main oscillators
 
 @namespace none
 @updated 2020-04-24
 */
class XYEnvolopedOscs
{
public:
    //==============================================================================
    /** Constructor*/
    XYEnvolopedOscs();
    /** Destructor*/
    ~XYEnvolopedOscs();
    //==============================================================================
    
    /**
     * Sets the sample Rate of the synth source
     *
     * @param sampleRate is the sampleRate in samples / s
     *
    */
    void setSampleRate(float sampleRate);
    
    /**
     * Sets the source type
     *
     * @param oscNum is the number oscillator that the source type is changing for
     * @param sourceType sets the source type of the oscillator
     *
    */
    void setSourceType(int oscNum, int sourceType);
    
    
    /**
     * Sets the oscillator minimum and maximum volume
     *
     * @param oscNum is the number oscillator that the volume is changing for
     * @param minVol  is the minimum volume of the oscillator
     * @param maxVol is the maximum volume of the oscilllator
     */
    void setOscMinMaxVolume(int oscNum, float minVol, float maxVol);

    /**
     * Sets the oscillator minimum volume
     *
     * @param oscNum is the number oscillator that the min volume is changing for
     * @param minVol  is the minimum volume of the oscillator
    */
    void setOscMinVol(int oscNum, float minVol);

    /**
     * Sets the oscillator maximum volume
     *
     * @param oscNum is the number oscillator that the max volume is changing for
     * @param maxVol is the maximum volume of the oscilllator
    */
    void setOscMaxVol(int oscNum, float maxVol);
    
    /**
     * Sets the oscillator pan amount
     *
     * @param oscNum is the number oscillator that the pan amount is changing for
     * @param newPanAmount is the pan amount of the oscillator
    */
    void setPanAmount(int oscNum, float newPanAmount);
    
    /**
     * Sets the oscillators midi input value
     *
     * @param midiNote is the input midi note
    */
    void setOscsMidiInput(int midiNote);
    
    /**
     * Sets oscillator tune amount
     *
     * @param oscNum is the oscillator to change the tune amount for
     * @param newTuneAmount is the tune amount for the oscillator in semitones
    */
    void setTuneAmount(int oscNum, int newTuneAmount);
    
    /**
     * Gets the next value based on envolope values and sets the output samples
     *
     * @param envs is the X Y input to set the oscillator amplitudes
     * @param outSamples is the array to output the stereo samples to
    */
    void getNextVal(float envs[2], float outSamples[2]);
    
    /**
     * Sets the systems play mode
     *
     * @param playMode is to be set true when playing and false when stopped
    */
    void playMode(bool playMode);
    
    
private:
    
    /**
     * Sets the pan amount
     *
     * @param newPanAmount sets the pan amount
     * @param channel sets the channel to pan
     */
    float pan(float newPanAmount, int channel);
    
    /**
     * Sets the oscillator frequnecy
     *
     * @param oscNum sets the oscillator number to change the frequnecy for
     * @param frequency sets the frequency to change the oscilalator to
    */
    void setOscFrequency(int oscNum, float frequency);
    
    /**
     * Updates the oscillator frequency for the next sample
    */
    void updateFreq();
    
    /**
     * Resetting the smoothed params to target values
    */
    void resetParams();
    
    //Array of osscilators
    OwnedArray<SynthSources> oscs;
    
    //Array to store min and max volumes of oscillators
    float minMaxVols[2][4]  =  {{0.1f, 0.1f, 0.1f, 0.1f},   //Min Oscillator Volumes
                                {0.5f, 0.5f, 0.5f, 0.5f}};  //Max Oscillator Volumes
    
    //Array to store tune amount and target tune amount
    int tuneAmount[4] = {0, 0, 0, 0};
    int targetTuneAmount[4] = {0, 0, 0, 0};
    
    //Array to store pan amount
    float panAmount[4] = {0, 0, 0, 0};
    
    //Param to store prev midi input
    int prevMidiInput = 48;
    
    bool playing = false; //Param for osc playing
    bool changeFreq[4] = {false, false, false, false}; //Check if freq changing
    bool enableOsc[4] = {true, true, true, true};  //Param for enabling osc
    
    //Smooth frequency object
    OwnedArray<SmoothChanges> smoothFreq;
    
};
