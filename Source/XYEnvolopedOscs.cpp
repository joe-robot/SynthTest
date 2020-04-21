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
    }
}

XYEnvolopedOscs::~XYEnvolopedOscs(){}

void XYEnvolopedOscs::setSampleRate(float sampleRate)
{
    for(auto* oscillator : oscs)
        oscillator -> setSampleRate(sampleRate);
}

void XYEnvolopedOscs::setOscsMidiInput(int midiNote)
{
    prevMidiInput = midiNote;
    for(int i = 0; i < 4; ++i)
        oscs[i] -> setFrequency(MidiMessage::getMidiNoteInHertz(prevMidiInput + tuneAmount[i]));
}

void XYEnvolopedOscs::setTuneAmount(int oscNum, int newTuneAmount) //MAY NEED TO SMOOTH THIS
{
    tuneAmount[oscNum] = newTuneAmount;
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
    float osc0Sample = (minMaxVols[0][0] + (minMaxVols[1][0] - minMaxVols[0][0]) * (1 - envs[0]) * (1 - envs[1])) * (oscs[0] -> getNextSample());
    
    float osc1Sample = (minMaxVols[0][1] + (minMaxVols[1][1] - minMaxVols[0][1]) * (1 - envs[0]) * (1 - envs[1])) * (oscs[1] -> getNextSample());
    
    float osc3Sample = (minMaxVols[0][2] + (minMaxVols[1][2] - minMaxVols[0][2]) * (1 - envs[0]) * (1 - envs[1])) * (oscs[2] -> getNextSample());
    
    float osc4Sample = (minMaxVols[0][3] + (minMaxVols[1][3] - minMaxVols[0][3]) * (1 - envs[0]) * (1 - envs[1])) * (oscs[3] -> getNextSample());
    
    outSamples[0] = osc0Sample * panAmount[0];
}
