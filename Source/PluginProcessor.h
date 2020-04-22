/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SmoothChanger.h"
#include "MyFirstSynth.h"
#include "ParamNames.h"
#include "ParamStore.h"

//==============================================================================
/**
*/
class SynthTesterAudioProcessor  : public AudioProcessor,
                                   public ValueTree::Listener
{
public:
    //==============================================================================
    SynthTesterAudioProcessor();
    ~SynthTesterAudioProcessor();

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
    
    void setParamTargets();
    
    void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthTesterAudioProcessor)
    
    int numVoices = 16; //Setting number of synth voices
    
    //Synthesiser
    Synthesiser mySynth;
    
    //Parametes
    AudioProcessorValueTreeState parameters;
    
    //Params for oscillators
    /*std::atomic<float>* Osc1Tune;
    std::atomic<float>* Osc1MinAmp;
    std::atomic<float>* Osc1MaxAmp;
    
    std::atomic<float>* Osc2Tune;
    std::atomic<float>* Osc2MinAmp;
    std::atomic<float>* Osc2MaxAmp;
    
    //Defining params for oscillator envolopes
    std::atomic<float>* OscXAttackParam;
    std::atomic<float>* OscXDecayParam;
    std::atomic<float>* OscXSustainParam;
    std::atomic<float>* OscXReleaseParam;
    
    //Defining parameter to store the param value
    std::atomic<float>* attackParam;
    std::atomic<float>* decayParam;
    std::atomic<float>* sustainParam;
    std::atomic<float>* releaseParam;*/
    
    int numOscs = 4;
    int numEnvs = 3;
    int numLFOs = 1;
    int numFilters = 2;
    
    std::atomic<bool> paramsUpdated {false};
    
    OwnedArray<EnvolopeParams> envolopeParams;
    OwnedArray<OscParams> oscillatorParams;
    OwnedArray<SimpleParams> lfoParams;
    OwnedArray<SimpleParams> filterParams;
    
    
    ParamNames paramID;
    
};
