/*
  ==============================================================================

    SmoothChanger.h
    This class is used to smooth changes of parameters from a current value to
    a target value in a specified amount of time
    Created: 20 Mar 2020 2:15:17pm
    Author:  Joseph Cresswell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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
     * @param the current value
     * @param the target value
     * @param the time to transition bettween points
     *
    */
    void init(float currentVal, float targetVal, float time);
    
    /**
     * Initialises the smoother with a new target and the current position
     *
     * @param the current value
     * @param the target value
     *
    */
    void init(float currentVal, float targetVal);
    
    /**
     * Initialises the smoother with a new target position assuming current position is unchanged
     *
     * @param the target value
     *
    */
    void setTargetVal(float targetVal);
    
    /**
     * Sets the sample rate of the processor
     *
     * @param sample rate
     *
    */
    void setSampleRate(float newSampleRate);
    
    /**
     * Sets the smoothing time bettween values
     *
     * @param smoothing time in MS
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

class MultiSmooth
{
public:
    MultiSmooth();
    MultiSmooth(int numParams);
    ~MultiSmooth();
    
    /**
     * Initialises the smoother with a new target and the current position
     * also setting the time the smooth should occur over
     *
     * @param the current value
     * @param the target value
     * @param the time to transition bettween points
     *
    */
    void init(float currentVal[4], float targetVal[4], float time);
    
    /**
     * Initialises the smoother with a new target and the current position
     *
     * @param the current value
     * @param the target value
     *
    */
    void init(float currentVal[4], float targetVal[4]);
    
    /**
     * Initialises the smoother with a new target position assuming current position is unchanged
     *
     * @param the target value
     *
    */
    void setTargetVal(float targetVals[4]);
    
    /**
     * Sets the sample rate of the processor
     *
     * @param sample rate
     *
    */
    void setSampleRate(float newSampleRate);
    
    /**
     * Sets the number of parameters that are being smoothed
     *
     * @param numParams
     *
    */
    void setNumParams(int numParams);
    
    /**
     * Gets the next value of the smoothed param parameters
     *
     * @return next Parameter value according to settings
     *
    */
    void getNextVal(float params[4]);
    
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
    
    //float paramParams[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //Setting default values for the param params
    
    float smoothTime = 50.0f;
    
    int numberParams = 4; //Number of parameters to smooth
};
