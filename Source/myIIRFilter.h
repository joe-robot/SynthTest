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
    void setFilterType(float highPassMode);
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
