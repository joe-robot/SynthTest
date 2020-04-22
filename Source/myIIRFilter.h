/*
  ==============================================================================

    myIIRFilter.h
    Created: 15 Apr 2020 9:05:07pm
    Author:  Joseph Cresswell

  ==============================================================================
*/

#pragma once
#include <cmath>

class myIIRFilter
{
public:
    myIIRFilter();
    ~myIIRFilter();
    
    void setSampleRate(float newSampleRate);
    float processNextSample(float nextSample);
    void resetFilter();
    
    void setFilterCutOffFreq(float newCutOffFreq);
    void setFilterType(bool highPassMode);
    void setFilterOrder(float newMinus24DbMode);
    void setFilterParams(float newCutOffFreq, bool newMinus24DbMode);
    void setFilterParams(float newCutOffFreq, bool newMinus24DbMode, bool highPassMode);
    
    void calcDiffEqnCoeffs(bool typeChanged);
    
    
private:
    bool setCutOffFreq(float newCutOffFreq);
    bool setOrder(bool newMinus24DbMode);
    bool setType(bool highPassMode);
    
    bool type = false; //type is the variable that decides type of oscillator (false is low pass, true is highpass);
    bool minus24dbMode = false;
    int order = 2;
    
    double prevInput[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    double prevOutput[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0,0.0f,0.0f,0.0f,0.0f};
    
    int delayLinePos = 0;
    double cutOffFreq=100;
    double sampleTime = 1/48000;
    
    double PI = 3.14159265358979;
    
    double wp = 1;
    
    double secondOrderCoeff = 2 * sqrt(2);
    double fourthOrderCoeff[2] = {2 * 0.7654, 2 * 1.8478};

    double differenceEQNCoeffs[2][5];
};


class StereoIIRFilters
{
public:
    StereoIIRFilters(){};
    ~StereoIIRFilters(){};
    
    void setSampleRate(float sampleRate)
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].setSampleRate(sampleRate);
        }
    }
    
    void setFilterType(bool highPassMode)
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].setFilterType(highPassMode);
        }
    }
    
    void setFilterOrder(bool minus24DbMode)
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].setFilterOrder(minus24DbMode);
        }
    }
    
    void setFilterCutOffFreq(float freq)
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].setFilterCutOffFreq(freq);
        }
    }
    
    void getNextSample(float* inputSamples)
    {
        for(int i = 0; i < 2; ++i)
        {
            inputSamples[i] = filters[i].processNextSample(inputSamples[i]);
        }
    }
    
    void resetFilter()
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].resetFilter();
        }
    }
    
private:
    
    myIIRFilter filters[2];
    
};
