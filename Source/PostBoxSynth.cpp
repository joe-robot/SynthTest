/*
  ==============================================================================

    PostBoxSynth.cpp
    Definition of a synth that uses envolopes to modify parameters upon playing
    Created: 15 Apr 2020
    Author:  B159113

  ==============================================================================
*/


#include "PostBoxSynth.h"

PostBoxSynth::PostBoxSynth(int numOscs, int numEnvs, int numFilters)
{

    for(int i = 0; i < numOscs; ++i)  //Initialising an owned array of oscillator parameter smoothers
    {
        smoothOscParams.add(new MultiSmooth(4));
    }
        
    for(int i = 0; i < numEnvs; ++i)  //Initialising an owned array of env parameter smoothers and ADSRs
    {
        smoothEnvParams.add(new MultiSmooth());
        myEnvs.add(new ADSR());
    }
        
    for(int i = 0; i < numFilters; ++i) //Initialising an owned array of filter parameter smoothers, adding filters and intialising filter types
    {
        auto* filter = synthFilters.add(new StereoIIRFilters());
        filter -> setFilterType(i == 1);
        smoothFilterParams.add(new SmoothChanges());
    }
        
    smoothLFOParams.add(new MultiSmooth(2));    //intialising smoother for LFO parameters
    
    for(int i = 0; i < 12; ++i) //Intialises max param values smoother
    {
        maxParamsVals.add(new SmoothChanges());
    }
        
}


void PostBoxSynth::setSampleRate(float sampleRate)
{
    for(int i = 0; i < smoothOscParams.size(); ++i) //Setting sample rate for oscillators and their parameter smoothers
    {
        smoothOscParams[i] -> setSampleRate(sampleRate);
        sourceOscs.setSampleRate(sampleRate);
    }
        
    for(int i = 0; i < smoothEnvParams.size(); ++i) //Setting sample rate for envolopess and their parameter smoothers
    {
        smoothEnvParams[i] -> setSampleRate(sampleRate);
        myEnvs[i] -> setSampleRate(sampleRate);
    }
        
    for(int i = 0; i < smoothFilterParams.size(); ++i) //Setting sample rate for filters and their parameter smoothers
    {
        smoothFilterParams[i] -> setSampleRate(sampleRate);
        synthFilters[i] -> setSampleRate(sampleRate);
    }
        
    smoothLFOParams[0] -> setSampleRate(sampleRate); //Setting sample rate for lfos and their parameter smoothers
    lfoOsc.setSampleRate(sampleRate);
    
    for(int i = 0; i < 12; ++i) //Setting sample rate for max param val smoother
    {
        maxParamsVals[i] -> setSampleRate(sampleRate);
    }
        
}

    
void PostBoxSynth::setParams(OwnedArray<EnvolopeParams>& envs, OwnedArray<OscParams>& oscs, OwnedArray<SimpleParams>& lfos, OwnedArray<SimpleParams>& filters, OwnedArray<SimpleParams>& paramEnvsChoice)
{
    for(int i = 0; i < envs.size(); ++i)    //Iterating through all envolope parameters
    {
        if(envs[i] -> getValSwitch() != envUpdate[i])   //Check if env updated since last checked
        {
            updateEnv(i, envs[i] -> getADSRParams());   //Update envolope with new parameters
            envUpdate[i] = envs[i] -> getValSwitch();   //update value switch
        }
    }
        
    for(int i = 0; i < oscs.size(); ++i)    //iterating through all oscillator parameters
    {
        if(oscs[i] -> getValSwitch() != oscUpdate[i])   //Check if osc updated since last checked
        {
            sourceOscs.setSourceType(i, oscs[i] -> getSourceType());   //updating source type immediatly
            updateOsc(i, oscs[i] -> getOscParams(0), oscs[i] -> getOscParams(1), oscs[i] -> getOscParams(2), oscs[i] -> getOscParams(3)); //Update Osc params
            oscUpdate[i] = oscs[i] -> getValSwitch();   //update value switch
        }
    }
        
    for(int i = 0; i < lfos.size(); ++i) //iterating through all lfo
    {
        if(lfos[i] -> getValSwitch() != lfoUpdate) //Check if lfo updated since last checked
        {
            updateLFOs(lfos[i] -> getParams(0), lfos[i] -> getParams(1));   //Update lfos with new params
            lfoUpdate = lfos[i] -> getValSwitch();      //update value switch
        }
    }
        
    for(int i = 0; i < filters.size(); ++i)     //iterating through all filters
    {
        if(filters[i] -> getValSwitch() != filterUpdate[i])     //check if filter update since last checked
        {
            updateFilters(i, filters[i] -> getChoiceParams(0), filters[i] -> getParams(0)); //Update filters with new params
            filterUpdate[i] = filters[i] -> getValSwitch(); //update the value switch
        }
    }
        
    for(int i = 0; i < paramEnvsChoice.size(); ++i) //iterating through all parameter envolopes
    {
        if(paramEnvsChoice[i] -> getValSwitch() != paramEnvUpdate[i])   //Check if parameter envolopes update since last checked
        {
            updateParamEnvs(i, paramEnvsChoice[i] -> getChoiceParams(0), paramEnvsChoice[i] -> getParams(0));   //Updare parameter envolopes
            paramEnvUpdate[i] = paramEnvsChoice[i] -> getValSwitch(); //Update value switch
        }
    }
}
    

