/*
  ==============================================================================

    XYEnvolopedOscs.cpp
    Generates a 4 source sound source that changes bettween 4 sources using an
    input X, Y value
    Created: 19 Apr 2020
    Author:  B159113

  ==============================================================================
*/

#include "XYEnvolopedOscs.h"

XYEnvolopedOscs::XYEnvolopedOscs()
{
    for(int i = 0; i < 4; ++i)      //Creating all synth sources and frequency smoothing parameters
    {
        auto* oscillator = oscs.add(new SynthSources());
        oscillator -> setType(i);
        smoothFreq.add(new SmoothChanges());
    }
}

XYEnvolopedOscs::~XYEnvolopedOscs(){}

void XYEnvolopedOscs::setSampleRate(float sampleRate)
{
    for(auto* oscillator : oscs) //Set oscillator samplerates
        oscillator -> setSampleRate(sampleRate);
    
    for(auto* smoother : smoothFreq) //Set smoother sample rate
        smoother -> setSampleRate(sampleRate);
}

void XYEnvolopedOscs::setOscsMidiInput(int midiNote)
{
    prevMidiInput = midiNote;   //Update prev midinote
    for(int i = 0; i < 4; ++i)  //Update frequency for all souces including the tune amount
        setOscFrequency(i , MidiMessage::getMidiNoteInHertz(prevMidiInput + tuneAmount[i]));
}

void XYEnvolopedOscs::setTuneAmount(int oscNum, int newTuneAmount) //MAY NEED TO SMOOTH THIS
{
    if(!playing) //Not playing then update tune amount without smoothing
    {
        tuneAmount[oscNum] = newTuneAmount;
    }
    else //Otherwise smooth tune amount change
    {
        if(newTuneAmount != targetTuneAmount[oscNum]) //It tune amount changed
        {
            targetTuneAmount[oscNum] = newTuneAmount; //Updated tune amount
            smoothFreq[oscNum] -> init(MidiMessage::getMidiNoteInHertz(prevMidiInput + tuneAmount[oscNum]), MidiMessage::getMidiNoteInHertz(prevMidiInput + newTuneAmount)); //initslise smoothed freq
            changeFreq[oscNum] = true;
        }
    }
}

void XYEnvolopedOscs::setPanAmount(int oscNum, float newPanAmount)
{
    int absPanAmount = abs(newPanAmount); //absolute panning amount
    panAmount[oscNum] = absPanAmount > 1 ? absPanAmount/newPanAmount : newPanAmount; //Set pan amount (only bettween -1 and 1)
}

void XYEnvolopedOscs::setSourceType(int oscNum, int oscType)
{
    oscs[oscNum] -> setType(oscType); //Set Source type
}

void XYEnvolopedOscs::setOscMinMaxVolume(int oscNum, float minVol, float maxVol)
{
    setOscMinVol(oscNum, minVol); //updating min val
    setOscMaxVol(oscNum, maxVol); //updating max val
}

void XYEnvolopedOscs::setOscMinVol(int oscNum, float minVol)
{
    minMaxVols[0][oscNum] = minVol; //setting min val
}

void XYEnvolopedOscs::setOscMaxVol(int oscNum, float maxVol)
{
    minMaxVols[1][oscNum] = maxVol; //Setting max val
}

void XYEnvolopedOscs::getNextVal(float *envs, float *outSamples)
{
    updateFreq();  //Updating osc frequency
    
    float oscSample;
    
    float envResults[4] = {1 - envs[0], envs[0], 1 - envs[1], envs[1]}; //Possible env results
    
    for(int i = 0; i < 4; ++i)  //Get osc samples and adding them to thre output samples
    {
        int envResult1 = (i % 2);       //Getting which env result to use
        int envResult2 = i < 2 ? 2 : 3;
        oscSample = (minMaxVols[0][i] + (minMaxVols[1][i] - minMaxVols[0][i]) * envResults[envResult1] * envResults[envResult2]) * oscs[i] ->getNextSample();   //Calculating oscilaltor samples
        for(int j = 0; j < 2; ++j)
            outSamples[j] = outSamples[j] + oscSample * pan(panAmount[i], j);   //Updating output samples for each channel
    }

}

float XYEnvolopedOscs::pan(float newPanAmount, int channel)
{
    float rightPan = (newPanAmount + 1.0f)/2.0f;    //Calculating right pan

    return channel < 1 ? 1.0f - rightPan : rightPan;    //Outputting pan amount
}

void XYEnvolopedOscs::updateFreq()
{
    for(int i = 0; i < 4; ++i) //iterating thorough all sources
    {
        if(changeFreq[i])   //If frequency changing
        {
            if(smoothFreq[i] -> checkChanging())    //Check smooth value not at target
            {
                setOscFrequency(i, smoothFreq[i] -> getNextVal());  //Update osc freq with new freq
            }
            else
            {
                tuneAmount[i] = targetTuneAmount[i];    //If at target set tune amount to targer and stop changing frequency
                changeFreq[i] = false;
            }
        }
    }
}


void XYEnvolopedOscs::playMode(bool playMode)
{
    if(playing && !playMode)    //If not in playmode any longer reset parameters
        resetParams();

    playing = playMode;
}

void XYEnvolopedOscs::setOscFrequency(int oscNum, float frequency)
{
    oscs[oscNum] -> setFrequency(frequency);    //Update source frequency
}

void XYEnvolopedOscs::resetParams()
{
    for(int i = 0; i < 4; ++i)  //Iterate through all oscillators
    {
        if(changeFreq[i])   //If their frequency is still changing set frequency to target
        {
            smoothFreq[i] -> setToTarget();
            tuneAmount[i] = targetTuneAmount[i];
        }
    }
}
