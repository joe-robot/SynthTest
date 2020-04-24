/*
  ==============================================================================

    myIIRFilter.h
    This class creates a filter to filter incoming samples,
    The file contains 2 classes the myIIRFilter that filters mono samples and
    StereoIIRFilters that can filter stereo samples using the myIIRFilter class
    Created: 15 Apr 2020
    Author:  B159113

  ==============================================================================
*/

#pragma once
#include <cmath>    //Including cmath for maths functions


//Defining the class methods and attributes
class myIIRFilter
{
public:
    //==============================================================================
    /** Constructors*/
    myIIRFilter();
    
    /** Destructor*/
    ~myIIRFilter();
    
    //==============================================================================
    /**
     * Set the sample rate of the filter
     *
     * @param the updated sample rate
     *
    */
    void setSampleRate(float newSampleRate);
    
    /**
     * Get's the next sample from the filter inputing the most recent sample
     *
     * @param next sample which is the most recent unfiltered output sample
     *
     * @return float of the next sample value
     *
    */
    float processNextSample(float nextSample);
    
    /**
     * Method to reset the filter previous input and output samples
     *
    */
    void resetFilter();
    
    /**
     * Set the filters cut off frequency
     *
     * @param new cut off frequency in Hz
     *
    */
    void setFilterCutOffFreq(float newCutOffFreq);
    
    /**
     * Set the filter type bettween high pass and low pass and update difference eqns
     *
     * @param true makes the filter a high pass filter
     *        false makes the filter a low pass filter
     *
    */
    void setFilterType(bool highPassMode);
    
    /**
     * Set the filter order to -12Db/oct or -24dB/oct and update difference eqns
     *
     * @param true makes the filter -24Db/oct
     *        false makes the filter -12Db/oct
     *
    */
    void setFilterOrder(float newMinus24DbMode);
    
    /**
     * Set the filter cut off frequncy and filter order to -12Db/oct or -24dB/oct
     * to avoid recalculating the difference equations multiple times when setting
     * multiple parameters and update difference eqns
     *
     * @param new cut off frequency in Hz
     * @param true makes the filter -24Db/oct
     *        false makes the filter -12Db/oct
     *
    */
    void setFilterParams(float newCutOffFreq, bool newMinus24DbMode);
    
    /**
     * Set the filter cut off frequncy and filter order to -12Db/oct or -24dB/oct
     * to avoid recalculating the difference equations multiple times when setting
     * multiple parameters and update difference eqns
     *
     * @param new cut off frequency in Hz
     * @param true makes the filter -24Db/oct
     *        false makes the filter -12Db/oct
     * @param true makes the filter a high pass filter
     *        false makes the filter a low pass filter
     *
    */
    void setFilterParams(float newCutOffFreq, bool newMinus24DbMode, bool highPassMode);
    
private:
    
    /**
     * Function to callculate the difference equations for the filter
     *
     * @param type changed marks if the filter type or order has recently changed requiring additional calculations
     *
    */
    void calcDiffEqnCoeffs(bool typeChanged);
    
    /**
     * Set the filters cut off frequency
     *
     * @param new cut off frequency in Hz
     *
     * @return return bool is true if value has changed if not it is false
     *
    */
    bool setCutOffFreq(float newCutOffFreq);
    
    /**
     * Set the filters cut off frequency
     *
     * @param true makes the filter -24Db/oct
     *        false makes the filter -12Db/oct
     *
     * @return return bool is true if value has changed if not it is false
     *
     
    */
    bool setOrder(bool newMinus24DbMode);
    
    /**
     * Set the filters type
     *
     * @param true makes the filter a high pass filter
     *        false makes the filter a low pass filter
     *
     * @return return bool is true if value has changed if not it is false
     *
    */
    bool setType(bool highPassMode);
    
    bool type = false; //type is the variable that decides type of oscillator (false is low pass, true is highpass);
    bool minus24dbMode = false; //Order false is -12Db/oct mode, true is -24Db/oct mode
    int order = 2;              //Order of the filter
    
    //Arrays for storing previous inputs and outputs used in the array
    double prevInput[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    double prevOutput[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0,0.0f,0.0f,0.0f,0.0f};
    
    //Delay line positon fot the prev input and output arrays
    int delayLinePos = 0;
    
    //Cut off frequency in Hz
    double cutOffFreq=100;
    
    //Sample time in s
    double sampleTime = 1/48000;
    
    //Pi to be used in calculations
    double PI = 3.14159265358979;
    
    //Frequency warped cut off
    double wp = 1;
    
    //The coefficients for 2nd and 4th order filters
    double secondOrderCoeff = 2 * sqrt(2);
    double fourthOrderCoeff[2] = {2 * 0.7654, 2 * 1.8478};

    //Array for storing the difference equations parameters
    double differenceEQNCoeffs[2][5];
};

//==============================================================================

//Defining the class methods and attributes
class StereoIIRFilters
{
public:
    //==============================================================================
    /** Constructors*/
    StereoIIRFilters(){};
    /** Destructor*/
    ~StereoIIRFilters(){};
    //==============================================================================
    
    /**
     * Set the sample rate of the filter
     *
     * @param the updated sample rate
     *
    */
    void setSampleRate(float sampleRate)
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].setSampleRate(sampleRate);
        }
    }
    
    /**
     * Set the filter type bettween high pass and low pass and update difference eqns
     *
     * @param true makes the filter a high pass filter
     *        false makes the filter a low pass filter
     *
    */
    void setFilterType(bool highPassMode)
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].setFilterType(highPassMode);
        }
    }
    
    /**
     * Set the filter order to -12Db/oct or -24dB/oct and update difference eqns
     *
     * @param true makes the filter -24Db/oct
     *        false makes the filter -12Db/oct
     *
    */
    void setFilterOrder(bool minus24DbMode)
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].setFilterOrder(minus24DbMode);
        }
    }
    
    /**
     * Set the filters cut off frequency
     *
     * @param new cut off frequency in Hz
     *
    */
    void setFilterCutOffFreq(float freq)
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].setFilterCutOffFreq(freq);
        }
    }
    
    /**
     * Get's the next sample from the filter inputing the most recent sample
     *
     * @param array of stereo samples which is the most recent unfiltered output samples
     *
     *
    */
    void getNextSample(float* inputSamples)
    {
        for(int i = 0; i < 2; ++i)
        {
            inputSamples[i] = filters[i].processNextSample(inputSamples[i]);
        }
    }
    
    /**
     * Method to reset the filter previous input and output samples
     *
    */
    void resetFilter()
    {
        for(int i = 0; i < 2; ++i)
        {
            filters[i].resetFilter();
        }
    }
    
private:
    
    //Creating an array of two IIR filters for each channel of stereo
    myIIRFilter filters[2];
    
};
