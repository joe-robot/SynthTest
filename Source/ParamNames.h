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

class ParamNames
{
public:
    ParamNames(){};
    ~ParamNames(){};

    std::string getEnvolopeParamName(int envNum, int envolopePos)
    {
        return envNames[envNum] + adsrNames[envolopePos];
    };
    
    std::string getOscParamName(int oscNum, int oscParamNum)
    {
        return oscNames[oscNum] + oscParamNames[oscParamNum];
    };
    
    std::string getLfoParamName(int lfoNum, int lfoParamNum)
    {
        return lfoNames[lfoNum] + lfoParamNames[lfoParamNum];
    };
    
    std::string getFilterParamName(int filterNum, int filterParamNum)
    {
        return filterNames[filterNum] + filterParamNames[filterParamNum];
    };
    
    std::string getMaxParamName(int paramNum)
    {
        return maxParams[paramNum];
    }
    
    std::string typesOfOscs[6] = {"None", "Sine", "Saw", "Triangle", "Square", "Noise"};
    
    int numMaxParams = 12;
    
private:
    std::string envNames[8] =
    {
        "",
        "oscX",
        "oscY",
        "custEnv1",
        "custEnv2",
        "custEnv3",
        "custEnv4",
        "custEnv5"
        
    };
    
    std::string adsrNames[5] =
    {
        "attack",
        "decay",
        "sustain",
        "release",
        "Choice"
    };
    std::string oscNames[4]
    {
        "osc1",
        "osc2",
        "osc3",
        "osc4"
    };
    
    
    std::string oscParamNames[5]
    {
        "Source",
        "Tune",
        "Pan",
        "MinAmp",
        "MaxAmp"
    };
    
    std::string lfoNames[2]
    {
      "lfo1"
    };
    
    std::string lfoParamNames[2]
    {
      "Depth",
      "Freq"
    };
    
    std::string filterNames[2]
    {
      "lpFilter",
      "hpFilter"
    };
    
    std::string filterParamNames[2]
    {
      "Mode",
      "Freq"
    };
    
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
