/*
  ==============================================================================

    SmoothChanger.cpp
    This class is used to smooth changes of parameters from a current value to
    a target value in a specified amount of time
    Created: 20 Mar 2020 2:15:17pm
    Author:  Joseph Cresswell

  ==============================================================================
*/

#include <iostream>
#include "SmoothChanger.h"

SmoothChanges::SmoothChanges(){
    setSmoothTime(transitionTimeMS); //Setting transition time in samples
}

SmoothChanges::~SmoothChanges(){}

void SmoothChanges::init(float currentVal, float targetVal, float time)
{
    setSmoothTime(time);    //Setting smooth time to set value
    init(currentVal, targetVal);    //Initialising the increment values bettween current and target
}

void SmoothChanges::init(float currentVal, float targetVal) //Initialising the increment bettween current and target
{
        lastValue = currentVal;     //Update last value
        setTargetVal(targetVal);    //Set target value to new target
}

void SmoothChanges::setTargetVal(float targetVal)
{
    if(targetVal != targetValue) //Checking target value changed
    {
        //std::cout<<"Old Target: "<<targetValue  << "Target Val: "<< targetVal << " LastValue: " << lastValue;
        if(lastValue != targetVal) //Checking not already reached target
        {
            targetValue = targetVal;    //Update target value
            //std::cout<<"   New Target: "<<targetValue;
            float valDiff = targetValue - lastValue;     //Get the difference and calculate the increment each sample
            increment = valDiff / transitionTime;       //Calculate the increment
            valueChanging = true;
        }
        else
        {
            valueChanging = false;  //If target reached reset the increment and stop changing
            increment = 0.0;
        }
    }
    //Otherwise make no change
}

void SmoothChanges::setSampleRate(float newSampleRate)
{
    sampleRate= newSampleRate > 0 ? newSampleRate : 48000; //Check passed sample rate bigger than zero if not set as default value
    
    setSmoothTime(transitionTimeMS); //Need to set time again for change
}

void SmoothChanges::setSmoothTime(float timeMS)
{
    transitionTimeMS = timeMS > 0 ? timeMS : 50.0f;   //Checking time is more than 0 and setting transition time MS attribute appropriatly
    
    transitionTime = (transitionTimeMS / 1000.0f) * sampleRate; //Setting transition time in MS
    
}


float SmoothChanges::getNextVal()
{
    if(valueChanging)   //Check value is changing
    {
        lastValue += increment; //Incrementing the value

        if(increment > 0.0f)
        {
            if(lastValue >= targetValue)     //Checking value exceed target if so output target
            {
                increment = 0;
                valueChanging = false;
                return targetValue;
            }
        }
        else
        {
            if(lastValue <= targetValue)     //Checking value exceed target if so output target
            {
                increment = 0;
                valueChanging = false;
                return targetValue;
            }
        }
        return lastValue;           //If target not reached output incremented value
    }
    
    return targetValue;             //If no increment then just output the target value
}

bool SmoothChanges::checkChanging()
{
    return valueChanging;
}


float SmoothChanges::getTargetVal() //getting the target value
{
    return targetValue;
}

void SmoothChanges::setToTarget()
{
    if(valueChanging)
    {
        lastValue = targetValue;
        valueChanging = false;
    }
}

//-----------------------------------------

MultiSmooth::MultiSmooth()
{
    setNumParams(numberParams); //Setting up parameter smooth array up with default number of arrays
}

MultiSmooth::MultiSmooth(int numParams)
{
    setNumParams(numParams);
}

MultiSmooth::~MultiSmooth(){}

void MultiSmooth::init(float* currentVal, float* targetVal, float time)
{
    smoothTime = time > 0 ? time : smoothTime;  //Updating the smooth time attribute with user input
    init(currentVal, targetVal);                //Initialising the current and target times of param smoothign
}

void MultiSmooth::init(float* currentVal, float* targetVal)
{
    for(int i = 0; i < numberParams; ++i)  //Reinitialising the array with the updated current and target values
    {
        paramSmooth[i] -> init(currentVal[i], targetVal[i], smoothTime);
    }
}

void MultiSmooth::setTargetVal(float* targetVal)
{
    for(int i=0; i < numberParams; ++i)    //Update target value for all parts of the envolope
    {
        paramSmooth[i] -> setTargetVal(targetVal[i]);
    }
}

void MultiSmooth::getNextVal(float *params)
{
    for(int i = 0; i < numberParams; ++i)  //Update param params with next smoothed value
    {
        params[i] = paramSmooth[i] -> getNextVal();
    }
}

void MultiSmooth::setSampleRate(float newSampleRate)
{
    for(int i = 0; i < numberParams; ++i)  //Updating the sampleRate for each parameter
    {
        paramSmooth[i] -> setSampleRate(newSampleRate);
    }
}

void MultiSmooth::setNumParams(int numParams)
{
    paramSmooth.clear();
    //paramSmooth.clearQuick(true);
    numberParams = numParams;
    
    for(int i = 0; i < numberParams; ++i)  //Initialising an owned array of parameter smoothers
    {
        paramSmooth.add(new SmoothChanges());
    }
}


bool MultiSmooth::checkChanging()
{
    for(int i = 0; i < numberParams; ++i)
    {
        if(paramSmooth[i] -> checkChanging())
        {
            return true;
        }
    }

    return false;
}



void MultiSmooth::setToTarget()
{
    for(int i = 0; i < numberParams; ++i)
    {
        paramSmooth[i] -> setToTarget();
    }
}
