/*
  ==============================================================================

    myIIRFilter.cpp
    Created: 15 Apr 2020 9:05:07pm
    Author:  Joseph Cresswell

  ==============================================================================
*/

#include "myIIRFilter.h"

myIIRFilter::myIIRFilter()
{
    
}
myIIRFilter::~myIIRFilter(){}

void myIIRFilter::setSampleRate(float newSampleRate)
{
    sampleTime = 1/newSampleRate;
}


bool myIIRFilter::setCutOffFreq(float newCutoffFreq)
{
    if(cutOffFreq != newCutoffFreq)
    {
        cutOffFreq = newCutoffFreq;
        wp = 2 * tan(cutOffFreq * PI * sampleTime);

        return true;
    }
    
    return false;
    
}


void myIIRFilter::calcDiffEqnCoeffs(bool typeChanged)
{
    double wpSquared = wp * wp;
    double wpSquaredp4 = wpSquared + 4;
    
    if(minus24dbMode)
    {
        double wps4Squared = wpSquaredp4 * wpSquaredp4;
        double b1 = fourthOrderCoeff[0] * wp;
        double b2 = fourthOrderCoeff[1] * wp;
        double b1b2 = b1 * b2;
        double b1pb2 = b1 + b2;
        double bco = 2 * wpSquared -8;

        if(type)
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
        else
        {
            differenceEQNCoeffs[0][0] = wpSquared * wpSquared;
            differenceEQNCoeffs[0][1] = 4 * differenceEQNCoeffs[0][0];
            differenceEQNCoeffs[0][2] = 6 * differenceEQNCoeffs[0][0];
            differenceEQNCoeffs[0][3] = differenceEQNCoeffs[0][1];
            differenceEQNCoeffs[0][4] = differenceEQNCoeffs[0][0];
        }
        
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
    else
    {
        double b = secondOrderCoeff * wp;
        if(type)
        {
            if(typeChanged) //Only update if type or order recently changed for high pass
            {
                differenceEQNCoeffs[0][0] = 4;
                differenceEQNCoeffs[0][1] = -8;
                differenceEQNCoeffs[0][2] = 4;
            }
        }
        else
        {
            differenceEQNCoeffs[0][0] = wpSquared;
            differenceEQNCoeffs[0][1] = 2 * differenceEQNCoeffs[0][0];
            differenceEQNCoeffs[0][2] = differenceEQNCoeffs[0][0];
        }
        
        differenceEQNCoeffs[1][0] = (-wpSquaredp4 + b);
        differenceEQNCoeffs[1][1] = -2 * wpSquared + 8;
        differenceEQNCoeffs[1][2] = wpSquaredp4 + b;
    }

}



float myIIRFilter::processNextSample(float nextSample)
{
    prevInput[delayLinePos] = nextSample;
    
    int delayDiff = delayLinePos - order;
    
    double output = 0;
    
    for(int i = 0 ; i < order+1; ++i)
    {
        int pos = delayDiff + i;
        if(pos < 0)
        {
            pos = pos + 10;
            
        }
        
        output = output + differenceEQNCoeffs[0][i] * prevInput[pos];
    }
    
    for(int i = 0 ; i < order; ++i)
    {
        int pos = delayDiff + i;
        if(pos < 0)
        {
            pos = pos + 10;
            
        }
        output = output + differenceEQNCoeffs[1][i] * prevOutput[pos];
    }
    
    output = output / differenceEQNCoeffs[1][order];
    
    prevOutput[delayLinePos] = output;
    
    delayLinePos++;
    if(delayLinePos > 9)
    {
        delayLinePos = 0;
    }
    
    return output;
}


bool myIIRFilter::setOrder(bool newMinus24DbMode)
{
    if(minus24dbMode != newMinus24DbMode)
    {
        minus24dbMode = newMinus24DbMode;
        order = minus24dbMode ? 4 : 2;
        return true;
    }
    return false;
}

bool myIIRFilter::setType(bool highPassMode)
{
    if(highPassMode != type)
    {
        type = highPassMode;
        return true;
    }
    
    return false;
}

void myIIRFilter::setFilterParams(float newCutOffFreq, bool newMinus24DbMode, bool highPassMode)
{
    bool updateOrderType = false;
    if(setType(highPassMode) || setOrder(newMinus24DbMode))
    {
        updateOrderType = true;
    }
    
    if(setCutOffFreq(newCutOffFreq) || updateOrderType)
    {
        calcDiffEqnCoeffs(updateOrderType);
    }
}

void myIIRFilter::setFilterParams(float newCutOffFreq, bool newMinus24DbMode)
{
    setFilterParams(newCutOffFreq, newMinus24DbMode, type);
}

void myIIRFilter::setFilterType(bool highPassMode)
{
    if(setType(highPassMode))
    {
        calcDiffEqnCoeffs(true);
    }
}

void myIIRFilter::setFilterOrder(float newMinus24DbMode)
{
    if(setOrder(newMinus24DbMode))
    {
        calcDiffEqnCoeffs(true);
    }
}

void myIIRFilter::setFilterCutOffFreq(float newCutOffFreq)
{
    if(setCutOffFreq(newCutOffFreq))
    {
        calcDiffEqnCoeffs(false);
    }
}
