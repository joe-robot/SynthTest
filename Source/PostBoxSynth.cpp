

#pragma once

#include "Oscillator.h"
#include "SmoothChanger.h"
#include "ParamStore.h"
#include "XYEnvolopedOscs.h"
#include "myIIRFilter.h"

#include "MyFirstSynth.h"

// =================================
// =================================
// Synthesiser Voice - your synth code goes in here

/*!
 @class MyFirstSynthVoice
 @abstract struct defining the DSP associated with a specific voice.
 @discussion multiple MyFirstSynthVoice objects will be created by the Synthesiser so that it can be played polyphicially
 
 @namespace none
 @updated 2019-06-18
 */

PostBoxSynth::PostBoxSynth(int numOscs, int numEnvs, int numFilters)
{
        smoothEnvParams.clear();
        smoothOscParams.clear();
        for(int i = 0; i < numOscs; ++i)  //Initialising an owned array of parameter smoothers
        {
            smoothOscParams.add(new MultiSmooth(4));
        }
        
        for(int i = 0; i < numEnvs; ++i)  //Initialising an owned array of parameter smoothers
        {
            smoothEnvParams.add(new MultiSmooth());
            myEnvs.add(new ADSR());
        }
        
        for(int i = 0; i < numFilters; ++i)
        {
            auto* filter = synthFilters.add(new StereoIIRFilters());
            filter -> setFilterType(i == 1);
            smoothFilterParams.add(new SmoothChanges());
        }
        
        smoothLFOParams.add(new MultiSmooth(2));
        
}
    //--------------------------------------------------------------------------
    
    /**
    Initialising the voice of the synth with required paramters

    @param sampleRate
    */
void PostBoxSynth::init(float sampleRate)
{
    for(int i = 0; i < smoothOscParams.size(); ++i)
    {
        smoothOscParams[i] -> setSampleRate(sampleRate);
        sourceOscs.setSampleRate(sampleRate);
    }
        
    for(int i = 0; i < smoothEnvParams.size(); ++i)
    {
        smoothEnvParams[i] -> setSampleRate(sampleRate);
        myEnvs[i] -> setSampleRate(sampleRate);
    }
        
    for(int i = 0; i < smoothFilterParams.size(); ++i)
    {
        smoothFilterParams[i] -> setSampleRate(sampleRate);
        synthFilters[i] -> setSampleRate(sampleRate);
    }
        
    smoothLFOParams[0] -> setSampleRate(sampleRate);
    lfoOsc.setSampleRate(sampleRate);
        
}

    //--------------------------------------------------------------------------

     
    //--------------------------------------------------------------------------
     

    
void PostBoxSynth::setParams(OwnedArray<EnvolopeParams>& envs, OwnedArray<OscParams>& oscs, OwnedArray<SimpleParams>& lfos, OwnedArray<SimpleParams>& filters, OwnedArray<SimpleParams>& customEnvsChoice)
{
    for(int i = 0; i < envs.size(); ++i)
    {
        if(envs[i] -> getValSwitch() != envUpdate[i])   //Check if env updated since last checked
        {
            updateEnv(i, envs[i] -> getADSRParams());
            envUpdate[i] = envs[i] -> getValSwitch();
        }
    }
        
    for(int i = 0; i < oscs.size(); ++i)
    {
        if(oscs[i] -> getValSwitch() != oscUpdate[i])   //Check if env updated since last checked
        {
            sourceOscs.setOscType(i, oscs[i] -> getSourceType());
            updateOsc(i, oscs[i] -> getOscParams(0), oscs[i] -> getOscParams(1), oscs[i] -> getOscParams(2), oscs[i] -> getOscParams(3));
            oscUpdate[i] = oscs[i] -> getValSwitch();
        }
    }
        
    for(int i = 0; i < lfos.size(); ++i)
    {
        if(lfos[i] -> getValSwitch() != lfoUpdate)
        {
            updateLFOs(lfos[i] -> getParams(0), lfos[i] -> getParams(1));
            lfoUpdate = lfos[i] -> getValSwitch();
        }
    }
        
    for(int i = 0; i < filters.size(); ++i)
    {
        if(filters[i] -> getValSwitch() != filterUpdate[i])
        {
            updateFilters(i, filters[i] -> getChoiceParams(0), filters[i] -> getParams(0));
            filterUpdate[i] = filters[i] -> getValSwitch();
        }
    }
        
    for(int i = 0; i < customEnvsChoice.size(); ++i)
    {
        if(customEnvsChoice[i] -> getValSwitch() != customEnvUpdate[i])
        {
            updateCustomEnvs(i, customEnvsChoice[i] -> getChoiceParams(0), customEnvsChoice[i] -> getParams(0));
            customEnvUpdate[i] = customEnvsChoice[i] -> getValSwitch();
        }
    }
}
    
