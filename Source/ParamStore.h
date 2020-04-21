/*
  ==============================================================================

    ParamStore.h
    Created: 11 Apr 2020 10:33:10am
    Author:  Joseph Cresswell

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class EnvolopeParams
{
public:
    EnvolopeParams()
    {
        setADSRParams();
    };
    ~EnvolopeParams(){};
    
    void setParams(float attackMS, float decayMS, float sustain, float releaseMS)
    {
        bool changed = false;
        if(compareVals(attackMS, currentADSRms[0])||compareVals(decayMS, currentADSRms[1])||compareVals(sustain, currentADSRms[2])||compareVals(releaseMS, currentADSRms[3]))
        {
            changed = true;
        }
        
        if(changed == true)
        {
            currentADSRms[0] = attackMS;
            currentADSRms[1] = decayMS;
            currentADSRms[2] = sustain;
            currentADSRms[3] = releaseMS;
            setADSRParams();
            newVals = (newVals + 1) % 4; //Switch value of newVals
        }
        
    }
    
    int getValSwitch()
    {
        return newVals;
    }
    
    ADSR::Parameters getADSRParams()
    {
        return envolope;
    };
    
private:
    ADSR::Parameters envolope;
    
    float currentADSRms[4] = {100.0f, 100.0f, 100.0f, 100.0f};
    
    
    bool compareVals(float newVal, float oldVal)
    {
        if(newVal != oldVal)
        {
            return true;
        }
        
        return false;
    }
    
    void setADSRParams()
    {
        envolope.attack = currentADSRms[0] / 1000.0f;
        envolope.decay = currentADSRms[1] / 1000.0f;
        envolope.sustain = currentADSRms[2] / 100.0f;
        envolope.release = currentADSRms[3] / 1000.0f;
    }
    
    int newVals = 0;
    
};


class OscParams
{
public:
    OscParams(){};
    ~OscParams(){};
    
    int getValSwitch()
    {
        return newVals;
    }
    
    void setParams(int newSourceType, float tune, float pan, float minAmp, float maxAmp)
    {
        bool changed = false;
        if(oscParam[0] != tune || oscParam[1] != pan || oscParam[2] != minAmp/100.0f || oscParam[3] != maxAmp/100.0f || sourceType != newSourceType)
        {
            changed = true;
        }
        
        if(changed)
        {
            oscParam[0] = tune;
            oscParam[1] = pan;
            oscParam[2] = minAmp/100.0f;
            oscParam[3] = maxAmp/100.0f;
            sourceType = newSourceType;
            
            newVals = (newVals + 1) % 4;
        }
        
        if(newSourceType == sourceType)
        {
            sourceType = newSourceType;
        }
        
    };
    
    float getOscParams(int oscParamNum)
    {
        return oscParam[oscParamNum];
    };
    
    int getSourceType()
    {
        return sourceType;
    }
    
private:
    float oscParam[4] = {0.0f, 0.0f, 0.5f, 0.9f};
    int sourceType = 1;
    int newVals = 0;
    bool sourceChanged = true;
};
