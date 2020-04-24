/*
  ==============================================================================

    myIIRFilter.h
    This class creates a filter to filter incoming samples using a IIR filter
    Filters can be -12db/oct or -24db/oct, high pass or low pass
    Created: 15 Apr 2020
    Author:  B159113

  ==============================================================================
*/

#include "myIIRFilter.h"
#include <iostream>

myIIRFilter::myIIRFilter()  //Constructor
{}
myIIRFilter::~myIIRFilter(){}   //Destructor

void myIIRFilter::setSampleRate(float newSampleRate)
{
    sampleTime = 1/newSampleRate;   //updating sample time based on input sample rate
}


bool myIIRFilter::setCutOffFreq(float newCutoffFreq)
{
    if(cutOffFreq != newCutoffFreq)             //check cut off frequency changed
    {
        cutOffFreq = newCutoffFreq;             //Update cut off
        wp = 2 * tan(cutOffFreq * PI * sampleTime);     //Calculate freuqncy warped cut-off so bilinear transform can be used

        return true;                        //Return true to show that the value has changed
    }
    
    return false;                           //Return false if value hasn't changed
    
}


void myIIRFilter::calcDiffEqnCoeffs(bool typeChanged)
{
    double wpSquared = wp * wp;             //Calculations required for the difference equations
    double wpSquaredp4 = wpSquared + 4;
    
    if(minus24dbMode)                   //If in -24Db/Octave mode calculate 4th order difference equation coeffs
    {
        double wps4Squared = wpSquaredp4 * wpSquaredp4;
        double b1 = fourthOrderCoeff[0] * wp;
        double b2 = fourthOrderCoeff[1] * wp;
        double b1b2 = b1 * b2;
        double b1pb2 = b1 + b2;
        double bco = 2 * wpSquared -8;

        if(type)                //If high pass use high pass coeffs
        {
            if(typeChanged) //Only update if type or order recently changed for high pass
            {
                differenceEQNCoeffs[0][0] = 16;
                differenceEQNCoeffs[0][1] = -64;
                differenceEQNCoeffs[0][2] = 96;
                differenceEQNCoeffs[0][3] = -64;
                differenceEQNCoeffs[0][4] = 16;
            }
        }
        else        //If low pass calc low pass coedds
        {
            differenceEQNCoeffs[0][0] = wpSquared * wpSquared;
            differenceEQNCoeffs[0][1] = 4 * differenceEQNCoeffs[0][0];
            differenceEQNCoeffs[0][2] = 6 * differenceEQNCoeffs[0][0];
            differenceEQNCoeffs[0][3] = differenceEQNCoeffs[0][1];
            differenceEQNCoeffs[0][4] = differenceEQNCoeffs[0][0];
        }
        
        //Calculate output part of the difference equation, same for both high pass and low pass
        
        double firstAndLast = wps4Squared + b1b2;
        double gainsumthingy = b1pb2 * wpSquaredp4;
        double middleVals = bco * 2 * wpSquaredp4;
        double middleVals2 = bco * b1pb2;
        
        differenceEQNCoeffs[1][0] = -firstAndLast + gainsumthingy;
        differenceEQNCoeffs[1][1] = -middleVals + middleVals2;
        differenceEQNCoeffs[1][2] = -2*wps4Squared + 2*b1b2 - bco * bco;
        differenceEQNCoeffs[1][3] = -middleVals - middleVals2;
        differenceEQNCoeffs[1][4] = firstAndLast + gainsumthingy;
        
    }
    else    //Otherwise calculate 2nd order differnce equation
    {
        double b = secondOrderCoeff * wp;
        if(type)    //Calculate input part for high pass if in high pass mode
        {
            if(typeChanged) //Only update if type or order recently changed for high pass
            {
                differenceEQNCoeffs[0][0] = 4;
                differenceEQNCoeffs[0][1] = -8;
                differenceEQNCoeffs[0][2] = 4;
            }
        }
        else //Calculate input part of the diffence equation
        {
            differenceEQNCoeffs[0][0] = wpSquared;
            differenceEQNCoeffs[0][1] = 2 * differenceEQNCoeffs[0][0];
            differenceEQNCoeffs[0][2] = differenceEQNCoeffs[0][0];
        }
        
        //Calculate output part of difference equation, same for high pass and low pass
        
        differenceEQNCoeffs[1][0] = (-wpSquaredp4 + b);
        differenceEQNCoeffs[1][1] = -2 * wpSquared + 8;
        differenceEQNCoeffs[1][2] = wpSquaredp4 + b;
    }

}