void PostBoxSynth::startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    noteVelocity = velocity;            //Update note velocity param
    
    for(int i = 0; i < myEnvs.size(); ++i)  // reset all envolopes and set note on
    {
        myEnvs[i] -> reset();
        myEnvs[i] -> noteOn();
    }
    sourceOscs.playMode(true);              //Initiate oscillators to play mode
    sourceOscs.setOscsMidiInput(midiNoteNumber);    //set oscillator frequency
    
    //Mark as playing and not released
    released = false;
    playing = true;
        
}

void PostBoxSynth::stopNote(float /*velocity*/, bool allowTailOff)
{
    for(int i = 0; i < myEnvs.size(); ++i) //Send note off to all envolopes
        myEnvs[i] -> noteOff();

    released = true;    //Mark released as true
}
    
void PostBoxSynth::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    // iterate through the all samples
    for (int sampleIndex = startSample;   sampleIndex < (startSample+numSamples);   sampleIndex++)
    {
        float ampEnv = 0;           //Setting amp envolope to 0 intially
        float currentSample[2] = {0, 0};    //Setting current sample to 0 intially

        //If note is playing then
        if(playing)
        {
            //Update synth parameters
            updateParams();
            
            //Get next sample from the oscillators
            oscsNextSample(currentSample);
                
            //Apply effects to the oscillator samples
            applyFX(currentSample);
            
            //Get amplitude envolope
            ampEnv = myEnvs[0] -> getNextSample();
                
            //Mark as released and reset voice if amplitude envolope is below a threshold
            if(released && ampEnv<0.0001f)
            {
                resetVoice();
            }
        }
        
        // for each channel, write the currentSample float to the output
        for (int chan = 0; chan<outputBuffer.getNumChannels(); chan++)
        {
            // The output sample is scaled by 0.9 and note velocity so that it is not too loud by default
            outputBuffer.addSample (chan, sampleIndex, ampEnv * currentSample[chan] * noteVelocity * 0.9);
        }
    }
}


bool PostBoxSynth::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<PostBoxSynthSound*> (sound) != nullptr;
}

void PostBoxSynth::updateFilters(int filterNum, int filterMode, float filterFreq)
{
    if(filterMode != 0) //If filter mode isn't 0 (filter is off)
    {
        filterEnable[filterNum] = true;     //Ensure filter enabled
        synthFilters[filterNum] -> setFilterOrder(filterMode==2); //setting filter order immediatly
    }
    else
    {
        filterEnable[filterNum] = false;    //Otherwise disable the filter
    }
        
    if(!playing || !filterEnable[filterNum])    //If not playing or filter not enabled
    {
        //Update filter parameters immediatly, no smoothing needed
        smoothFilterParams[filterNum] -> init(filterFreq, filterFreq);
        synthFilters[filterNum] -> setFilterCutOffFreq(filterFreq);
    }
    else
    {
        smoothFilterParams[filterNum] -> setTargetVal(filterFreq); //Otherwise if playing then set target to desired cutoff
    }
}
    
void PostBoxSynth::updateLFOs(float lfoAmp, float lfoFreq)
{
    float lfoPar[2] = {lfoAmp, lfoFreq};
    if(!playing)    //If not playing update parameters no smoothing needed
    {
        smoothLFOParams[0] -> init(lfoPar, lfoPar);
        lfoOsc.setFrequency(lfoFreq);
    }
    else //Otherwise set target for updated parameters
    {
        smoothLFOParams[0] -> setTargetVal(lfoPar);
    }
}
    
