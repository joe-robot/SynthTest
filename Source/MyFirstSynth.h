/*
  ==============================================================================

    MyFirstSynthesiser.h
    Created: 7 Mar 2020 4:27:57pm
    Author:  Tom Mudd

  ==============================================================================
*/


#pragma once

#include "Oscillator.h"
#include "SmoothChanger.h"
#include "ParamStore.h"
#include "XYEnvolopedOscs.h"
#include "myIIRFilter.h"

// ===========================
// ===========================
// SOUND
class MyFirstSynthSound : public SynthesiserSound
{
public:
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};




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
class MyFirstSynthVoice : public SynthesiserVoice
{
public:
    MyFirstSynthVoice(int numOscs, int numEnvs, int numFilters) {
        smoothEnvParams.clear();
        smoothOscParams.clear();
        for(int i = 0; i < numOscs; ++i)  //Initialising an owned array of parameter smoothers
        {
            smoothOscParams.add(new MultiSmooth(4));
        }
        
        for(int i = 0; i < numEnvs; ++i)  //Initialising an owned array of parameter smoothers
        {
            //ADSRSmooth.add(new SmoothChanges());
            smoothEnvParams.add(new MultiSmooth());
            myEnvs.add(new ADSR());
        }
        
        for(int i = 0; i < numFilters; ++i)
        {
            auto* filter = synthFilters.add(new myIIRFilter());
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
    
    void init(float sampleRate)
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
     
    /**
    Changing ADSR parameters

    @param Attack
    @param Decay
    @param Sustain
    @param Release
    */
    
    /*void changeADSR(float newAttack, float newDecay, float newSustain, float newRelease)
    {
        //inialising ramps if params changed
        ADSRSmooth[0] -> init(ADSRparams[0], newAttack / 1000.0f);
        ADSRSmooth[1] -> init(ADSRparams[1], newDecay / 1000.0f);
        ADSRSmooth[2] -> init(ADSRparams[2], newSustain / 100.0f);
        ADSRSmooth[3] -> init(ADSRparams[3], newRelease / 1000.0f);
        
    }*/
    
    void setParams(OwnedArray<EnvolopeParams>& envs, OwnedArray<OscParams>& oscs, OwnedArray<SimpleParams>& lfos, OwnedArray<SimpleParams>& filters)
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
    }
    
    void updateFilters(int filterNum, int filterMode, float filterFreq)
    {
        if(filterMode != 0)
        {
            filterEnable[filterNum] = true;
            synthFilters[filterNum] -> setFilterOrder(filterMode == 2);
        }
        else
        {
            filterEnable[filterNum] = false;
        }
        
        if(!playing || !filterEnable[filterNum])
        {
            smoothFilterParams[filterNum] -> init(filterFreq, filterFreq, 500);
            synthFilters[filterNum] -> setFilterCutOffFreq(filterFreq);
        }
        else
        {
            smoothFilterParams[filterNum] -> setTargetVal(filterFreq);
        }
    }
    
    void updateLFOs(float lfoAmp, float lfoFreq)
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
    
    void updateEnv(int envNum, ADSR::Parameters thisADSR)
    {
        float adsrParams[4] = {thisADSR.attack, thisADSR.decay, thisADSR.sustain, thisADSR.release};
        //std::cout<< "sent targets" << (float)adsrParams[0] <<" | "<<adsrParams[1] << " | "<< adsrParams[2]<< " | " << adsrParams[3] <<std::endl;
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
        
    void updateOsc(int oscNum, float newTune, float newPan, float newMinAmp, float newMaxAmp)
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
    
   /* void setInitalParams()
    {
        for(int i = 0; i < 2; ++i)
        {
            float initialEnv[4] = {1.0, 1.0, 0.5f, 1.0f};
            float initialOsc[3] = {0.0f, 0.2f, 1.0f};
            smoothEnvParams[i] -> init(initialEnv ,initialEnv);
            smoothOscParams[i] -> init(initialOsc ,initialOsc);
            setADSR(i, initialEnv);
        }
    }*/
     //--------------------------------------------------------------------------
     /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        float freq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        noteVelocity = velocity;
        for(int i = 0; i < myEnvs.size(); ++i)
        {
            myEnvs[i] -> reset();
            myEnvs[i] -> noteOn();
        }
        sourceOscs.playMode(true);
        sourceOscs.setOscsMidiInput(midiNoteNumber);
        //oscs.setMidi
        released = false;
        playing = true;
        //ADSR::Parameters theParamsandSHit =  myEnvs[0] -> getParameters();
        //std::cout<< theParamsandSHit.attack <<std::endl;
        
    }
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        myEnvs[0] -> noteOff();
        myEnvs[1] -> noteOff();
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
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        
        //ADSR::Parameters checkParams = myEnvs[0] -> getParameters();
        //std::cout << checkParams.attack << std::endl;
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
                
                float osc1[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                float osc2[4]  = {0.0f, 0.0f, 0.0f, 0.0f};
                float osc3[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                float osc4[4]  = {0.0f, 0.0f, 0.0f, 0.0f};
                
                
                smoothOscParams[0] -> getNextVal(osc1);
                smoothOscParams[1] -> getNextVal(osc2);
                smoothOscParams[2] -> getNextVal(osc3);
                smoothOscParams[3] -> getNextVal(osc4);
                
                sourceOscs.setOscMinMaxVolume(0, osc1[2], osc1[3]);
                sourceOscs.setOscMinMaxVolume(1, osc2[2], osc2[3]);
                sourceOscs.setOscMinMaxVolume(2, osc3[2], osc3[3]);
                sourceOscs.setOscMinMaxVolume(3, osc4[2], osc4[3]);
                
                sourceOscs.setPanAmount(0, osc1[1]);
                sourceOscs.setPanAmount(1, osc2[1]);
                sourceOscs.setPanAmount(2, osc3[1]);
                sourceOscs.setPanAmount(3, osc4[1]);
                
                sourceOscs.setTuneAmount(0, osc1[0]);
                sourceOscs.setTuneAmount(1, osc2[0]);
                sourceOscs.setTuneAmount(2, osc3[0]);
                sourceOscs.setTuneAmount(2, osc4[0]);
                
                ampEnv = myEnvs[0] -> getNextSample();
                float envVals[2] = {myEnvs[1] -> getNextSample(), myEnvs[2] -> getNextSample()};
                //float envVal4 = myEnvs[3] -> getNextSample();

                
                sourceOscs.getNextVal(envVals, currentSample);
                
                float lfoParams[2] = {0,0};
                
                smoothLFOParams[0] -> getNextVal(lfoParams);
                
                lfoOsc.setFrequency(lfoParams[1]);
                
                if(lfoParams[0] > 0.0001f)
                {
                    float lfoVal = lfoOsc.getNextSample() * lfoParams[0];
                    float lfoDepthInv = 1.0f - lfoParams[0];
                    //lfoDepth * lfoOut * currentSample[0] + (1 - lfoDepth) * currentSample
                    
                    currentSample[0] = (lfoVal + lfoDepthInv) * currentSample[0];
                    currentSample[1] = (lfoVal + lfoDepthInv) * currentSample[1];
                }
                
                /*for(int i = 0; i < 2; ++i)
                {
                    if(filterEnable[i])
                    {
                        //synthFilters[i] -> processNextSample(<#float nextSample#>)
                    }
                }*/
                
                if(released && ampEnv<0.0001f)
                {
                    clearCurrentNote();
                    updateParams();
                    playing = false;
                    released = false;
                    //resetAllEnvs();
                    sourceOscs.playMode(false);
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
    //--------------------------------------------------------------------------
    void pitchWheelMoved(int) override {}
    //--------------------------------------------------------------------------
    void controllerMoved(int, int) override {}
    //--------------------------------------------------------------------------
    /**
     Can this voice play a sound. I wouldn't worry about this for the time being

     @param sound a juce::SynthesiserSound* base class pointer
     @return sound cast as a pointer to an instance of MyFirstSynthSound
     */
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<MyFirstSynthSound*> (sound) != nullptr;
    }
    //--------------------------------------------------------------------------
private:
    //--------------------------------------------------------------------------
    /*
    Setting ADSR parameters as attribute values

    */
    
    void setADSR(int envNum, float adsrVals[4])
    {
        //std::cout<< adsrVals[0] <<std::endl;
        //Setting ADSR parameters
        ADSR::Parameters myADSR;
        
        myADSR.attack = adsrVals[0];
        myADSR.decay = adsrVals[1];
        myADSR.sustain = adsrVals[2];
        myADSR.release = adsrVals[3];
        
        setADSR(envNum, myADSR);  //Setting envolope with passed parameters
        
    }
    
    void setADSR(int envNum, ADSR::Parameters adsrParams)
    {
        myEnvs[envNum] -> setParameters(adsrParams);   //Setting envolope with passed parameters
    }
    
    void updateParams()
    {
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
    
    
    // Set up any necessary variables here
    /// Should the voice be playing?
    bool playing = false;

    //Check Note released
    bool released = false;
    
    float noteVelocity=0;
    
    //LFO Oscillator
    Oscillator lfoOsc;
    
    //ADSR
    OwnedArray<ADSR> myEnvs;
    
    //Filters
    OwnedArray<myIIRFilter> synthFilters;
    
    int envUpdate[5] = {4, 4, 4, 4, 4};
    int oscUpdate[4] = {4, 4, 4, 4};
    int lfoUpdate = 4;
    int filterUpdate[2] = {4, 4};
    
    bool filterEnable[2] = {false, false};
    //Owned array for smoothing all parameters
    OwnedArray<MultiSmooth> smoothEnvParams;
    OwnedArray<MultiSmooth> smoothOscParams;
    OwnedArray<MultiSmooth> smoothLFOParams;
    OwnedArray<SmoothChanges> smoothFilterParams;
    
    int oscTypes[4] = {1, 2, 3, 4};
    
    XYEnvolopedOscs sourceOscs;
};
