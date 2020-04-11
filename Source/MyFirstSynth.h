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
    MyFirstSynthVoice() {
        smoothEnvParams.clear();
        smoothOscParams.clear();
        for(int i = 0; i < 4; ++i)  //Initialising an owned array of parameter smoothers
        {
            //ADSRSmooth.add(new SmoothChanges());
            smoothEnvParams.add(new MultiSmooth());
            smoothOscParams.add(new MultiSmooth(3));
            myEnvs.add(new ADSR());
        }
        
        //setADSR(); //Setting ADSR with attribute values
    }
    //--------------------------------------------------------------------------
    
    /**
    Initialising the voice of the synth with required paramters

    @param sampleRate
    */
    
    void init(float sampleRate)
    {
        myOsc1.setSampleRate(sampleRate);
        myOsc2.setSampleRate(sampleRate);
        myOsc2.setType(3);
        //myEnv.setSampleRate(sampleRate);
        
        for(int i = 0; i < 4; ++i)
        {
           // ADSRSmooth[i] -> setSampleRate(sampleRate);
            smoothEnvParams[i] -> setSampleRate(sampleRate);
            smoothOscParams[i] -> setSampleRate(sampleRate);
            myEnvs[i] -> setSampleRate(sampleRate);
        }
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
    
    void setParams(OwnedArray<EnvolopeParams>& envs, OwnedArray<OscParams>& oscs)
    {
        //NEED A BETTER WAY THAN MULTI DIMENSIONAL ARRAY I THINK
        for(int i = 0; i < envs.size(); ++i)
        {
            //if(envs[i] -> getValSwitch() != envUpdate[i])   //Check if env updated since last checked
            //{
                updateEnv(i, envs[i] -> getADSRParams());
                envUpdate[i] = !envUpdate[i];
            //}
        }
        
        for(int i = 0; i < oscs.size(); ++i)
        {
            //if(oscs[i] -> getValSwitch() != oscUpdate[i])   //Check if env updated since last checked
            //{
                updateOsc(i, oscs[i] -> getOscParams(0), oscs[i] -> getOscParams(1), oscs[i] -> getOscParams(2));
                oscUpdate[i] = !oscUpdate[i];
            //}
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
            smoothEnvParams[envNum] -> setTargetVal(adsrParams);
        }
    }
        
    void updateOsc(int oscNum, float newTune, float newMinAmp, float newMaxAmp)
    {
        float oscParams[3] = {newTune, newMinAmp, newMaxAmp};
        
        //Update osc differently if playing or not playing
        if(!playing)
        {
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
        myOsc1.setFrequency(freq);
        myOsc2.setFrequency(freq);
        //setInitalParams();
        noteVelocity = velocity;
        playing = true;
        myEnvs[0] -> reset();
        myEnvs[0] -> noteOn();
        myEnvs[1] -> reset();
        myEnvs[1] -> noteOn();
        ADSR::Parameters theParamsandSHit =  myEnvs[0] -> getParameters();
        std::cout<< theParamsandSHit.attack <<std::endl;
        
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
        
        ADSR::Parameters checkParams = myEnvs[0] -> getParameters();
        //std::cout << checkParams.attack << std::endl;
        // iterate through the necessary number of samples (from startSample up to startSample + numSamples)
        for (int sampleIndex = startSample;   sampleIndex < (startSample+numSamples);   sampleIndex++)
        {
            float currentSample = 0;
            // your sample-by-sample DSP code here!
            // An example white noise generater as a placeholder - replace with your own code
            if(playing)
            {
                //Updating synth parameters
                updateParams();
                
                float osc1[3] = {0.0f, 0.0f, 0.0f};;
                float osc2[3]  = {0.0f, 0.0f, 0.0f};;
                
                smoothOscParams[0] -> getNextVal(osc1);
                smoothOscParams[1] -> getNextVal(osc2);
                
                float envVal1 = myEnvs[0] -> getNextSample();
                float envVal2 = myEnvs[1] -> getNextSample();
                
                //currentSample = envVal1 * ((envVal2 * (oscParams[0][2] - oscParams[0][1]) + oscParams[0][1]) * myOsc1.getNextSample() + ((1 - envVal2) * (oscParams[1][2] -       oscParams[1][1]) + oscParams[1][1]) * myOsc2.getNextSample());
                
                //currentSample = envVal1 * ((envVal2 * (1.0f - 0.2f) + 0.2f) * myOsc2.getNextSample() + ((1.0f - envVal2) * (0.9f - 0.1f) + 0.1f) * myOsc1.getNextSample());
                
                currentSample = envVal1 * ((envVal2 * (osc1[2] - osc1[1]) + osc1[1]) * myOsc2.getNextSample() + ((1.0f - envVal2) * (osc2[2] - osc2[1]) + osc2[1]) * myOsc1.getNextSample());
                
                //currentSample = envVal1 * myOsc1.getNextSample();
                
                if(released && envVal1<0.0001f)
                {
                    clearCurrentNote();
                    playing = false;
                    released = false;
                    //resetAllEnvs();
                }
            }
            // for each channel, write the currentSample float to the output
            for (int chan = 0; chan<outputBuffer.getNumChannels(); chan++)
            {
                // The output sample is scaled by 0.2 and note velocity so that it is not too loud by default
                outputBuffer.addSample (chan, sampleIndex, currentSample * noteVelocity * 0.2);
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
    
    /*
    Checking if parameters have changed and if so updating them

    */
    /*void updateParams()
    {
        bool checkUpdate = false; //Initially set ADSR updated as false
        
        for(int i = 0; i < 4; ++i)
        {
            if(ADSRSmooth[i] -> checkChanging()) //If ADSR value is changing
            {
                ADSRparams[i] = ADSRSmooth[i] -> getNextVal(); //Update parameter value
                checkUpdate = true;     //Mark ADSR as changed
            }
        }
        
        if(checkUpdate) //Update ADSR if values have changed
        {
            setADSR();
        }
    }*/
    
    void updateParams()
    {
        for(int i = 0; i < 2; ++i)
        {
            if(smoothEnvParams[i] -> checkChanging())
            {
                float adsrVals[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                smoothEnvParams[i] -> getNextVal(adsrVals);
                setADSR(i, adsrVals);
            }
            //smoothEnvParams[i] -> getNextVal(ADSRparams[i]);
            //smoothOscParams[i] -> getNextVal(oscParams[i]);
            
            //setADSR(i);
        }
    }
    
    
    // Set up any necessary variables here
    /// Should the voice be playing?
    bool playing = false;

    //Check Note released
    bool released = false;
    
    /// a random object for use in our test noise function
    Random random;
    
    float noteVelocity=0;
    
    //Sine Oscillator
    Oscillator myOsc1;
    Oscillator myOsc2;
    
    //OwnedArray<Oscillator> myOscs;
    
    //ADSR
    OwnedArray<ADSR> myEnvs;
    bool envUpdate[5] = {true, true, true, true, true};
    bool oscUpdate[4] = {true, true, true, true};
    //float ADSRparams[2][4] = {{1.0f, 1.0f, 0.5f, 1.0f}, {1.0f, 1.0f, 0.5f, 1.0f}};
    //float oscParams[2][3] ={{0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.5f}};
    
    //Parameter Smoother
    //OwnedArray<SmoothChanges> ADSRSmooth;
    
    
    //Owned array for smoothing all parameters
    OwnedArray<MultiSmooth> smoothEnvParams;
    OwnedArray<MultiSmooth> smoothOscParams;
    
    
};
