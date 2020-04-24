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
    std::make_unique<AudioParameterChoice>("osc1Source", "Source 1", StringArray({"None","Sine","Square","Triangle","Saw","Noise"}), 1),
    std::make_unique<AudioParameterInt>("osc1Tune", "Osc 1 Tune (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc1Pan", "Osc 1 Pan", -1, 1, 0),
    std::make_unique<AudioParameterFloat>("osc1MinAmp", "Osc 1 Min Amplitude", 0.0f, 100.0f, 0.0f),
    std::make_unique<AudioParameterFloat>("osc1MaxAmp", "Osc 1 Max Amplitude", 0.0f, 100.0f, 100.0f),
    
    std::make_unique<AudioParameterChoice>("osc2Source", "Source 2", StringArray({"None","Sine","Square","Triangle","Saw","Noise"}), 2),
    std::make_unique<AudioParameterInt>("osc2Tune", "Osc 2 Tune (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc2Pan", "Osc 2 Pan", -1, 1, 0),
    std::make_unique<AudioParameterFloat>("osc2MinAmp", "Osc 2 Min Amplitude", 0.0f, 100.0f, 0.0f),
    std::make_unique<AudioParameterFloat>("osc2MaxAmp", "Osc 2 Max Amplitude", 0.0f, 100.0f, 100.0f),
    
    std::make_unique<AudioParameterChoice>("osc3Source", "Source 3", StringArray({"None","Sine","Square","Triangle","Saw","Noise"}), 3),
    std::make_unique<AudioParameterInt>("osc3Tune", "Osc 3 Tune (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc3Pan", "Osc 3 Pan", -1, 1, 0),
    std::make_unique<AudioParameterFloat>("osc3MinAmp", "Osc 3 Min Amplitude", 0.0f, 100.0f, 0.0f),
    std::make_unique<AudioParameterFloat>("osc3MaxAmp", "Osc 3 Max Amplitude", 0.0f, 100.0f, 100.0f),
    
    std::make_unique<AudioParameterChoice>("osc4Source", "Source 4", StringArray({"None","Sine","Square","Triangle","Saw","Noise"}), 4),
    std::make_unique<AudioParameterInt>("osc4Tune", "Osc 4 Tune (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc4Pan", "Osc 4 Pan", -1, 1, 0),
    std::make_unique<AudioParameterFloat>("osc4MinAmp", "Osc 4 Min Amplitude", 0.0f, 100.0f, 0.0f),
    std::make_unique<AudioParameterFloat>("osc4MaxAmp", "Osc 4 Max Amplitude", 0.0f, 100.0f, 100.0f),
    
    //Envolope parameters for x oscillators
    std::make_unique<AudioParameterFloat>("oscXattack", "Osc X Attack (ms)", NormalisableRange<float>(0.001f, 4000.0f), 1000.0f, "ms"),
    std::make_unique<AudioParameterFloat>("oscXdecay", "Osc X Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("oscXsustain", "Osc X Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("oscXrelease", "Osc X Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Envolope parameters for y oscillators
    std::make_unique<AudioParameterFloat>("oscYattack", "Osc Y Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("oscYdecay", "Osc Y Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("oscYsustain", "Osc Y Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("oscYrelease", "Osc Y Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Lfo Paramters
    std::make_unique<AudioParameterFloat>("lfo1Depth", "LFO Depth", 0.0f, 1.0f, 0.0f),
    std::make_unique<AudioParameterFloat>("lfo1Freq", "LFO Frequency (Hz)", 0.001f, 20.0f, 10.0f),
    
    //Envolope params for the whole note
    std::make_unique<AudioParameterFloat>("attack", "Master Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("decay", "Master Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("sustain", "Master Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("release", "Master Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Filter params
    
    //LP Filter
    std::make_unique<AudioParameterChoice>("lpFilterMode", "Low Pass Filter Mode", StringArray({"None","-12dB/oct","-24dB/oct"}), 0),
    std::make_unique<AudioParameterFloat>("lpFilterFreq", "Low Pass Filter Frequency (Hz)", 30.0f, 20000.0f, 1000.0f),
    
    //HP Filter
    std::make_unique<AudioParameterChoice>("hpFilterMode", "High Pass Filter Mode", StringArray({"None","-12dB/oct"}), 0),
    std::make_unique<AudioParameterFloat>("hpFilterFreq", "High Pass Filter Frequency (Hz)", 30.0f, 20000.0f, 1000.0f),
    
    //----Additional Envolope params----//
    
    //Max param Vals
    std::make_unique<AudioParameterInt>("osc1TuneMax", "Osc 1 Tune Max (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc1PanMax", "Osc 1 Pan Max", -1, 1, 0),
    std::make_unique<AudioParameterInt>("osc2TuneMax", "Osc 2 Tune Max (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc2PanMax", "Osc 2 Pan Max", -1, 1, 0),
    std::make_unique<AudioParameterInt>("osc3TuneMax", "Osc 3 Tune Max (semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc3PanMax", "Osc 3 Pan Max", -1, 1, 0),
    std::make_unique<AudioParameterInt>("osc4TuneMax", "Osc 4 Tune Max(semiTones)", -24, 24, 0),
    std::make_unique<AudioParameterFloat>("osc4PanMax", "Osc 4 Pan Max", -1, 1, 0),
    std::make_unique<AudioParameterFloat>("lfo1DepthMax", "LFO Depth Max", 0.0f, 1.0f, 0.0f),
    std::make_unique<AudioParameterFloat>("lfo1FreqMax", "LFO Frequency Max(Hz)", 0.001f, 20.0f, 10.0f),
    std::make_unique<AudioParameterFloat>("lpFilterFreqMax", "Low Pass Filter Frequency Max(Hz)", 30.0f, 20000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("hpFilterFreqMax", "High Pass Filter Frequency Max (Hz)", 30.0f, 20000.0f, 1000.0f),
    
    //Additional env 1
    std::make_unique<AudioParameterChoice>("custEnv1Choice","customEnv 1 choice", StringArray({"None","Osc 1 Tune", "Osc 1 Pan", "Osc 2 Tune", "Osc 2 Pan", "Osc 3 Tune", "Osc 3 Pan", "Osc 4 Tune", "Osc 4 Pan", "Lfo Depth", "Lfo Frequency","Low pass Filter Frequency", "High pass Filter Frequency"}), 0),
    std::make_unique<AudioParameterFloat>("custEnv1attack", "Custom Env Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv1decay", "Custom Env Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv1sustain", "Custom Env Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("custEnv1release", "Custom Env Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Additional env 2
    std::make_unique<AudioParameterChoice>("custEnv2Choice","customEnv 2 choice", StringArray({"None","Osc 1 Tune", "Osc 1 Pan", "Osc 2 Tune", "Osc 2 Pan", "Osc 3 Tune", "Osc 3 Pan", "Osc 4 Tune", "Osc 4 Pan", "Lfo Depth", "Lfo Frequency","Low pass Filter Frequency", "High pass Filter Frequency"}), 0),
    std::make_unique<AudioParameterFloat>("custEnv2attack", "Custom Env 2 Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv2decay", "Custom Env 2 Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv2sustain", "Custom Env 2 Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("custEnv2release", "Custom Env 2 Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Additional env 3
    std::make_unique<AudioParameterChoice>("custEnv3Choice","customEnv 3 choice", StringArray({"None","Osc 1 Tune", "Osc 1 Pan", "Osc 2 Tune", "Osc 2 Pan", "Osc 3 Tune", "Osc 3 Pan", "Osc 4 Tune", "Osc 4 Pan", "Lfo Depth", "Lfo Frequency","Low pass Filter Frequency", "High pass Filter Frequency"}), 0),
    std::make_unique<AudioParameterFloat>("custEnv3attack", "Custom Env 3 Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv3decay", "Custom Env 3 Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv3sustain", "Custom Env 3 Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("custEnv3release", "Custom Env 3 Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Additional env 4
    std::make_unique<AudioParameterChoice>("custEnv4Choice","customEnv 4 choice", StringArray({"None","Osc 1 Tune", "Osc 1 Pan", "Osc 2 Tune", "Osc 2 Pan", "Osc 3 Tune", "Osc 3 Pan", "Osc 4 Tune", "Osc 4 Pan", "Lfo Depth", "Lfo Frequency","Low pass Filter Frequency", "High pass Filter Frequency"}), 0),
    std::make_unique<AudioParameterFloat>("custEnv4attack", "Custom Env 4 Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv4decay", "Custom Env 4 Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv4sustain", "Custom Env 4 Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("custEnv4release", "Custom Env 4 Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Additional env 5
    std::make_unique<AudioParameterChoice>("custEnv5Choice","customEnv 5 choice", StringArray({"None","Osc 1 Tune", "Osc 1 Pan", "Osc 2 Tune", "Osc 2 Pan", "Osc 3 Tune", "Osc 3 Pan", "Osc 4 Tune", "Osc 4 Pan", "Lfo Depth", "Lfo Frequency","Low pass Filter Frequency", "High pass Filter Frequency"}), 0),
    std::make_unique<AudioParameterFloat>("custEnv5attack", "Custom Env 5 Attack (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv5decay", "Custom Env 5 Decay (ms)", 0.001f, 2000.0f, 1000.0f),
    std::make_unique<AudioParameterFloat>("custEnv5sustain", "Custom Env 5 Sustain (%)", 0.0f, 100.0f, 50.0f),
    std::make_unique<AudioParameterFloat>("custEnv5release", "Custom Env 5 Release (ms)", 0.001f, 2000.0f, 1000.0f),
    
    //Master Gain
    std::make_unique<AudioParameterFloat>("masterGain", "Master Gain", 0, 2.0f, 1.0f)
    

})
{
    //Setting up the synth
    mySynth.addSound(new MyFirstSynthSound());
    for(int i = 0; i < numVoices; ++i)
    {
        mySynth.addVoice(new MyFirstSynthVoice(numOscs, numEnvs, numFilters));
    }
    
    //Adding a listener for all the parameters
    parameters.state.addListener(this);
    
    //Adding the envolope parameters storing objects
    for(int i = 0; i < numEnvs; ++i)
    {
        envolopeParams.add(new EnvolopeParams());
    }
    
    //Adding oscillator parameters storing objects
    for(int i = 0; i < numOscs; ++i)
    {
        oscillatorParams.add(new OscParams());
    }

    //Adding LFO parameter storing objects
    for(int i = 0; i < numLFOs; ++i)
    {
        lfoParams.add(new SimpleParams(0, 2));
    }
    
    //Adding filter parameter storing objects
    for(int i = 0; i < numFilters; ++i)
    {
        filterParams.add(new SimpleParams(1, 1));
    }

    //Adding custom envolope storing objects
    for(int i = 0; i < numEnvs-3; ++i)  //CustomEnvChoice Setup
    {
        customEnvChoice.add(new SimpleParams(1, 1));
    }
    
    //Adding parameter for the master gain
    gainParam = parameters.getRawParameterValue("masterGain");
    
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
    mySynth.setCurrentPlaybackSampleRate(sampleRate); //Setting synth sample rate
    
    for(int i=0; i < numVoices; ++i)    //Initalising the synth voices
    {
        MyFirstSynthVoice* v = dynamic_cast<MyFirstSynthVoice*>(mySynth.getVoice(i));   //Getting the voice
        v -> init(sampleRate);  //Initilising voice sample rate
        setParamTargets();      //Getting update parameter targets
        v -> setParams(envolopeParams, oscillatorParams, lfoParams, filterParams, customEnvChoice); //Updating voice parameters
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
    //Checking if parameters updated
    bool updateParams = false;  //Ensure update params intially false and only activated if params updated
    if(paramsUpdated)
    {
        paramsUpdated = false;
        updateParams = true;    //Mark update params value as true
    }
    
    for(int i=0; i < numVoices; ++i)    //Iterating through each synth voice
    {
        
        MyFirstSynthVoice* v = dynamic_cast<MyFirstSynthVoice*>(mySynth.getVoice(i));
        if(updateParams)    //If parameters updated then set the params for each voice
        {
            v -> setParams(envolopeParams, oscillatorParams, lfoParams, filterParams, customEnvChoice);
        }
    }
    //Rendering synths next block
    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    //Applying master gain to samples
    if(prevGain!=*gainParam)    //Checking if gain has changed
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), prevGain, *gainParam);  //If changed ramping the gain
        prevGain = *gainParam;      //Setting prev gain to new gain
    }
    else
    {
        buffer.applyGain(0, buffer.getNumSamples(), prevGain);  //Otherwise just apply previous gain
    }
    
}

//==============================================================================
bool SynthTesterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SynthTesterAudioProcessor::createEditor()
{
    return new SynthTesterAudioProcessorEditor (*this);
    //return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void SynthTesterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    //Getting saved parameters
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void SynthTesterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //Saving presets
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthTesterAudioProcessor();
}

void SynthTesterAudioProcessor::setParamTargets()
{
    
    //Getting all envolope parameters
    for(int i = 0; i < envolopeParams.size(); ++i)
    {
        float adsr[4];
        for(int j=0; j < 4; ++j)
        {
            adsr[j] = *parameters.getRawParameterValue(paramID.getEnvolopeParamName(i, j));     //Getting envolope parameter
        }
        envolopeParams[i] -> setParams(adsr[0], adsr[1], adsr[2], adsr[3]); //Updating this envolopes parameter
    }
    
    //Getting all oscillator parameters
    for(int i = 0; i < oscillatorParams.size(); ++i)
    {
        float oscPar[5];
        for(int j=0; j < 5; ++j)
        {
            oscPar[j] = *parameters.getRawParameterValue(paramID.getOscParamName(i, j));    //Getting oscillator parameters
        }
        oscillatorParams[i] -> setParams(oscPar[0],oscPar[1], oscPar[2], oscPar[3], oscPar[4]);     //Updating oscillator parameters
    }
    
    //Getting LFO parameters
    for(int i = 0; i < lfoParams.size(); ++i)
    {
        float lfoPar[2];
        for(int j = 0; j < 2; ++j)
        {
            lfoPar[j] = (*parameters.getRawParameterValue(paramID.getLfoParamName(i, j)));  //Getting most recent LFO params
        }
        lfoParams[i] -> setParams(lfoPar);  //Updating lfo params
    }
    
    //Getting filter parameters
    for(int i = 0; i < filterParams.size(); ++i)
    {
        int choiceParam[1] = {(int)*parameters.getRawParameterValue(paramID.getFilterParamName(i, 0))}; //Getting filter choice value
        float filterPar[1]= {*parameters.getRawParameterValue(paramID.getFilterParamName(i, 1))};   //Getting filter frequency value
        filterParams[i] -> setParams(choiceParam, filterPar);   //Updating filter parameters
    }
    
    //Getting custom envolope choice parameters
    for(int i = 0; i < numEnvs - 3; ++i)
    {
        int customEnvChosen[1] = {(int)*parameters.getRawParameterValue(paramID.getEnvolopeParamName(3+i, 4))}; //Getting custom env choice
        float customEnvMax[1] = {0};
        if(customEnvChosen[0] > 0 && customEnvChosen[0] < paramID.numMaxParams+1)   //Getting the specific parameter value that the choice is pointing to if in range
        {
            customEnvMax[0] = *parameters.getRawParameterValue(paramID.getMaxParamName(customEnvChosen[0]-1));
        }
        customEnvChoice[i] -> setParams(customEnvChosen, customEnvMax);         //Updating custom env parameters
    }
}

void SynthTesterAudioProcessor::valueTreePropertyChanged(ValueTree& valTree, const Identifier& property)
{
    setParamTargets();      //Update params if the the parameters have changed
    paramsUpdated = true;   //Set parameters as updated
}
