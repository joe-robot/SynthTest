/*
  ==============================================================================

    ParamNames.h
    Contains all parameter names for quick access to parameters
    Created: 17 Apr 2020
    Author:  B159113

  ==============================================================================
*/

#pragma once

#include <stdio.h>  //Including the standard library

// =================================
// =================================
// Param Names

/*!
 @class ParamNames
 @abstract stores all names of parameters
 @discussion used to reference parameter names and access parameter values
 
 @namespace none
 @updated 2020-04-24
 */
class ParamNames
{
public:
    //==============================================================================
    /** Constructor*/
    ParamNames(){};
    /** Destructor*/
    ~ParamNames(){};
    //==============================================================================

    /**
     * Get Envolope Parameter Names
     *
     * @param envNum the envolope number
     * @param envolopePos the envolope parameter number
     *
     * @return returns a string containing concatanation of envolope name and parameter
     *
    */
    std::string getEnvolopeParamName(int envNum, int envolopePos)
    {
        return envNames[envNum] + adsrNames[envolopePos];
    };
    
    /**
     * Get Oscillator Parameter Names
     *
     * @param oscNum the oscillator number
     * @param oscParamNum the oscilaltor parameter number
     *
     * @return returns a string containing concatanation of oscillator name and parameter
     *
    */
    std::string getOscParamName(int oscNum, int oscParamNum)
    {
        return oscNames[oscNum] + oscParamNames[oscParamNum];
    };
    
    /**
     * Get LFO Parameter Names
     *
     * @param lfoNum the lfo number
     * @param lfoParamNum the lfo parameter number
     *
     * @return returns a string containing concatanation of lfo name and parameter
     *
    */
    std::string getLfoParamName(int lfoNum, int lfoParamNum)
    {
        return lfoNames[lfoNum] + lfoParamNames[lfoParamNum];
    };
    
    /**
     * Get Filter Parameter Names
     *
     * @param filterNum the filter number
     * @param filterParamNum the filter parameter number
     *
     * @return returns a string containing concatanation of filter name and parameter
     *
    */
    std::string getFilterParamName(int filterNum, int filterParamNum)
    {
        return filterNames[filterNum] + filterParamNames[filterParamNum];
    };
    
    /**
     * Get Max Parameter Name
     *
     * @param paramNum the max parameter number
     *
     * @return returns a string of max parameter name
     *
    */
    std::string getMaxParamName(int paramNum)
    {
        return maxParams[paramNum];
    }
    
    //Oscillator types string array
    std::string typesOfOscs[6] = {"None", "Sine", "Saw", "Triangle", "Square", "Noise"};
    
    int numMaxParams = 12; //Number of max parameters
    
private:
    //Array containing envolope names
    std::string envNames[8] =
    {
        "",
        "oscX",
        "oscY",
        "paramEnv1",
        "paramEnv2",
        "paramEnv3",
        "paramEnv4",
        "paramEnv5"
        
    };
    
    //Array containing envolope parameter names
    std::string adsrNames[5] =
    {
        "attack",
        "decay",
        "sustain",
        "release",
        "Choice"
    };
    
    //Array containing oscillator names
    std::string oscNames[4]
    {
        "osc1",
        "osc2",
        "osc3",
        "osc4"
    };
    
    //Array containing oscillator parameter names
    std::string oscParamNames[5]
    {
        "Source",
        "Tune",
        "Pan",
        "MinAmp",
        "MaxAmp"
    };
    
    //Array containing lfo names
    std::string lfoNames[2]
    {
      "lfo1"
    };
    
    //Array containing lfo parameter names
    std::string lfoParamNames[2]
    {
      "Depth",
      "Freq"
    };
    
    //Array containing filter names
    std::string filterNames[2]
    {
      "lpFilter",
      "hpFilter"
    };
    
    //Array containing filter parameter names
    std::string filterParamNames[2]
    {
      "Mode",
      "Freq"
    };
    
    //Array containing fmax parameter names
    std::string maxParams[12]
    {
        "osc1TuneMax",
        "osc1PanMax",
        "osc2TuneMax",
        "osc1PanMax",
        "osc3TuneMax",
        "osc3PanMax",
        "osc4TuneMax",
        "osc4PanMax",
        "lfo1DepthMax",
        "lfo1FreqMax",
        "lpFilterFreqMax",
        "hpFilterFreqMax"
    };

};