void PostBoxSynth::updateFilters(int filterNum, int filterMode, float filterFreq)
{
    if(filterMode != 0)
    {
        filterEnable[filterNum] = true;
        synthFilters[filterNum] -> setFilterOrder(filterMode==2); //TEPEOJEOIJIOEJF
    }
    else
    {
        filterEnable[filterNum] = false;
    }
        
    if(!playing || !filterEnable[filterNum])
    {
        smoothFilterParams[filterNum] -> init(filterFreq, filterFreq);
        synthFilters[filterNum] -> setFilterCutOffFreq(filterFreq);
    }
    else
    {
        smoothFilterParams[filterNum] -> setTargetVal(filterFreq);
    }
}
    
void PostBoxSynth::updateLFOs(float lfoAmp, float lfoFreq)
{
    float lfoPar[2] = {lfoAmp, lfoFreq};
    if(!playing)
    {
        smoothLFOParams[0] -> init(lfoPar, lfoPar);
        lfoOsc.setFrequency(lfoFreq);
    }
    else
    {
        smoothLFOParams[0] -> setTargetVal(lfoPar);
    }
}
    
void PostBoxSynth::updateEnv(int envNum, ADSR::Parameters thisADSR)
{
    float adsrParams[4] = {thisADSR.attack, thisADSR.decay, thisADSR.sustain, thisADSR.release};

    //Update env differently if playing or not playing
    if(!playing)
    {
        smoothEnvParams[envNum] -> init(adsrParams ,adsrParams);
        setADSR(envNum, adsrParams);
    }
    else
    {
        //std::cout<<"should set target here for env: "<< envNum<<std::endl;
        smoothEnvParams[envNum] -> setTargetVal(adsrParams);
        //std::cout << smoothEnvParams[envNum] -> checkChanging() << std::endl;
    }
}
        
void PostBoxSynth::updateOsc(int oscNum, float newTune, float newPan, float newMinAmp, float newMaxAmp)
{
    float oscParams[4] = {newTune, newPan, newMinAmp, newMaxAmp};
        
    //Update osc differently if playing or not playing
    if(!playing)
    {
        //std::cout << "updatepls" << std::endl;
        smoothOscParams[oscNum] -> init(oscParams, oscParams);
    }
    else
    {
        smoothOscParams[oscNum] -> setTargetVal(oscParams);
    }
}
    

     //--------------------------------------------------------------------------
     /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
void PostBoxSynth::startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override
{
    //float freq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    noteVelocity = velocity;
    for(int i = 0; i < myEnvs.size(); ++i)
    {
        myEnvs[i] -> reset();
        myEnvs[i] -> noteOn();
    }
    sourceOscs.playMode(true);
    sourceOscs.setOscsMidiInput(midiNoteNumber);
        
    released = false;
    playing = true;
        
}
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
void PostBoxSynth::stopNote(float /*velocity*/, bool allowTailOff) override
{
    for(int i = 0; i < myEnvs.size(); ++i)
        myEnvs[i] -> noteOff();

    released = true;
}
    
    //--------------------------------------------------------------------------
    /**
     The Main DSP Block: Put your DSP code in here
     
     If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

     @param outputBuffer pointer to output
     @param startSample position of first sample in buffer
     @param numSamples number of smaples in output buffer
     */
void PostBoxSynth::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
{
    // iterate through the necessary number of samples (from startSample up to startSample + numSamples)
    for (int sampleIndex = startSample;   sampleIndex < (startSample+numSamples);   sampleIndex++)
    {
        float ampEnv = 0;
        float currentSample[2] = {0, 0};
        // your sample-by-sample DSP code here!
        // An example white noise generater as a placeholder - replace with your own code
        if(playing)
        {
            //Updating synth parameters
            updateParams();
               
            oscsNextSample(currentSample);
                
            applyFX(currentSample);
                
            ampEnv = myEnvs[0] -> getNextSample();
                
            if(released && ampEnv<0.0001f)
            {
                resetVoice();
            }
        }
        // for each channel, write the currentSample float to the output
        for (int chan = 0; chan<outputBuffer.getNumChannels(); chan++)
        {
            // The output sample is scaled by 0.2 and note velocity so that it is not too loud by default
            outputBuffer.addSample (chan, sampleIndex, ampEnv * currentSample[chan] * noteVelocity * 0.2);
        }
    }
}


void PostBoxSynth::oscsNextSample(float* sample)
{
    float envVals[2] = {myEnvs[1] -> getNextSample(), myEnvs[2] -> getNextSample()};

    sourceOscs.getNextVal(envVals, sample);
}
    
void PostBoxSynth::applyFX(float* sample)
{
    applyLFO(sample);
    applyFilter(sample);
}
    
    
    //Have a parameter for each parameter that is added on the end of the changes in update parameter method, which is changed based on the update other envolopes method
    
void PostBoxSynth::applyLFO(float* sample)
{
    if(lfoAmp > 0.0001f)
    {
        float lfoVal = lfoOsc.getNextSample() * lfoAmp;
        float lfoDepthInv = 1.0f - lfoAmp;
        
        for(int i = 0; i < 2; ++i)
            sample[i] = (lfoVal + lfoDepthInv) * sample[i];
    }
}
    
