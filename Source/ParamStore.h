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
};

/*class LFOParams
{
    LFOParams(){};
    ~LFOParams(){};
    
    int getValSwitch()
    {
        return newVals;
    }
    
    void setParams(float amp, float freq)
    {
        bool changed = false;
        if(lfoParams[0] != amp || lfoParams[1] != freq)
        {
            changed = true;
        }
        
        if(changed)
        {
            lfoParams[0] = amp;
            lfoParams[1] = freq;
            
            newVals = (newVals + 1) % 4;
        }
        
        
    };
    
    float getLFOParams(int lfoParamNum)
    {
        return lfoParams[lfoParamNum];
    };
    
private:
    float lfoParams[2] = {0.0f, 10.0f};
    int newVals = 0;
    
};*/



class SimpleParams
{
public:
     SimpleParams(int newNumChoiceParams, float numNumParams)
    {
        numParams = numNumParams;
        numChoiceParams = newNumChoiceParams;
        params.resize(numNumParams, 0);
        choiceParams.resize(numChoiceParams, 0);
    };
    ~SimpleParams(){};
        
    int getValSwitch()
    {
        return newVals;
    }
    
    int getChoiceValSwitch()
    {
        return newChoiceVals;
    }
        
    void setParams(float* newParams)
    {
        if(checkParams(params, newParams))
        {
            newVals = (newVals + 1) % 4;
        }
    };
    
    void setParams(int* newChoiceParams, float* newParams)
    {
        if(numChoiceParams !=0)
        {
            if(checkParams(choiceParams, newChoiceParams))
            {
                newVals = (newVals + 1) % 4;   ///TEMP    EKPOEJFIJEWIOFJEWIOFJIOEWJFIOJEO
                newChoiceVals = (newChoiceVals + 1) % 4;
            }
        }
        
        setParams(newParams);
    }
        
    template<typename T>
    bool checkParams(std::vector<T>& params2Check, T* arrayNewParams)
    {
        for(int i = 0; i < params2Check.size(); ++i)
        {
            if(params2Check[i] != arrayNewParams[i])
            {
                for(int i = 0; i < params2Check.size(); ++i)
                {
                    params2Check[i] = arrayNewParams[i];
                }
                return true;
            }
        }
        
        return false;
        
    }
           
           
    float getParams(int paramNum)
    {
        return params[paramNum];
    };
    
    int getChoiceParams(int paramNum)
    {
        return choiceParams[paramNum];
    };
        
private:
    float lfoParams[2] = {0.0f, 10.0f};
    int newVals = 0;
    int newChoiceVals = 0;
    
    std::vector<float> params;
    std::vector<int> choiceParams;
    int numParams = 0;
    int numChoiceParams = 0;
};
