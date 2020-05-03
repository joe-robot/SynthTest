/*
  ==============================================================================

    PostBoxSynth.h
    Definition of a synth that uses envolopes to modify parameters upon playing
    Created: 15 Apr 2020
    Author:  B159113

  ==============================================================================
*/


#pragma once

#include "Oscillator.h"
#include "SmoothChanger.h"
#include "ParamStore.h"
#include "XYEnvolopedOscs.h"
#include "MyIIRFilter.h"

// ===========================
// ===========================
// Synthesiser sound
class PostBoxSynthSound : public SynthesiserSound
{
public:
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};




// =================================
// =================================
// Synthesiser Voice

/*!
 @class PostBoxSynth
 @abstract the processing for each synth voice.
 @discussion multiple PostBoxSynth objects will be created by the Synthesiser so that it can be played polyphicially
 
 @namespace none
 @updated 2020-04-24
 */
class PostBoxSynth : public SynthesiserVoice
{
public:
    //==============================================================================
    /** Constructor*/
    PostBoxSynth(int numOscs, int numEnvs, int numFilters);
    /** Destructors*/
    ~PostBoxSynth(){};
    //==============================================================================
    
    /**
     * Sets the sample Rate of the oscillator
     *
     * @param sampleRate is the sampleRate in samples / s
     *
     */
    void setSampleRate(float sampleRate);

    /**
     * Sets the parameters of the synth and updates them if they have changed
     *
     * @param envs is an array of envolope parameters
     * @param oscs is an array of oscillator parameters
     * @param lfos is an array of lfoparameters
     * @param filters is an array of filter parameters
     * @param filters is an array of filter parameters
     * @param costmEnvsChoice  is an array of parameter envolope parameters
    */
    void setParams(OwnedArray<EnvolopeParams>& envs, OwnedArray<SimpleParams>& oscs, OwnedArray<SimpleParams>& lfos, OwnedArray<SimpleParams>& filters, OwnedArray<SimpleParams>& paramEnvsChoice);
    
     /**
      * What should be done when a note starts

      * @param midiNoteNumber
      * @param velocity
      * @param SynthesiserSound unused variable
      * @param / unused variable
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
     *The main processing block that renders the output of the synth oscillators with applie FX
     *
     * @param outputBuffer pointer to output
     * @param startSample position of first sample in buffer
     * @param numSamples number of smaples in output buffer
     */
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    //--------------------------------------------------------------------------
    /**
     * Listener for the pitch wheel moving
     *
     * @param newPitchWheelValue
     *
    */
    void pitchWheelMoved(int) override {}
    //--------------------------------------------------------------------------
    /**
     * Listener for the midi controller moving
     *
     * @param controllerNumber
     * @param newControllerValue
     *
    */
    void controllerMoved(int, int) override {}
    //--------------------------------------------------------------------------
    
    /**
     * Can this voice play a sound. I wouldn't worry about this for the time being
     *
     * @param sound a juce::SynthesiserSound* base class pointer
     * @return sound cast as a pointer to an instance of MyFirstSynthSound
     */
    bool canPlaySound (SynthesiserSound* sound) override;
    
    
private:
    
    /**
     * Updates the filter parameters
     *
     * @param filterNum is which filter to update
     * @param filterMode is the mode of the filter
     * @param filterFreq is cutoff frequency of the filter in Hz
     *
    */
    void updateFilters(int filterNum, int filterMode, float filterFreq);
    
    /**
     * Updates the LFO parameters
     *
     * @param lfpAmp is the updated lfo amplitude parameter
     * @param lfoFreq is lfo frequency in Hz
     *
    */
    void updateLFOs(float lfoAmp, float lfoFreq);
    
    /**
     * Updates the envolope parameters
     *
     * @param envNum is which envolope to update
     * @param thisADSR is the updated ADSR of the envolope
     *
    */
    void updateEnv(int envNum, ADSR::Parameters thisADSR);
        
    /**
     * Updates the oscillator parameters
     *
     * @param oscNum is which oscillator to update
     * @param newTune new tune of oscillator in semiTones
     * @param newPan new pan of oscillator
     * @param newMinAmp new min amp of oscillator
     * @param newMaxAmp new max amp of oscillator
     *
    */
    void updateOsc(int oscNum, float newTune, float newPan, float newMinAmp, float newMaxAmp);
    
