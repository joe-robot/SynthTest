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
    ~EnvolopeParams();
    
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
            newVals = !newVals; //Switch value of newVals
        }
        
    }
    
    bool getValSwitch()
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
    
    bool newVals = false;
    
};


class OscParams
{
public:
    OscParams();
    ~OscParams();
    
    bool getValSwitch()
    {
        return newVals;
    }
    
    void setOscParams(float tune, float minAmp, float maxAmp)
    {
        bool changed = false;;
        if(oscParam[0] != tune || oscParam[1] != minAmp || oscParam[2] != maxAmp)
        {
            changed = true;
        }
        
        if(changed)
        {
            oscParam[0] = tune;
            oscParam[1] = minAmp;
            oscParam[2] = maxAmp;
            
            newVals = !newVals;
        }
    };
    
    float getOscParams(int oscParamNum)
    {
        return oscParam[oscParamNum];
    };
    
private:
    float oscParam[3] = {0.0f, 0.5f, 0.9f};
    
    bool newVals = false;
};