void PostBoxSynth::updateEnv(int envNum, ADSR::Parameters thisADSR)
{
    float adsrParams[4] = {thisADSR.attack, thisADSR.decay, thisADSR.sustain, thisADSR.release};

    //Update env no smoothing needed if not playing
    if(!playing)
    {
        smoothEnvParams[envNum] -> init(adsrParams ,adsrParams);
        setADSR(envNum, adsrParams);
    }
    else //Set desired adsr as target if playing
    {
        smoothEnvParams[envNum] -> setTargetVal(adsrParams);
    }
}
        
void PostBoxSynth::updateOsc(int oscNum, float newTune, float newPan, float newMinAmp, float newMaxAmp)
{
    float oscParams[4] = {newTune, newPan, newMinAmp, newMaxAmp};
        
    //Update osc with no smoothing if not playing
    if(!playing)
    {
        smoothOscParams[oscNum] -> init(oscParams, oscParams);
    }
    else //otherwise set desired osc params as target if it is playing
    {
        smoothOscParams[oscNum] -> setTargetVal(oscParams);
    }
}

void PostBoxSynth::updateParamEnvs(int paramEnvNum, int envChoice, float paramResult)
{
    int oldEnvChoice = paramEnvParamsChosen[paramEnvNum]; //Get old value selected
    if(paramEnvParamsChosen[paramEnvNum] != (envChoice - 1))    //Checking if same as previously chosen param
    {
        if(oldEnvChoice != -1)  //Updating last chosen value paramters
        {
            numTimesChosen[oldEnvChoice] --;    //Remove one from the amount of times this parameter was chosen
            if(numTimesChosen[oldEnvChoice] < 1)    //If less than 1 than disengage parameter
            {
                envolopedParam[oldEnvChoice] = false;
                numTimesChosen[oldEnvChoice] = 0;
            }
        }
                
        //Updating new paramters
        paramEnvParamsChosen[paramEnvNum] = envChoice - 1;
            
        if(envChoice > 0)   //If not set to change no parameters then update the chosen parameter array positons
        {
            numTimesChosen[paramEnvParamsChosen[paramEnvNum]]++;
            envolopedParam[paramEnvParamsChosen[paramEnvNum]] = true;   //Ensure enabled
            updateMaxParamVals(paramEnvParamsChosen[paramEnvNum], paramResult);  //Update max parameter values
        }
    }
        
    else    //If the same as last chosen value then just update the parameter result
    {
        if(oldEnvChoice != -1)
        {
            updateMaxParamVals(oldEnvChoice, paramResult);  //Update max parameter value
        }
    }
}
    
void PostBoxSynth::updateMaxParamVals(int paramEnvNum, float paramResult)
{
    if(!playing)    //If not playing then set max param to desired value
    {
        maxParamsVals[paramEnvNum] -> init(paramResult, paramResult);
    }
    
    maxParamsVals[paramEnvNum] -> setTargetVal(paramResult);    //Otherwise set desired value as a target
}

void PostBoxSynth::oscsNextSample(float* sample)
{
    float envVals[2] = {myEnvs[1] -> getNextSample(), myEnvs[2] -> getNextSample()}; //Updating the oscillator x y envolopes

    sourceOscs.getNextVal(envVals, sample); //Get output of oscillators
}
    
void PostBoxSynth::applyFX(float* sample)
{
    applyLFO(sample);   //Apply LFO
    applyFilter(sample);    //Apply filter
}
    
void PostBoxSynth::applyLFO(float* sample)
{
    if(lfoAmp > 0.0001f)    //If lfo Amp not 0 then enable it otherwise don't do calculations
    {
        float lfoVal = lfoOsc.getNextSample() * lfoAmp; //get next lfo sample and apply lfo amp
        float lfoDepthInv = 1.0f - lfoAmp;  //Inverse depth calcilation
        
        for(int i = 0; i < 2; ++i)  //For each channel calculate the next sample with the LFO applied
            sample[i] = (lfoVal + lfoDepthInv) * sample[i];
    }
}
    
void PostBoxSynth::applyFilter(float* sample)
{
    for(int i = 0; i < 2; ++i)  //For each filter
    {
        if(filterEnable[i]) //Check filter is enabled
        {
            synthFilters[i] -> setFilterCutOffFreq(getParamVal(10 + i, smoothFilterParams[i] -> getNextVal())); //Set cutoff frequency for the filter
            synthFilters[i] -> getNextSample(sample);   //Get updated samples when processed through the filter
        }
    }
}
    
