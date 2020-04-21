/*
  ==============================================================================

    XYEnvolopedOscs.h
    Created: 21 Apr 2020 11:02:03am
    Author:  Joseph Cresswell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SmoothChanger.h"
#include "Oscillator.h"


class XYEnvolopedOscs
{
public:
    XYEnvolopedOscs();
    ~XYEnvolopedOscs();
    
    void setSampleRate(float sampleRate);
    void setOscType(int oscNum, int oscType);
    
    void setOscMinMaxVolume(int oscNum, float minVol, float maxVol);
    void setOscMinVol(int oscNum, float minVol);
    void setOscMaxVol(int oscNum, float maxVol);
    
    void setPanAmount(int oscNum, float newPanAmount);
    
    void setOscsMidiInput(int midiNote);
    
    void setTuneAmount(int oscNum, int newTuneAmount);
    
    void getNextVal(float envs[2], float outSamples[2]);
    
    void playMode(bool playMode);
    
    
private:
    
    float pan(float newPanAmount, int channel);
    
    void setOscFrequency(int oscNum, float frequency);
    
    void updateFreq();
    
    void resetParams();
    
    OwnedArray<Oscillator> oscs;
    float minMaxVols[2][4]  =  {{0.1f, 0.1f, 0.1f, 0.1f},   //Min Oscillator Volumes
                                {0.5f, 0.5f, 0.5f, 0.5f}};  //Max Oscillator Volumes
    
    int tuneAmount[4] = {0, 0, 0, 0};
    int targetTuneAmount[4] = {0, 0, 0, 0};
    
    float panAmount[4] = {0, 0, 0, 0};
    
    int prevMidiInput = 48;
    
    bool playing = false;
    bool changeFreq[4] = {false, false, false, false};
    
    OwnedArray<SmoothChanges> smoothFreq;
    
};
