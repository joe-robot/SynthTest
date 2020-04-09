/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthTesterAudioProcessor::SynthTesterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
parameters(*this, nullptr, "Parameters", {
    //Oscillator Params
    std::make_unique<AudioParameterFloat>("osc1Tune", "Osc 1 Tune (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc1MinAmp", "Osc 1 Min Amplitude", 0.0f, 100.0f, 0.0f),
    std::make_unique<AudioParameterFloat>("osc1MaxAmp", "Osc 1 Max Amplitude", 0.0f, 100.0f, 100.0f),
    std::make_unique<AudioParameterFloat>("osc2Tune", "Osc 2 Tune (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc2MinAmp", "Osc 2 Min Amplitude", 0.0f, 100.0f, 0.0f),
    std::make_unique<AudioParameterFloat>("osc2MaxAmp", "Osc 2 Max Amplitude", 0.0f, 100.0f, 100.0f),
    
    //Envolope parameters for x oscillator
    std::make_unique<AudioParameterFloat>("oscXAttack", "Osc X Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("oscXDecay", "Osc X Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("oscXSustain", "Osc X Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("oscXRelease", "Osc X Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Envolope params for the whole note
    std::make_unique<AudioParameterFloat>("attack", "Master Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("decay", "Master Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("sustain", "Master Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("release", "Master Release (ms)", 0.001f, 2000.0f, 1000.0f)
    
})
{
    //Setting up the synth
    mySynth.addSound(new MyFirstSynthSound());
    for(int i = 0; i < numVoices; ++i)
    {
        mySynth.addVoice(new MyFirstSynthVoice());
    }
    
    //Ensuring Smooth params owned arrays are empty
    smoothParams.clear();
    smoothEnvParams.clear();
    smoothOscParams.clear();
    
    //Main Envolope Params
    attackParam = parameters.getRawParameterValue("attack");
    decayParam = parameters.getRawParameterValue("decay");
    sustainParam = parameters.getRawParameterValue("sustain");
    releaseParam = parameters.getRawParameterValue("release");
    smoothEnvParams.add(new MultiSmooth());
    
    //Setting Up oscillator parameters  NEEEEEEEEED TO BE LOWCASE PARAMS!!!!!!!!!!!
    Osc1Tune = parameters.getRawParameterValue("osc1Tune");
    Osc1MinAmp = parameters.getRawParameterValue("osc1MinAmp");
    Osc1MaxAmp = parameters.getRawParameterValue("osc1MaxAmp");
    smoothOscParams.add(new MultiSmooth(3));
    
    Osc2Tune = parameters.getRawParameterValue("osc2Tune");
    Osc2MinAmp = parameters.getRawParameterValue("osc2MinAmp");
    Osc2MaxAmp = parameters.getRawParameterValue("osc2MaxAmp");
    smoothOscParams.add(new MultiSmooth(3));

    //Osc X Params
    OscXAttackParam = parameters.getRawParameterValue("oscXAttack");
    OscXDecayParam = parameters.getRawParameterValue("oscXDecay");
    OscXSustainParam = parameters.getRawParameterValue("oscXSustain");
    OscXReleaseParam = parameters.getRawParameterValue("oscXRelease");
    smoothEnvParams.add(new MultiSmooth());
    
}

SynthTesterAudioProcessor::~SynthTesterAudioProcessor()
{
}

//==============================================================================
const String SynthTesterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthTesterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthTesterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthTesterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthTesterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthTesterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthTesterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthTesterAudioProcessor::setCurrentProgram (int index)
{
}

const String SynthTesterAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthTesterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SynthTesterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mySynth.setCurrentPlaybackSampleRate(sampleRate);
    
    for(int i=0; i < numVoices; ++i)
    {
        MyFirstSynthVoice* v = dynamic_cast<MyFirstSynthVoice*>(mySynth.getVoice(i));
        v -> init(sampleRate);
        v -> changeADSR(*attackParam, *decayParam, *sustainParam, *releaseParam);
    }
    
    float defaultEnvArray[4] {1000.0f, 1000.0f, 50.0f, 1000.0f};
    
    for(int i = 0; i < smoothEnvParams.size(); ++i)
    {
        smoothEnvParams[i] -> setSampleRate(sampleRate);
        smoothEnvParams[i] -> init(defaultEnvArray, defaultEnvArray);
    }
    
    for(int i = 0; i < smoothOscParams.size(); ++i)
    {
        smoothOscParams[i] -> setSampleRate(sampleRate);
        smoothOscParams[i] -> init(defaultEnvArray, defaultEnvArray);
    }
}

void SynthTesterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthTesterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthTesterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    setParamTargets();
    
    for(int i=0; i < numVoices; ++i)
    {
        
        MyFirstSynthVoice* v = dynamic_cast<MyFirstSynthVoice*>(mySynth.getVoice(i));
        v -> changeADSR(*attackParam, *decayParam, *sustainParam, *releaseParam);
        
        v -> setParams(smoothEnvParams, smoothOscParams);
    }
    
    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SynthTesterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SynthTesterAudioProcessor::createEditor()
{
    //return new SynthTesterAudioProcessorEditor (*this);
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void SynthTesterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthTesterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthTesterAudioProcessor();
}

void SynthTesterAudioProcessor::setParamTargets()
{
    //Long way need to figure out a quicker way!!!!!
    float env1Array[4]{*attackParam, *decayParam, *sustainParam, *releaseParam};
    smoothEnvParams[0] -> setTargetVal(env1Array);
    float env2Array[4]{*attackParam, *decayParam, *sustainParam, *releaseParam};
    smoothEnvParams[1] -> setTargetVal(env2Array);
    
    float osc1Array[3]{*Osc1Tune, *Osc1MinAmp, *Osc1MaxAmp};
    smoothOscParams[0] -> setTargetVal(osc1Array);
    float osc2Array[3]{*Osc2Tune, *Osc2MinAmp, *Osc2MaxAmp};
    smoothOscParams[1] -> setTargetVal(osc2Array);
    
}
