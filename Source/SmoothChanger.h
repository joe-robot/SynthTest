/*
  ==============================================================================

    SmoothChanger.h
    This class is used to smooth changes of parameters from a current value to
    a target value in a specified amount of time
 
    The file contains two classes one for smoothing for singular paramters and
    one for smoothing for multiple parameters
    Created: 20 Mar 2020 2:15:17pm
    Author:  Joseph Cresswell

  ==============================================================================
*/

#pragma once

//Include juce
#include <JuceHeader.h>

// =================================
// =================================
// Smooth Changes

/*!
 @class SmoothChanges
 @abstract smooth parameters from current to a target value in a set amount of time
 @discussion called to avoid clicking when user is changing parameters
 
 @namespace none
 @updated 2020-04-24
 */
class SmoothChanges
{
public:
    //==============================================================================
    /** Constructors*/
    SmoothChanges();
    
    /** Destructor*/
    ~SmoothChanges();
    
    //==============================================================================
    
    /**
     * Initialises the smoother with a new target and the current position
     * also setting the time the smooth should occur over
     *
     * @param currentVal is the current value
     * @param targetVal is the target value
     * @param time is the time to transition bettween points
     *
    */
    void init(float currentVal, float targetVal, float time);
    
    /**
     * Initialises the smoother with a new target and the current position
     *
     * @param currentVal is the current value
     * @param targetVal is the target value
     *
    */
    void init(float currentVal, float targetVal);
    
    /**
     * Initialises the smoother with a new target position assuming current position is unchanged
     *
     * @param targetVal is the target value
     *
    */
    void setTargetVal(float targetVal);
    
    /**
     * Sets the sample rate of the processor
     *
     * @param newSampleRate is the sample rate sample/s
     *
    */
    void setSampleRate(float newSampleRate);
    
    /**
     * Sets the smoothing time bettween values
     *
     * @param timeMS which is the smoothing time in MS
     *
    */
    void setSmoothTime(float timeMS);
    
    /**
     * Gets the next value of the smoothed parameter
     *
     * @return next Parameter value according to settings
     *
    */
    float getNextVal();
    
    /**
     * Checks if the value is currently being smoothed
     *
     * @return true if value is still changing toward the target
     *         false if value has reached the target value
     *
    */
    bool checkChanging();
    
    /**
     * Gets the target value
     *
     * @return float of target value
     *
    */
    float getTargetVal();
    
    /**
     * Moves value to target if not already reached
     *
     *
    */
    void setToTarget();
    
private:
    
    float targetValue = 0;
    float lastValue = 0;
    float transitionTime;
    float transitionTimeMS = 50.0f;
    float increment;
    
    float sampleRate = 48000;
    
    bool valueChanging = false;

};


// =================================
// =================================
// Multi Smooth

/*!
 @class MultiSmooth
 @abstract smooth multiple parameters from current to a target value in a set amount of time
 @discussion called to avoid clicking when user is changing parameters
 
 @namespace none
 @updated 2020-04-24
 */
class MultiSmooth
{
public:
    //==============================================================================
    /** Constructors*/
    MultiSmooth();
    MultiSmooth(int numParams);
    /** Destructors*/
    ~MultiSmooth();
    //==============================================================================
    
    /**
     * Initialises the smoother with a new target and the current position
     * also setting the time the smooth should occur over
     *
     * @param currentVal is a array of current values
     * @param targetVal is a array of target values
     * @param time is the time to transition bettween points
     *
    */
    void init(float* currentVal, float* targetVal, float time);
    
    /**
     * Initialises the smoother with a new target and the current position
     *
     * @param currentVal is a array of current values
     * @param targetVal is a array of target values
     *
    */
    void init(float* currentVal, float* targetVal);
    
    /**
     * Initialises the smoother with a new target position assuming current position is unchanged
     *
     * @param targetVal is a array of target values
     *
    */
    void setTargetVal(float* targetVals);
    
    /**
     * Sets the sample rate of the processor
     *
     * @param newSampleRate the sample rate in sample/s
     *
    */
    void setSampleRate(float newSampleRate);
    
    /**
     * Sets the number of parameters that are being smoothed
     *
     * @param numParams the number of parameters to use
     *
    */
    void setNumParams(int numParams);
    
    /**
     * Gets the next value of the smoothed param parameters
     *
     * @param params a array that returns the next valies of the parameters
     *
    */
    void getNextVal(float* params);
    
    /**
     * Checks if any values being smoothed are still changing
     *
     * @return true if it is changing, otherwise false
     *
    */
    bool checkChanging();
    
    
    /**
     * Moves value to target if not already reached
     *
     *
    */
    void setToTarget();
    
private:
    OwnedArray<SmoothChanges> paramSmooth;   //Owned array of parameter smoothing class to smooth each param
    
    float smoothTime = 50.0f;
    
    int numberParams = 4; //Number of parameters to smooth
};