    /**
     * Method to update the parameter envolopes parameters
     *
     * @param paramEnvNum envolpe to update envolopes for
     * @param envChoice the drop down menu item
     * @param paramResult the result for the max parameter value
     *
    */
    void updateParamEnvs(int paramEnvNum, int envChoice, float paramResult);
    
    /**
     * Method to update the max parameter value
     *
     * @param paramEnvNum envolpe to update envolopes for
     * @param paramResult the result for the max parameter value
     *
    */
    void updateMaxParamVals(int paramEnvNum, float paramResult);
    
    /**
     * Gets next samples from the oscilllators
     *
     * @param sample returns an array of ocillator next samples
     *
    */
    void oscsNextSample(float* sample);
    
    /**
     * Applies FX to stereo samples
     *
     * @param sample collects stereo samples and returns the array of samples with FX applied
     *
    */
    void applyFX(float* sample);
    
    /**
     * Gets next samples from the lfos
     *
     * @param sample returns an array of samples with LFOs applied
     *
    */
    void applyLFO(float* sample);
    
    /**
     * Gets next samples from the filters
     *
     * @param sample returns an array of samples with filters applied
     *
    */
    void applyFilter(float* sample);
    
    /**
     * Resets the voice to be called once note has finsihed playing
     *
    */
    void resetVoice();
    
    /**
     * Sets the ADSR values for an envolope
     *
     * @param envNum envolpe to change parameters for
     * @param adsrVals array of ADSR values
     *
    */
    void setADSR(int envNum, float adsrVals[4]);
    
    /**
     * Sets the ADSR values for an envolope
     *
     * @param envNum envolpe to change parameters for
     * @param adsrParams ADSR values
     *
    */
    void setADSR(int envNum, ADSR::Parameters adsrParams);
    
    /**
     * Updates all the Parameter envolopes that are active
     *
    */
    void getNextParamEnvVals();
    
    /**
     * get parameter value with relevent parameter envolope applied
     *
     * @param paramNum is the parameter number
     * @param paramVal is the value of the parameter with no envolope applied
     *
    */
    float getParamVal(int paramNum, float paramVal);

    /**
     * Updates the parameters
     *
    */
    void updateParams();
    
    /**
     * Updates the envoloope parameters
     *
    */
    void updateEnvParams();
    
    
    /**
     * Updates the oscillator parameters
     *
    */
    void updateOscParams();
    
    /**
     * Updates the LFO parameters
     *
    */
    void updateLFOParams();
    
    
    //Variable to check if voice should be playing
    bool playing = false;

    //Check if the note has been released
    bool released = false;
    
    //Variable for storing the note velocity
    float noteVelocity=0;
    
    //LFO Oscillator
    Oscillator lfoOsc;
    
    //Env ADSRs
    OwnedArray<ADSR> myEnvs;
    
    //Filters
    OwnedArray<StereoIIRFilters> synthFilters;
    
    //Value switches to check if parameters have changed since last checked
    int envUpdate[8] = {4, 4, 4, 4, 4, 4, 4, 4};
    int oscUpdate[4] = {4, 4, 4, 4};
    int lfoUpdate = 4;
    int filterUpdate[2] = {4, 4};
    int paramEnvUpdate[5] = {4, 4, 4, 4, 4};
    
    //Array to check if filter enabled
    bool filterEnable[2] = {false, false};
    
    //Owned array for smoothing all parameters
    OwnedArray<MultiSmooth> smoothEnvParams;
    OwnedArray<MultiSmooth> smoothOscParams;
    OwnedArray<MultiSmooth> smoothLFOParams;
    OwnedArray<SmoothChanges> smoothFilterParams;
    
    //Intial oscillator types
    int oscTypes[4] = {1, 2, 3, 4};
    
    //Variable for lfo a,plitude
    float lfoAmp = 0;
    
    //Source oscillators that are modified by X, Y envolopes
    XYEnvolopedOscs sourceOscs;
    
    //Parameters to deal with envoloping parameters (12 possible parameters to be envoloped)
    int paramEnvParamsChosen[5] = {0, 0, 0, 0, 0};
    OwnedArray<SmoothChanges> maxParamsVals;
    int numTimesChosen[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float envolopedParamVals[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bool envolopedParam[14] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    
};
