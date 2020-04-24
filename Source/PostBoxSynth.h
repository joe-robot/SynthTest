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
class PostBoxSynthSound : public SynthesiserSound
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
class PostBoxSynth : public SynthesiserVoice
{
public:
    PostBoxSynth(int numOscs, int numEnvs, int numFilters);
    ~PostBoxSynth(){};

    
    void init(float sampleRate);

    
    void setParams(OwnedArray<EnvolopeParams>& envs, OwnedArray<OscParams>& oscs, OwnedArray<SimpleParams>& lfos, OwnedArray<SimpleParams>& filters, OwnedArray<SimpleParams>& customEnvsChoice);
    
    void updateFilters(int filterNum, int filterMode, float filterFreq);
    
    void updateLFOs(float lfoAmp, float lfoFreq);
    
    void updateEnv(int envNum, ADSR::Parameters thisADSR);
        
    void updateOsc(int oscNum, float newTune, float newPan, float newMinAmp, float newMaxAmp);
    


     /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override;
    
    //--------------------------------------------------------------------------
    /**
     The Main DSP Block: Put your DSP code in here
     
     If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

     @param outputBuffer pointer to output
     @param startSample position of first sample in buffer
     @param numSamples number of smaples in output buffer
     */
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
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
    bool canPlaySound (SynthesiserSound* sound) override;
    //--------------------------------------------------------------------------
private:
    //--------------------------------------------------------------------------
    
    void oscsNextSample(float* sample);
    
    void applyFX(float* sample);
    
    
    //Have a parameter for each parameter that is added on the end of the changes in update parameter method, which is changed based on the update other envolopes method
    
    void applyLFO(float* sample);
    
    void applyFilter(float* sample);
    
    void resetVoice();
    
    /*
    Setting ADSR parameters as attribute values

    */
    
    void setADSR(int envNum, float adsrVals[4]);
    
    void setADSR(int envNum, ADSR::Parameters adsrParams);
    
    
    void updateCustomEnvs(int customEnvNum, int envChoice, float paramResult);
    
    void getNextCustomEnvVals();
    
    float getParamVal(int paramNum, float paramVal);

    
    void updateParams();
    
    void updateOscParams();
    
    void updateLFOParams();
    
    
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
    OwnedArray<StereoIIRFilters> synthFilters;
    
    int envUpdate[8] = {4, 4, 4, 4, 4, 4, 4, 4};
    int oscUpdate[4] = {4, 4, 4, 4};
    int lfoUpdate = 4;
    int filterUpdate[2] = {4, 4};
    int customEnvUpdate[5] = {4, 4, 4, 4, 4};
    
    bool filterEnable[2] = {false, false};
    
    //Owned array for smoothing all parameters
    OwnedArray<MultiSmooth> smoothEnvParams;
    OwnedArray<MultiSmooth> smoothOscParams;
    OwnedArray<MultiSmooth> smoothLFOParams;
    OwnedArray<SmoothChanges> smoothFilterParams;
    
    int oscTypes[4] = {1, 2, 3, 4};
    
    float lfoAmp;
    
    XYEnvolopedOscs sourceOscs;
    
    
    int customEnvParamsChosen[5] = {0, 0, 0, 0, 0};
    float maxParamVals[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int numTimesChosen[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float envolopedParamVals[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bool envolopedParam[14] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    
};
