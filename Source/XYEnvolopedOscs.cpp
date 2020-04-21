/*
  ==============================================================================

    XYEnvolopedOscs.cpp
    Created: 21 Apr 2020 11:02:03am
    Author:  Joseph Cresswell

  ==============================================================================
*/

#include "XYEnvolopedOscs.h"




XYEnvolopedOscs::XYEnvolopedOscs()
{
    for(int i = 0; i < 4; ++i)
    {
        auto* oscillator = oscs.add(new Oscillator());
        oscillator -> setType(i);
        smoothFreq.add(new SmoothChanges());
    }
}

XYEnvolopedOscs::~XYEnvolopedOscs(){}

void XYEnvolopedOscs::setSampleRate(float sampleRate)
{
    for(auto* oscillator : oscs)
        oscillator -> setSampleRate(sampleRate);
    
    for(auto* smoother : smoothFreq)
        smoother -> setSampleRate(sampleRate);
}

void XYEnvolopedOscs::setOscsMidiInput(int midiNote)
{
    prevMidiInput = midiNote;
    for(int i = 0; i < 4; ++i)
        setOscFrequency(i , MidiMessage::getMidiNoteInHertz(prevMidiInput + tuneAmount[i]));
}

void XYEnvolopedOscs::setTuneAmount(int oscNum, int newTuneAmount) //MAY NEED TO SMOOTH THIS
{
    
    if(!playing)
    {
        tuneAmount[oscNum] = newTuneAmount;
    }
    else
    {
        if(newTuneAmount != targetTuneAmount[oscNum])
        {
            targetTuneAmount[oscNum] = newTuneAmount;
            smoothFreq[oscNum] -> init(MidiMessage::getMidiNoteInHertz(prevMidiInput + tuneAmount[oscNum]), MidiMessage::getMidiNoteInHertz(prevMidiInput + newTuneAmount));
            changeFreq[oscNum] = true;
        }
    }
}

void XYEnvolopedOscs::setPanAmount(int oscNum, float newPanAmount)
{
    int absPanAmount = abs(newPanAmount);
    panAmount[oscNum] = absPanAmount > 1 ? absPanAmount/newPanAmount : newPanAmount;
}

void XYEnvolopedOscs::setOscType(int oscNum, int oscType)
{
    oscs[oscNum] -> setType(oscType);
}

void XYEnvolopedOscs::setOscMinMaxVolume(int oscNum, float minVol, float maxVol)
{
    setOscMinVol(oscNum, minVol);
    setOscMaxVol(oscNum, maxVol);
}

void XYEnvolopedOscs::setOscMinVol(int oscNum, float minVol)
{
    minMaxVols[0][oscNum] = minVol;
}

void XYEnvolopedOscs::setOscMaxVol(int oscNum, float maxVol)
{
    minMaxVols[1][oscNum] = maxVol;
}

void XYEnvolopedOscs::getNextVal(float *envs, float *outSamples)
{
    updateFreq();
    
    float oscSamples[4];
    
    float envResults[4] = {1 - envs[0], envs[0], 1 - envs[1], envs[1]};
    
    for(int i = 0; i < 4; ++i)
    {
        int envResult1 = (i % 2);
        int envResult2 = i < 2 ? 2 : 3;
        oscSamples[i] = (minMaxVols[0][i] + (minMaxVols[1][i] - minMaxVols[0][i]) * envResults[envResult1] * envResults[envResult2]) * oscs[i] ->getNextSample();
        for(int j = 0; j < 2; ++j)
            outSamples[j] = outSamples[j] + oscSamples[i] * pan(panAmount[i], j);
    }

}

float XYEnvolopedOscs::pan(float newPanAmount, int channel)
{
    float rightPan = (newPanAmount + 1.0f)/2.0f;

    return channel < 1 ? 1.0f - rightPan : rightPan;
}

void XYEnvolopedOscs::updateFreq()
{
    for(int i = 0; i < 4; ++i)
    {
        if(changeFreq[i])
        {
            if(smoothFreq[i] -> checkChanging())
            {
                setOscFrequency(i, smoothFreq[i] -> getNextVal());
            }
            else
            {
                tuneAmount[i] = targetTuneAmount[i];
                changeFreq[i] = false;
            }
        }
    }
}


void XYEnvolopedOscs::playMode(bool playMode)
{
    if(playing && !playMode)
        resetParams();

    playing = playMode;
}

void XYEnvolopedOscs::setOscFrequency(int oscNum, float frequency)
{
    oscs[oscNum] -> setFrequency(frequency);
}

void XYEnvolopedOscs::resetParams()
{
    for(int i = 0; i < 4; ++i)
    {
        if(changeFreq[i])
        {
            smoothFreq[i] -> setToTarget();
            tuneAmount[i] = targetTuneAmount[i];
        }
    }
}
