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
    
    
    std::string oscParamNames[3]
    {
        "Tune",
        "MinAmp",
        "MaxAmp"
    };

};
