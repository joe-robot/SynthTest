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
    //Parameters
    AudioProcessorValueTreeState parameters;
    ParamNames paramID;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthTesterAudioProcessor)
    
    int numVoices = 6; //Setting number of synth voices
    
    std::atomic<bool> paramsUpdated {false};
    
    //Synthesiser
    Synthesiser mySynth;
    
    int numOscs = 4;
    int numEnvs = 8;
    int numLFOs = 1;
    int numFilters = 2;
    
    
    OwnedArray<EnvolopeParams> envolopeParams;
    OwnedArray<OscParams> oscillatorParams;
    OwnedArray<SimpleParams> lfoParams;
    OwnedArray<SimpleParams> filterParams;
    OwnedArray<SimpleParams> customEnvChoice;
    
};
