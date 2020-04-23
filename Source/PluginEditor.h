#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class NewRotaryDesign : public LookAndFeel_V4
{
public:
    void setColour(Colour myColour)
    {
        mainColour = myColour;
    }
    
    void drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override
    {
        float diameter = jmin(width, height)*0.99; //Finding maximim diameter possible
        float radius = diameter/2;
        float centrex = width/2 + x;
        float centrey = height/2 +y;
        float rx = centrex - radius;
        float ry = centrey - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        float dialWidth = 5.0;
        
        /*Rectangle<float> area(x,y,width,height);
        g.drawRect(area);
        g.fillAll(Colours::hotpink);*/
        
        g.setColour(mainColour);
        g.fillEllipse(rx, ry, diameter, diameter);
        
        g.setColour(Colours::black);
        g.drawEllipse(rx+(diameter * 0.025), ry + (diameter * 0.025), diameter-(diameter * 0.05), diameter-(diameter * 0.05),0.05 * diameter);
        
        Path dialTick;
        dialTick.addRectangle(-dialWidth/2,-radius+(diameter * 0.025), dialWidth, (2 * radius/3.0f));//-(diameter * 0.025), -radius
        g.setColour(Colours::black);
        g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centrex, centrey));
        
    }
    
private:
    Colour mainColour = Colours::blue;
};


