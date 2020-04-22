/*
  ==============================================================================

    ParamNames.h
    Created: 11 Apr 2020 10:04:21am
    Author:  Joseph Cresswell

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
    
    const std::string typesOfOscs[6] = {"None", "Sine", "Saw", "Triangle", "Square", "Noise"};
    
private:
    std::string envNames[5] =
    {
        "",
        "oscX",
        "oscY",
        "env1",
        "env2"
    };
    
    std::string adsrNames[4] =
    {
        "attack",
        "decay",
        "sustain",
        "release"
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

};
