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
        for(int i = 0; i < 4; ++i)  //Initialising an owned array of parameter smoothers
        {
            ADSRSmooth.add(new SmoothChanges());
            smoothEnvParams.add(new MultiSmooth());
            smoothOscParams.add(new MultiSmooth());
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
            ADSRSmooth[i] -> setSampleRate(sampleRate);
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
    
    void setParams(float envParams[2][4], float newOscParams[2][3])
    {
        //NEED A BETTER WAY THAN MULTI DIMENSIONAL ARRAY I THINK
        
        float newenvolopeParams[2][4] = {{1000.0f/1000.0f, 1000.0f/1000.0f, 0.7f, 1000.0f/1000.0f},
        {500.0f/1000.0f, 1500.0f/1000.0f, 0.8f, 100.0f/1000.0f}};
        
        if(!playing)
        {
            for(int i = 0; i < 2; ++i)
            {
                for(int j = 0; j < 4; ++j)
                {
                    if(j<3)
                    {
                        oscParams[i][j] = newOscParams[i][j];
                    }
                    ADSRparams[i][j] = newenvolopeParams[i][j];
                }
            }
        }
        else
        {
            for(int i = 0; i < 2; ++i)
            {
                smoothEnvParams[i] -> setTargetVal(newenvolopeParams[i]);
                smoothOscParams[i] -> setTargetVal(newOscParams[i]);
            }
            
        }
        
    }
    
    void setInitalParams()
    {
        for(int i = 0; i < 2; ++i)
        {
            smoothEnvParams[i] -> init(ADSRparams[i] ,ADSRparams[i]);
            smoothOscParams[i] -> init(oscParams[i] ,oscParams[i]);
            setADSR(i);
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
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        float freq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        myOsc1.setFrequency(freq);
        myOsc2.setFrequency(freq);
        setInitalParams();
        noteVelocity = velocity;
        playing = true;
        myEnvs[0] -> reset();
        myEnvs[0] -> noteOn();
        myEnvs[1] -> reset();
        myEnvs[1] -> noteOn();
        
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
                
                float envVal1 = myEnvs[0] -> getNextSample();
                float envVal2 = myEnvs[1] -> getNextSample();
                
                //currentSample = envVal1 * ((envVal2 * (oscParams[0][2] - oscParams[0][1]) + oscParams[0][1]) * myOsc1.getNextSample() + ((1 - envVal2) * (oscParams[1][2] -       oscParams[1][1]) + oscParams[1][1]) * myOsc2.getNextSample());
                
                currentSample = envVal1 * ((envVal2 * (1.0f - 0.2f) + 0.2f) * myOsc2.getNextSample() + ((1.0f - envVal2) * (0.9f - 0.1f) + 0.1f) * myOsc1.getNextSample());
                
                //currentSample = envVal1 * myOsc1.getNextSample();
                
                if(released && envVal1<0.0001f)
                {
                    clearCurrentNote();
                    playing = false;
                    released = false;
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
    
    void setADSR(int envNum)
    {
        //Setting ADSR parameters
        ADSR::Parameters myADSR;
        
        myADSR.attack = ADSRparams[envNum][0];
        myADSR.decay = ADSRparams[envNum][1];
        myADSR.sustain = ADSRparams[envNum][2];
        myADSR.release = ADSRparams[envNum][3];
        
        myEnvs[envNum] -> setParameters(myADSR);   //Setting envolope with passed parameters
        
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
            //smoothEnvParams[i] -> getNextVal(ADSRparams[i]);
            smoothOscParams[i] -> getNextVal(oscParams[i]);
            
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
    
    //ADSR
    OwnedArray<ADSR> myEnvs;
    bool envUpdate[5] = {false, false, false, false, false};
    float ADSRparams[2][4] = {{1.0f, 1.0f, 0.5f, 1.0f}, {1.0f, 1.0f, 0.5f, 1.0f}};
    float oscParams[2][3] ={{0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.5f}};
    
    //Parameter Smoother
    OwnedArray<SmoothChanges> ADSRSmooth;
    
    //Owned array for smoothing all parameters
    OwnedArray<MultiSmooth> smoothEnvParams;
    OwnedArray<MultiSmooth> smoothOscParams;
    
    
};