//==============================================================================
/**
*/
class SynthTesterAudioProcessorEditor  : public AudioProcessorEditor,
public ComboBox::Listener
{
public:
    SynthTesterAudioProcessorEditor (SynthTesterAudioProcessor&);
    ~SynthTesterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void drawContainer(float posX, float posY, float sizeX, float sizeY, Colour color, Graphics&);

    void addSlider(OwnedArray<Slider>& sliderArray, LookAndFeel* const newLookAndFeel, std::string labelName, bool labelToLeft = true);
    
    void addComboBox(OwnedArray<ComboBox>& comboArray, std::string* comboFill, int numComboElements, std::string labelName, std::string comboBoxName = "");
    
    void comboBoxChanged (ComboBox *comboBoxThatHasChanged) override;
    
private:
    
    
    
    void setLabelFonts(OwnedArray<Label>& labels, Font labelFont);
    
    void setFontHeight(Font& thisFont, float height, bool bold);
    
    void setSliderPositions(OwnedArray<Slider>& sliderArray, int minSliderNum, int numSliders, float containerPosX, float containerPosY, float containerWidth, float containerHeight, int numXDiv, float numYDiv, int numHorizontal, int xOffset, int yOffset, bool labelAbove = false, bool centred = false, float borderXpecentage = 0.0f, float borderYpercentage = 0.0f); //Could include it wihtout the horizontal and vertical offset
    
    void setComboPosition(OwnedArray<ComboBox>& newComboBoxes, int comboNum, float containerPosX, float containerPosY, float containerWidth, float containerHeight, int numXDiv, int numYDiv, int xDivPos, int yDivPos, float xFillPercentage, float yFillPercentage);
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthTesterAudioProcessor& processor;

    OwnedArray<Slider> uiSliders;
    
    OwnedArray<Label> uiLabels;
    OwnedArray<Label> boldUiLabels;
    OwnedArray<Label> titleLabels;
    

    int numOscs = 4;
    int numEnvs = 8;
    int numFilters = 2;
    
    OwnedArray<NewRotaryDesign> rotaryDesign;
    
    float hDecrease = 0.87f;
    
    float containerSizes[14] = {0.6, 0.55f * hDecrease, //Osc Container
                                0.6, 0.2f * hDecrease,  //Env X Container
                                0.15, 0.55f * hDecrease,   //Env Y Container
                                0.15, 0.2f * hDecrease,     //Lfo Container
                                0.25, 0.35f * hDecrease,    //Amp Env Container
                                0.25, 0.4f * hDecrease,     //Filter Container
                                0.9, 0.3475            //Param Env Container
                                };
    
    float sliderContainerSizes[27] =   {4, 0.288, 0.235f * hDecrease, //Osc Slider Container
                                        1, 0.15, 0.3f * hDecrease, //XY graph box
        
                                        1, 0.584, 0.2f * hDecrease, //EnvX SLider Container
                                        1, 0.15, 0.5f * hDecrease, //EnvY SliderContainer
        
                                        1, 0.14, 0.14f * hDecrease, //Lfo slider container
                                        1, 0.225, 0.28f * hDecrease,    //Amp env elider container
                                        2, 0.2375, 0.1f * hDecrease, //LP HP Slider Container
                                        5, 0.8f, 0.0655,    //Param Env Slider Container 0.0695
                                        1, 0.8f, 0.0655    //Max Param Env Slider Container 0.0695
                                        };
    
    float sliderContainerPositions[32] =   {0.008, 0.06f * hDecrease, //Osc 1 Slider Container
                                            0.304, 0.06f * hDecrease, //Osc 2 Slider Container
                                            0.008, 0.305f * hDecrease, //Osc 3 Slider Container
                                            0.304, 0.305f * hDecrease, //Osc 4 Slider Container
                                            0.2, 0.15f * hDecrease, //XY graph box
        
                                            0.008, 0.55f * hDecrease, //EnvX SLider Container
                                            0.6, 0.05f * hDecrease, //EnvY SliderContainer
        
                                            0.605, 0.6f * hDecrease, //Lfo slider Container
                                            0.7625, 0.05f * hDecrease, //Amp Env Slider Container
                                            0.75625, 0.45f * hDecrease, //LP slider Container
                                            0.75625, 0.62f * hDecrease, //HP slider Container
                                            
                                            0.1f, 0.6545f,  //Param 1 slider Env Container
                                            0.1f, 0.724f,  //Param 2 slider Env Container
                                            0.1f, 0.7935f, //Param 3 slider Env Container
                                            0.1f, 0.863f,  //Param 4 slider Env Container
                                            0.1f, 0.9325f //Param 5 slider Env Container
                                            };
    
    /*
     0.03f, 0.6525f + 0.002f,  //Param 1 slider Env Container
     0.03f, 0.722f + 0.002f,  //Param 2 slider Env Container
     0.03f, 0.7915f + 0.002f, //Param 3 slider Env Container
     0.03f, 0.861f + 0.002f,  //Param 4 slider Env Container
     0.03f, 0.9305f + 0.002f
     0.0758f * hDecrease}
     */
    
    
    float containerPositions[14] = {0, 0.0f * hDecrease, //Osc Container
                                    0, 0.55f * hDecrease,  //Env X Container
                                    0.6, 0.0f * hDecrease,   //Env Y Container
                                    0.6, 0.55f * hDecrease,     //Lfo Container
                                    0.75, 0.0f * hDecrease,    //Amp Env Container
                                    0.75, 0.35f * hDecrease,     //Filter Container
                                    0, 0.6525f          //Param env Container
                                    };
    
    float sliderSizes[2] = {0.0929, 0.072f * hDecrease}; //Slider Sizes
    
    int sliderArrangeInfo[105] ={0, 0, 0, 0, 0,//Osc 1   position ref, size ref, layout ref, offset ref, label pos
                                1, 0, 0, 1, 0,//Osc 2
                                2, 0, 0, 2, 0,//Osc 3
                                3, 0, 0, 3, 0,//Osc 4
                                5, 2, 1, 4, 0,//Env X
                                6, 3, 2, 5, 1,//Env Y
                                8, 5, 4, 8, 0,//Amp Env
        
                                11, 7, 6, 10, 0, //Param 1 slider Env
                                12, 7, 6, 10, 0, //Param 2 slider Env
                                13, 7, 6, 10, 0, //Param 3 slider Env
                                14, 7, 6, 10, 0,//Param 4 slider Env
                                15, 7, 6, 10, 0,//Param 4 slider Env
        
                                7, 4, 3, 6, 0,//Lfo row 1
                                7, 4, 3, 7, 0,//Lfo row 2

                                9, 6, 5, 9, 0,//Low Pass Filter
                                10,6, 5, 9, 0,//High Pass Filter
                                
                                11, 8, 7, 11, 0, //Param 1 max slider
                                12, 8, 7, 11, 0, //Param 2 max slider Env
                                13, 8, 7, 11, 0, //Param 3 max slider Env
                                14, 8, 7, 11, 0, //Param 4 max slider Env
                                15, 8, 7, 11, 0 //Param 4 max slider Env
                                };
    
    float sliderLayout[32] =   {4, 3, 3, 2, //Num sliders, x div, y div, num sliders per horizintal
                                4, 4, 3, 4,
                                4, 3, 4, 1,
                                1, 2, 2, 1,
                                4, 2, 2, 2,
                                2, 2, 1, 2,
                                4, 7, 1, 4,
                                1, 7, 1, 1
                                };
    
    float sliderOffsets[24] =  {0, 1,   //x offset, y offset
                                1, 1,
                                0, 0,
                                1, 0,
                                0, 1,
                                1, 0,
                                0, 0,
                                1, 1,
                                0, 0,
                                0, 0,
                                3, 0,
                                2, 0
                                };
                
    Colour containerColours[7] = {Colours::darkgrey, Colours::slategrey, Colours::slategrey, Colours::darkgrey, Colours::darkgrey, Colours::dimgrey, Colours::grey};
    Colour sliderContainerColours[8] = {Colours::dimgrey, Colours::lightgrey, Colours::slategrey, Colours::slategrey,  Colours::dimgrey, Colours::dimgrey, Colours::darkgrey, Colours::darkgrey};
    
    Colour sliderColours[4] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};

    std::string  oscLabelNames[4] = {"Tune", "Pan", "Min Amp", "Max Amp"};
    std::string  envLabelNames[4] = {"Attack", "Decay", "Sustain", "Release"};
    std::string  filterLabels[2] = {"Filter Mode", "Cut-Off Freq"};
    std::string  lfoLabels[2] = {"Amp", "Freq"};
    std::string  nameLabels[8] = {"Sources", "X Axis Envolope", "Y Axis Envolope", "Lfo", "Amplitude Envolope", "Filters"};
    std::string filterNames[2] = {"Low Pass Filter", "High Pass Filter"};
    Font textFont = {12.0f};
    Font componentFont = {12.0f};
    Font titleFont = {12.0f};

    OwnedArray<ComboBox> comboBoxes;
    std::string comboBoxFill[6] = {"None", "Sine", "Saw", "Triangle", "Square", "Noise"};
    std::string comboBoxFillcustEnv[13] = {"None","Osc 1 Tune", "Osc 1 Pan", "Osc 2 Tune", "Osc 2 Pan", "Osc 3 Tune", "Osc 3 Pan", "Osc 4 Tune", "Osc 4 Pan", "Lfo Depth", "Lfo Frequency","Low pass Filter Frequency", "High pass Filter Frequency"};
    
    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> comboAttachment;
    
    float width;
    float height;
    
    int storeSlider = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthTesterAudioProcessorEditor)
};


