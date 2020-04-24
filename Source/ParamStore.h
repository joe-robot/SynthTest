/*
  ==============================================================================

    ParamStore.h
    Contains classes to store parameters, envolope parameters and general parameters
    Created: 17 Apr 2020
    Author:  B159113

  ==============================================================================
*/

#pragma once
//Inclufin juce header
#include <JuceHeader.h>

// =================================
// =================================
// Envolope Params

/*!
 @class EnvolopeParams
 @abstract class to store envolope parameter in a ADSR
 @discussion multiple created for each envolope
 
 @namespace none
 @updated 2020-04-24
 */
class EnvolopeParams
{
public:
    //==============================================================================
    /** Constructor*/
    EnvolopeParams()
    {
        setADSRParams();    //Intialise by setting ADSR params
    };
    /**Destructor*/
    ~EnvolopeParams(){};
    //==============================================================================
    
    /**
     * Set envolope parameters
     *
     * @param attackMS attack in ms
     * @param decayMS decay in ms
     * @param sustain sustain in percentage
     * @param relaseMS release in ms
     *
    */
    void setParams(float attackMS, float decayMS, float sustain, float releaseMS)
    {
        bool changed = false;
        if(compareVals(attackMS, currentADSRms[0])||compareVals(decayMS, currentADSRms[1])||compareVals(sustain, currentADSRms[2])||compareVals(releaseMS, currentADSRms[3]))   //If any parameter has changed
        {
            changed = true; //Mark parameters as changed
        }
        
        if(changed == true) // If params changed update array of ADSRs in MS and update ADSR params
        {
            currentADSRms[0] = attackMS;
            currentADSRms[1] = decayMS;
            currentADSRms[2] = sustain;
            currentADSRms[3] = releaseMS;
            setADSRParams();
            newVals = (newVals + 1) % 4; //Switch value of newVals
        }
        
    }
    
    /**
     * Get value switch
     *
     * @return valueSwitch value from 0 -> 4
     *
    */
    int getValSwitch()
    {
        return newVals;
    }
    
    /**
     * Get envolope parameters
     *
     * @return returns ADSR params
     *
    */
    ADSR::Parameters getADSRParams()
    {
        return envolope;
    };
    
private:
    /**
     * Method to compare two values to check if they are the same
     *
     * @param newVal new Value
     * @param oldVal old Value
     *
    */
    bool compareVals(float newVal, float oldVal)
    {
        if(newVal != oldVal)
        {
            return true;
        }
        
        return false;
    }
    
    /**
     * Set envolope ADSR parameters using current ADSR in ms array
     *
    */
    void setADSRParams()
    {
        envolope.attack = currentADSRms[0] / 1000.0f;
        envolope.decay = currentADSRms[1] / 1000.0f;
        envolope.sustain = currentADSRms[2] / 100.0f;
        envolope.release = currentADSRms[3] / 1000.0f;
    }
    
    ADSR::Parameters envolope;  //Stored ADSR params
    
    float currentADSRms[4] = {100.0f, 100.0f, 100.0f, 100.0f};  //Array containing current ADSR in ms
    
    int newVals = 0;    //Value switch intially set to 0
    
};

// =================================
// =================================
// Simple Params

/*!
 @class SimpleParams
 @abstract class to store simple parameters
 @discussion multiple created for simple parameter
 
 @namespace none
 @updated 2020-04-24
 */
class SimpleParams
{
public:
    //==============================================================================
    /** Constructor*/
     SimpleParams(int newNumChoiceParams, float numNumParams)
    {
        //Intialise int and float vectors with number choice parameters and number float parameters respectively
        numParams = numNumParams;
        numChoiceParams = newNumChoiceParams;
        params.resize(numNumParams, 0);
        choiceParams.resize(numChoiceParams, 0);
    };
    /** Destructor*/
    ~SimpleParams(){};
    //==============================================================================
    
    /**
     * Get value switch
     *
     * @return valueSwitch value from 0 -> 4
     *
    */
    int getValSwitch()
    {
        return newVals;
    }
        
    /**
     * Setting float parameters
     *
     * @param newParams to set float parameters
     *
    */
    void setParams(float* newParams)
    {
        if(checkParams(params, newParams))  //Check parameters changed
        {
            newVals = (newVals + 1) % 4;    //Update value switch
        }
    };
    
    /**
     * Setting float and int parameters
     *
     * @param newChoice Params to set int parameters
     * @param newParams to set float parameters
     *
    */
    void setParams(int* newChoiceParams, float* newParams)
    {
        if(numChoiceParams !=0) //If there are any choice parameters
        {
            if(checkParams(choiceParams, newChoiceParams))  //Check if they have changed
            {
                newVals = (newVals + 1) % 4;   //If changed then updated value switch
            }
        }

        setParams(newParams);   //Do the same for the float parameters
    }
     
    /**
     * Checking parameter arrays are equal
     *
     * @param params2Check is the previously set parameters vector
     * @param arrayNewParams is the nuwly updated params array
     *
     * @return true if paramters changed, otherwise false
     *
    */
    template<typename T>
    bool checkParams(std::vector<T>& params2Check, T* arrayNewParams)
    {
        for(int i = 0; i < params2Check.size(); ++i)    //Itereate through all parameters
        {
            if(params2Check[i] != arrayNewParams[i])    //Check if changed
            {
                for(int j = i; j < params2Check.size(); ++j)    //Update all non-checked parameters if changed
                {
                    params2Check[j] = arrayNewParams[j];
                }
                return true;    //Return true if changed
            }
        }
        
        return false;   //Return false if unchanged
        
    }
           
    /**
     * get float parameters
     *
     * @param paramNum the parameter number
     * @return float parameter value
     *
    */
    float getParams(int paramNum)
    {
        return params[paramNum];
    };
    
    /**
     * get Choice parameters
     *
     * @param paramNum the parameter number
     * @return choice (int) parameter value
     *
    */
    int getChoiceParams(int paramNum)
    {
        return choiceParams[paramNum];
    };
        
private:
    int newVals = 0; //Set value switch to 0 initally
    
    //Vector for storing parameters
    std::vector<float> params;
    std::vector<int> choiceParams;
    
    //Parameters for storing number of each type
    int numParams = 0;
    int numChoiceParams = 0;
};