void PostBoxSynth::resetVoice()
{
    clearCurrentNote(); //Clear Current Note
    playing = false;    //Mark stopped playing note
    released = false;   //Release note
    updateParams();     //Update parameters to targets
    sourceOscs.playMode(false); //Set source oscs to not playing
}
    
    
void PostBoxSynth::setADSR(int envNum, float adsrVals[4])
{
    //Setting ADSR parameters
    ADSR::Parameters myADSR;
        
    //Creating ADSR params
    myADSR.attack = adsrVals[0];
    myADSR.decay = adsrVals[1];
    myADSR.sustain = adsrVals[2];
    myADSR.release = adsrVals[3];
        
    setADSR(envNum, myADSR);  //Setting envolope with passed parameters
        
}
    
void PostBoxSynth::setADSR(int envNum, ADSR::Parameters adsrParams)
{
    myEnvs[envNum] -> setParameters(adsrParams);   //Setting envolope with passed parameters
}
    
    

    
void PostBoxSynth::getNextParamEnvVals()
{
    std::vector<int> alreadyPickedVals;         //Vector to store already picked vals
    
    for(int i = 0; i < myEnvs.size()-3; ++i)    //Iterate through param envolopes
    {
        if( paramEnvParamsChosen[i] > -1)       //If param envolope active
        {
            int chosenParam = paramEnvParamsChosen[i];  //Get chosen parameter of envolope
            bool valAlreadyChosen = false;
            for( int val : alreadyPickedVals)           //Check if value already chosen
            {
                if(val == chosenParam)
                {
                    valAlreadyChosen = true;
                    break;
                }
            }
            if(valAlreadyChosen)    //If value already choesen then multiply by previous value
            {
                envolopedParamVals[chosenParam] = envolopedParamVals[chosenParam] * myEnvs[i+3] -> getNextSample();
            }
            else            //Otherwise reset envoloped param num
            {
                envolopedParamVals[chosenParam] = myEnvs[i+3] -> getNextSample();
                alreadyPickedVals.push_back(chosenParam);   //Add parameter to already picked vector
            }
        }
        
    }
        
        
}
    
float PostBoxSynth::getParamVal(int paramNum, float paramVal)
{
    if(envolopedParam[paramNum])    //If parameter to be envoloped return envoloped result
    {
        return (paramVal + (maxParamsVals[paramNum] -> getNextVal() - paramVal) * envolopedParamVals[paramNum]);
    }
        
    return paramVal;    //Otherwise return entered parameter value
}

    
void PostBoxSynth::updateParams()
{
    getNextParamEnvVals(); //Get next parameter envolope values
    updateOscParams();      //Update oscillator parameters
    updateLFOParams();      //Update LFO parameters
    updateEnvParams();      //Update envolope parameters
}

void PostBoxSynth::updateEnvParams()
{
    for(int i = 0; i < myEnvs.size(); ++i)
    {
        if(smoothEnvParams[i] -> checkChanging())   //Check if parameters are changing
        {
            float adsrVals[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            if(!playing)     //If not playing then update envolope parameters to target
            {
                smoothEnvParams[i] -> setToTarget();
            }
                
            smoothEnvParams[i] -> getNextVal(adsrVals); //Get next smoothed value

            setADSR(i, adsrVals);   //Set envolope ADSR with update values
        }

    }
}
    
void PostBoxSynth::updateOscParams()
{
    for(int i = 0; i < 4; ++i)  //iterate through all oscillators
    {
        float osc[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        if(!playing)                        //If not playing then set smoother to target
        {
            smoothOscParams[i] -> setToTarget();
        }
        smoothOscParams[i] -> getNextVal(osc);              //Get next smoothed params
        
        //Update source oscillator parameters
        sourceOscs.setOscMinMaxVolume(i, osc[2], osc[3]);
        sourceOscs.setPanAmount(i, getParamVal(2 * i + 1, osc[1]));
        sourceOscs.setTuneAmount(i, getParamVal(2 * i, osc[0]));
    }
}
    
    
void PostBoxSynth::updateLFOParams()
{
    float lfoParams[2] = {0,0};
    if(!playing)                    //If not playing then set LFO params to target
    {
        smoothLFOParams[0] -> setToTarget();
    }
    smoothLFOParams[0] -> getNextVal(lfoParams);    //Get next LFO parameter values
        
    lfoOsc.setFrequency(getParamVal(9, lfoParams[1]));  //Set lfo Frequency
        
    lfoAmp = getParamVal(8, lfoParams[0]);  //Get LFO amplitude
}
  
