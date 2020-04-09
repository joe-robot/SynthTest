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
        myOsc.setSampleRate(sampleRate);
        //myEnv.setSampleRate(sampleRate);
        
        for(int i = 0; i < 4; ++i)
        {
            ADSRSmooth[i] -> setSampleRate(sampleRate);
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
    
    void setParams(OwnedArray<MultiSmooth>* envSmoothParams, OwnedArray<MultiSmooth>* oscSmoothParams)
    {
        
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
        myOsc.setFrequency(freq);
        noteVelocity = velocity;
        playing = true;
        //myEnv.reset();
        //myEnv.noteOn();
        
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
        //myEnv.noteOff();
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
        if (playing) // check to see if this voice should be playing
        {
            // iterate through the necessary number of samples (from startSample up to startSample + numSamples)
            for (int sampleIndex = startSample;   sampleIndex < (startSample+numSamples);   sampleIndex++)
            {
                // your sample-by-sample DSP code here!
                // An example white noise generater as a placeholder - replace with your own code
                
                //Updating synth parameters
                //updateParams();
                
                //float envVal = myEnv.getNextSample();
                
                float currentSample = myOsc.getNextSample();
                
                //if(released && envVal<0.0001f)
                //{
                //    clearCurrentNote();
                //    playing = false;
                //    released = false;
                //}
                
                // for each channel, write the currentSample float to the output
                for (int chan = 0; chan<outputBuffer.getNumChannels(); chan++)
                {
                    // The output sample is scaled by 0.2 and note velocity so that it is not too loud by default
                    outputBuffer.addSample (chan, sampleIndex, currentSample * noteVelocity * 0.2);
                }
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
    
    /*void setADSR()
    {
        //Setting ADSR parameters
        ADSR::Parameters myADSR;
        
        myADSR.attack = ADSRparams[0];
        myADSR.decay = ADSRparams[1];
        myADSR.sustain = ADSRparams[2];
        myADSR.release = ADSRparams[3];
        
        myEnv.setParameters(myADSR);   //Setting envolope with passed parameters
        
    }*/
    
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
    
    
    // Set up any necessary variables here
    /// Should the voice be playing?
    bool playing = false;

    //Check Note released
    bool released = false;
    
    /// a random object for use in our test noise function
    Random random;
    
    float noteVelocity=0;
    
    //Sine Oscillator
    Oscillator myOsc;
    
    //ADSR
    OwnedArray<ADSR> myEnvs;
    float ADSRparams[4] = {1.0f, 1.0f, 0.5f, 1.0f};
    
    //Parameter Smoother
    OwnedArray<SmoothChanges> ADSRSmooth;
};