float myIIRFilter::processNextSample(float nextSample) //Process the next sample
{
    prevInput[delayLinePos] = nextSample;   //update previous input
    
    int delayDiff = delayLinePos - order;   //Calulate the latest sample position
    
    double output = 0;  //Initialise output as 0
    
    for(int i = 0 ; i < order+1; ++i)
    {
        int pos = delayDiff + i;
        if(pos < 0)
        {
            pos = pos + 10;
            
        }
        
        output = output + differenceEQNCoeffs[0][i] * prevInput[pos]; //Calculate input part of the equations
        
        if(i < order)   //is one less than the order
        {
            output = output + differenceEQNCoeffs[1][i] * prevOutput[pos]; //Calculate output part of the equations
        }
    }
    
    output = output / differenceEQNCoeffs[1][order];    //Dividing output by output coeff
    
    prevOutput[delayLinePos] = output;  //Updating the previous output
    
    delayLinePos++;         //Updating the delay line position
    if(delayLinePos > 9)
    {
        delayLinePos = 0;
    }
    
    return output;  //Returning the output
}


bool myIIRFilter::setOrder(bool newMinus24DbMode)   //Setting filter order
{
    if(minus24dbMode != newMinus24DbMode)   //If the mode changed
    {
        minus24dbMode = newMinus24DbMode;   //Update mode
        order = minus24dbMode ? 4 : 2;      //Update the order based on mode selected
        return true;                        //Return that value has changed
    }
    return false;                           //Return that the value hasn't changed
}

bool myIIRFilter::setType(bool highPassMode)    // Setting filter type
{
    if(highPassMode != type)        //If the type has changed
    {
        type = highPassMode;        //Update type
        return true;                //Return that the value has changed
    }
    
    return false;                   //Return that the value hasn't changed
}

void myIIRFilter::setFilterParams(float newCutOffFreq, bool newMinus24DbMode, bool highPassMode) //Setting all filter params
{
    bool updateOrderType = false;               //Parm to check if the type or order has changed
    if(setType(highPassMode) || setOrder(newMinus24DbMode)) //Check if type or order have changed
    {
        updateOrderType = true;
    }
    
    if(setCutOffFreq(newCutOffFreq) || updateOrderType) //If order type or cutoff frequency changed recalculate the difference equations
    {
        calcDiffEqnCoeffs(updateOrderType);
    }
}

void myIIRFilter::setFilterParams(float newCutOffFreq, bool newMinus24DbMode)
{
    setFilterParams(newCutOffFreq, newMinus24DbMode, type); //Use other set filter params method with current type
}

void myIIRFilter::setFilterType(bool highPassMode)  //Setting the filter type
{
    if(setType(highPassMode))       //Check if type changed
    {
        calcDiffEqnCoeffs(true);    //If it has changed update the difference eqns
    }
}

void myIIRFilter::setFilterOrder(float newMinus24DbMode)    //Setting filter order
{
    if(setOrder(newMinus24DbMode))  //Check if order changed
    {
        calcDiffEqnCoeffs(true);    //If it has changed update the difference eqns
    }
}

void myIIRFilter::setFilterCutOffFreq(float newCutOffFreq)  //Setting cutoff frequency
{
    if(setCutOffFreq(newCutOffFreq))    //Check if cut off frequency changed
    {
        calcDiffEqnCoeffs(false);       //If changed calculate the difference equations
    }
}

void myIIRFilter::resetFilter() //Reset the filer
{
    for(int i = 0; i < 10; ++i) //Loop through all the previous stored parameters and set them to 0
    {
        prevInput[i] = 0.0f;
        prevOutput[i] = 0.0f;
    }
}
