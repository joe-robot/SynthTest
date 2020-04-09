/*
  ==============================================================================

    Oscillator.cpp
    This class creates calualtes the next sample of an oscillator. The type of
    oscillator can be switched bettween 7 possible types.
    Created: 31 Jan 2020 3:40:51pm
    Author:  B159113

  ==============================================================================
*/

#include "Oscillator.h"

//==============================================

Oscillator::Oscillator(){} //Basic constructor, required values already intiallised in header

Oscillator::Oscillator(float newFrequency, float newSampleRate, int newType) //Constructor to initialise values of oscillator
{
    setType(newType);       //Setting type of oscillator
    setSampleRate(newSampleRate);   //Setting the sample Rate
    setFrequency(newFrequency);     //Setting the frequency
}

Oscillator::~Oscillator()   //Destructor
{
}

//==============================================

void Oscillator::setFrequency(float newFrequency)
{
    if(newFrequency > 0)             //If frequency bigger than zero than set it as passed value
    {
        frequency = newFrequency;
    }
    else                        //Otherwise set it as a defualt value
    {
        frequency = 220;
    }
    
    phaseDelta = frequency / sampleRate;      //Update phase delta for the new frequency
}

void Oscillator::setSampleRate(float newSampleRate)
{
    sampleRate=newSampleRate > 0 ? newSampleRate : 48000; //Check passed sample rate bigger than zero if not set as default value
    setFrequency(frequency);                            //If sample rate changes then the phaseDelta needs to be recalcualted
}



void Oscillator::setType(int newType)
{
    if(newType < 0 || newType > 6)        //If type is out of range set to default value
    {
        type = 0;
    }
    else                            //Otherwise set to passed value
    {
        type = newType;
    }
}

float Oscillator::getNextSample()
{
    float sample;
    switch(type)            //Switching what to calculate based on type of oscillator selected
    {
        case 0:{                    //Case 0 is a sine wave osc
            sample = sinWave();
            break;}
        case 1:{                    //Case 1 is a square wave osc
            sample = squareWave();
            break;}
        case 2:{                    //Case 2 is a triangle wave osc
            sample = triWave();
            break;}
        case 3:{                    //Case 3 is a phasor
            sample = phasor();
            break;}
        case 4:{                    //Case 4 is a sine wave osc that is 0 after 0.5
            sample = singleInitialBump();
            break;}
        case 5:{                    //Case 5 is case 4 shifted accross by 0.25 and 0 before
            sample = singleMiddleBump();
        break;}
        case 6:{                    //Case 5 is case 4 shifted accross by 0.5 and 0 before
            sample = singleEndBump();
        break;}
            
        default:{                   //Default case is a sine wave osc
            sample = sinWave();
            break;
        }
    }
       
    
    if((phasePos += phaseDelta)>1)        //Update the phase position, if exceeding 1, -1 from it's value
    {
        phasePos -= 1;
    }
    
    return sample;                      //Return calculated sample
}

float Oscillator::sinWave()
{
    return sin(2.0f * 3.141592653 * phasePos);      //Returning sine calculation
}

float Oscillator::squareWave()
{
    if(phasePos<0.5)                //If less than 0.5 output 1
    {
        return 1;
    }

    return -1;                      //Otherwise output -1
}

float Oscillator::triWave()
{
    return 4 * (fabs(phasePos - 0.5) - 0.25);     //Calculating triangle wave bettween -1 and 1
}

float Oscillator::phasor()
{
    return phasePos;            //Just returning the phase
}

float Oscillator::singleEndBump()
{
    if(phasePos > 0.5)            //If phase larger than 0.5 then calculate the phase shifted sine otherwise set to 0
    {
        return sin(2.0f * 3.141592653 * (phasePos - 0.5));
    }
    
    return 0;
}

float Oscillator::singleInitialBump()
{
    if(phasePos < 0.5)    //If phase smaller than 0.5 then calculate the sine otherwise set to 0
    {
        return sin(2.0f * 3.141592653 * (phasePos));
    }
    
    return 0;
}

float Oscillator::singleMiddleBump()
{
    if(phasePos > 0.25 && phasePos < 0.75)    //If phase larger than 0.25 then calculate the phase shifted sine and set to 0 before 0.25 and after phase of 0.75
    {
        return sin(2.0f * 3.141592653 * (phasePos - 0.25));
    }
    
    return 0;
}