void PostBoxSynth::applyFilter(float* sample)
{
    for(int i = 0; i < 2; ++i)
    {
        if(filterEnable[i])
        {
            synthFilters[i] -> setFilterCutOffFreq(getParamVal(10 + i, smoothFilterParams[i] -> getNextVal()));
            synthFilters[i] -> getNextSample(sample);
        }
    }
}
    
void PostBoxSynth::resetVoice()
{
    clearCurrentNote();
    updateParams();
    playing = false;
    released = false;
    sourceOscs.playMode(false);
}
    
    /*
    Setting ADSR parameters as attribute values

    */
    
void PostBoxSynth::setADSR(int envNum, float adsrVals[4])
{
    //Setting ADSR parameters
    ADSR::Parameters myADSR;
        
    myADSR.attack = adsrVals[0];
    myADSR.decay = adsrVals[1];
    myADSR.sustain = adsrVals[2];
    myADSR.release = adsrVals[3];
        
    setADSR(envNum, myADSR);  //Setting envolope with passed parameters
        
}
    
void PostBoxSynth::setADSR(int envNum, ADSR::Parameters adsrParams)
{
    myEnvs[envNum] -> setParameters(adsrParams);   //Setting envolope with passed parameters
}
    
    
void PostBoxSynth::updateCustomEnvs(int customEnvNum, int envChoice, float paramResult)
{
    int oldEnvChoice = customEnvParamsChosen[customEnvNum];
    if(customEnvParamsChosen[customEnvNum] != (envChoice - 1))    //Checking if same as previously chosen param
    {
        if(oldEnvChoice != -1)  //Updating las chosen value paramters
        {
            numTimesChosen[oldEnvChoice] --;
            if(numTimesChosen[oldEnvChoice] < 1)
            {
                envolopedParam[oldEnvChoice] = false;
                numTimesChosen[oldEnvChoice] = 0;
            }
        }
                
        //Updating new paramters
        customEnvParamsChosen[customEnvNum] = envChoice - 1;
            
        if(envChoice > 0)
        {
            numTimesChosen[customEnvParamsChosen[customEnvNum]]++;
            envolopedParam[customEnvParamsChosen[customEnvNum]] = true;
            maxParamVals[customEnvParamsChosen[customEnvNum]] = paramResult
        }
    }
        
    else
    {
        if(oldEnvChoice != -1)
        {
            maxParamVals[oldEnvChoice] = paramResult;
        }
    }
}
    
void PostBoxSynth::getNextCustomEnvVals()
{
    std::vector<int> alreadyPickedVals;
    for(int i = 0; i < myEnvs.size()-3; ++i)
    {
        if( customEnvParamsChosen[i] > -1)
        {
            int chosenParam = customEnvParamsChosen[i];
            bool valAlreadyChosen = false;
            for( int val : alreadyPickedVals)
            {
                if(val == chosenParam)
                {
                    valAlreadyChosen = true;
                    break;
                }
            }
            if(valAlreadyChosen)
            {
                envolopedParamVals[chosenParam] = envolopedParamVals[chosenParam] * myEnvs[i+3] -> getNextSample();
            }
            else
            {
                envolopedParamVals[chosenParam] = myEnvs[i+3] -> getNextSample();
                alreadyPickedVals.push_back(chosenParam);
            }
        }
        
    }
        
        
}
    
float PostBoxSynth::getParamVal(int paramNum, float paramVal)
{
    if(envolopedParam[paramNum])
    {
        return (paramVal + (maxParamVals[paramNum] - paramVal) * envolopedParamVals[paramNum]);
    }
        
    return paramVal;
}

    
void PostBoxSynth::updateParams()
{
    getNextCustomEnvVals();
    updateOscParams();
    updateLFOParams();
        
    for(int i = 0; i < myEnvs.size(); ++i)
    {
        if(smoothEnvParams[i] -> checkChanging())
        {
            //std::cout<< "Therefore this should be changing: "<< i <<std::endl;
            float adsrVals[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            if(playing)
            {
                smoothEnvParams[i] -> getNextVal(adsrVals);
            }
            else
            {
                smoothEnvParams[i] -> setToTarget();
                smoothEnvParams[i] -> getNextVal(adsrVals);
            }
            //std::cout<<"wehere"<<std::endl;
            setADSR(i, adsrVals);
        }

    }
}
    
void PostBoxSynth::updateOscParams()
{
    for(int i = 0; i < 4; ++i)
    {
        float osc[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        smoothOscParams[i] -> getNextVal(osc);
        sourceOscs.setOscMinMaxVolume(i, osc[2], osc[3]);
        sourceOscs.setPanAmount(i, getParamVal(2 * i + 1, osc[1]));
        sourceOscs.setTuneAmount(i, getParamVal(2 * i, osc[0]));
    }
}
    
    
void PostBoxSynth::updateLFOParams()
{
    float lfoParams[2] = {0,0};
        
    smoothLFOParams[0] -> getNextVal(lfoParams);
        
    lfoOsc.setFrequency(getParamVal(9, lfoParams[1]));
        
    lfoAmp = getParamVal(8, lfoParams[0]);
}
  
