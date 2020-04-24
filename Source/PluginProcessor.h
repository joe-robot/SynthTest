/*
  ==============================================================================

    JUCE plugin processor.

    Created: 12 Apr 2020
    Author:  B159113
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SmoothChanger.h"
#include "PostBoxSynth.h"
#include "ParamNames.h"
#include "ParamStore.h"

//==============================================================================
/**
*/

// =================================
// =================================
// PostBoxSynthesiserProcessor

/*!
 @class PostBoxSynthesiserProcessor
 @abstract The main processor of the plugin that holds the main DSP process loop
 
 @namespace none
 @updated 2020-04-24
 */
class PostBoxSynthesiserProcessor  : public AudioProcessor,
                                   public ValueTree::Listener
{
public:
    //==============================================================================
    PostBoxSynthesiserProcessor();
    ~PostBoxSynthesiserProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    /**
     * Updates the parameter arrays with update parameter values
     *
    */
    void setParamTargets();
    
    /**
     * Called when any parmeter in the value tree is updated, to ensire that parameters are passed to the synth when updated
     * and avoid passing them when they have not changed. Is an override required when inheriting from the value tree lisetener class
     *
     * @param treeWhosePropertyhasChanged tells which value tree has changed
     * @param propery tells which value tree property has cahnged
     *
    */
    void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    //Parameters
    AudioProcessorValueTreeState parameters;
    ParamNames paramID;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PostBoxSynthesiserProcessor)
    
    int numVoices = 6; //Setting number of synth voices
    
    std::atomic<bool> paramsUpdated {false};
    
    //Synthesiser
    Synthesiser mySynth;
    
    //Defining the number of each type used in the synth
    int numOscs = 4;
    int numEnvs = 8;
    int numLFOs = 1;
    int numFilters = 2;
    
    //Atomic float to point to gain parameter
    std::atomic<float>* gainParam;
    float prevGain = 1; //Parameter for storing previous gain
    
    //Defining owned arrays for storing the parameters
    OwnedArray<EnvolopeParams> envolopeParams;
    OwnedArray<SimpleParams> oscillatorParams;
    OwnedArray<SimpleParams> lfoParams;
    OwnedArray<SimpleParams> filterParams;
    OwnedArray<SimpleParams> paramEnvChoice;
    
};
