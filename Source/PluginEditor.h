/*
  ==============================================================================

    PluginEditor.h
    This class deals with the UI of the plugin
    Created: 20 Apr 2020
    Author:  B159113

  ==============================================================================
*/


#pragma once

//Including required files
#include <JuceHeader.h>
#include "PluginProcessor.h"


// =================================
// =================================
// New Rotary Design

/*!
 @class NewRotaryDesign
 @abstract Class for defining new slider look and feel
 @discussion called multiple times by the editor to set look of sliders
 
 @namespace none
 @updated 2020-04-24
 */
class NewRotaryDesign : public LookAndFeel_V4   //Inheriting from look and feel and overriding required methods
{
public:
    /**
     * Method to set main colour of the slider
     *
     * @param myColour is the main colour of the slider
     *
    */
    void setColour(Colour myColour) //Method to set the main colour of the slider
    {
        mainColour = myColour;
    }
    
    /**
     * Overrided draw slider method to draw a custom slider
     *
     * @params deal with slider size and position to enable drawing it in the correct place
     *
     */
    void drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override
    {
        float diameter = jmin(width, height)*0.99; //Finding maximim diameter possible
        float radius = diameter/2;                  //Getting radius
        float centrex = width/2 + x;                //Finding centre position
        float centrey = height/2 +y;
        float rx = centrex - radius;                //Finding top left corner position
        float ry = centrey - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle)); //Finding angle of slider
        float dialWidth = 5.0;      //Defining width of dial
        
        //Setting colour of main slider and drawing the ellipse
        g.setColour(mainColour);
        g.fillEllipse(rx, ry, diameter, diameter);
        
        //Drawing a black border for the slider
        g.setColour(Colours::black);
        g.drawEllipse(rx+(diameter * 0.025), ry + (diameter * 0.025), diameter-(diameter * 0.05), diameter-(diameter * 0.05),0.05 * diameter);
        
        //Drawing the dial tick so it rotates around the centre of the slider properly
        Path dialTick;
        dialTick.addRectangle(-dialWidth/2,-radius+(diameter * 0.025), dialWidth, (2 * radius/3.0f));//-(diameter * 0.025), -radius
        g.setColour(Colours::black);
        g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centrex, centrey));
        
    }
    
    /**
     * Overrided create Slider Text Box method as to avoid having slider values being cut off
     *
     * @param slider is the sider the text box is connected
     *
    */
    Label* createSliderTextBox (Slider& slider) override
    {
        Label* newLabel = LookAndFeel_V4::createSliderTextBox(slider);  //Getting the label
        newLabel -> setMinimumHorizontalScale(0.0001);          //Setting minimum horizintal to very small to avoid cut off
        return newLabel;        //Returning the label
    }
    
private:
    Colour mainColour = Colours::blue;  //Default colour of slider
};



// =================================
// =================================
// SynthTesterAudioProcessorEditor

/*!
 @class SynthTesterAudioProcessorEditor
 @abstract The main editor that creates the UI for the plugin
 @discussion creates plugin UI
 
 @namespace none
 @updated 2020-04-24
 */
class SynthTesterAudioProcessorEditor  : public AudioProcessorEditor,   //Inheriting from juce editor, to make it an editor
                                         public ComboBox::Listener      //Inheriting from combobox listener to make changes based on a combo box selection
{
public:
    //==============================================================================
    /** Constructors*/
    SynthTesterAudioProcessorEditor (SynthTesterAudioProcessor&);   //Editor requires reference to processor to get the parameters
    /** Destructor*/
    ~SynthTesterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;    //Method to paint ui components
    void resized() override;            //Method called when ui resized
    
private:
    /**
     * Draw container draws a container in a specifed position and size and colour
     *
     * @param posX is the x positon of container top left corner
     * @param posY is the y positon of container top left corner
     * @param sizeX is the x size of the container
     * @param sizeY is the y size of the container
     * @param color is the colour of the container
     * @param Graphics& is a referece to the graphics of the UI to allowing drawing rectangles
     *
    */
    void drawContainer(float posX, float posY, float sizeX, float sizeY, Colour color, Graphics&);

    /**
     * Adding sliders to slider array, setting their look and feel, units and  adding a label to the sliders
     *
     * @param sliderAraay is the array of sliders being used
     * @param newLookAndFeel is the the look and feel of the slider
     * @param labelName is the string of text for the label
     * @param suffix is the unit of the parameter being controlled by the slider
     * @param labelToLeft is the position on the label (true means to the left, false means above)
     * @param noLabel sets if there is a label at all (true means no label, false means there is a label)
     *
    */
    void addSlider(OwnedArray<Slider>& sliderArray, LookAndFeel* const newLookAndFeel, std::string labelName, std::string suffix = "", bool labelToLeft = true, bool noLabel = false);
    
    /**
     * Adding comboBoxes to comboBox array, filling the combo box with elements and setting the label for the comboBox
     *
     * @param comboArray is the array of comboBoxes being used
     * @param comboFill is the array of stings for what is contained in the sliders
     * @param numComboElements is the number of elements to be in the slider
     * @param labelName is the label string for the comboBox
     * @param comboBoxName is the combo box name so it can be identiified in a callback
     *
    */
    void addComboBox(OwnedArray<ComboBox>& comboArray, std::string* comboFill, int numComboElements, std::string labelName, std::string comboBoxName = "");
    
    /**
     * Overrided from comboBox listener, is called when combo box is changed, this method changes the slider attachment to the maxsliders
     * based on the selected value in the combo box
     *
     * @param comboBoxThatHasChanged is a pointer to combobox that has changed
     *
    */
    void comboBoxChanged (ComboBox *comboBoxThatHasChanged) override;
    
    /**
     * Method to apply fonts to an array of labels
     *
     * @param labels is the array of labels
     * @param labelFont is the font to apply to the labels
     *
    */
    void setLabelFonts(OwnedArray<Label>& labels, Font labelFont);
    
    /**
     * Method to define font size and look
     *
     * @param thisFont is the font to change
     * @param height is the height of the font
     * @param bold makes the font bold (true = bold, false = regular)
     *
    */
    void setFontHeight(Font& thisFont, float height, bool bold);
    
    /**
     * Funciton for placing sliders into a slider container based on number of divisions the container is to be split into. And to place the labels and text boxes.
     *
     * @param sliderArray is the array of sliders being used
     * @param minSliderNum is the  min slider number in the array to place
     * @param numSliders is the number of sliders from the min position to place from the array
     * @param containerPosX is the slider container X position (as percentage of width)
     * @param containerPosY is the slider container Y position (as percentage of height)
     * @param containerWidth is the slider container X size (as percentage of width)
     * @param containerHeight is the slider container Y size (as percentage of height)
     * @param numXDiv is the number of divisions to split container into horizontally
     * @param numYDiv is the number of divisions to splt the container into vertically
     * @param numHorizontal is the number of sliders alllowed in a horizontal line
     * @param xOffset is the offset number of horizontal divisions before drawing sliders
     * @param yOffset is the offset number of vertical divisions before drawing sliders
     * @param labelAbove is the label position (true is above, false is to the left)
     * @param centred (true is centred horizontally and vertically, false is uncentred)
     * @param borderXpercentage allows for setting what percentage each side (X) of the container is left for border
     * @param borderYpercentage allows for setting what percentage each side (Y) of the container is left for border
     *
    */
    void setSliderPositions(OwnedArray<Slider>& sliderArray, int minSliderNum, int numSliders, float containerPosX, float containerPosY, float containerWidth, float containerHeight, int numXDiv, float numYDiv, int numHorizontal, int xOffset, int yOffset, bool labelAbove = false, bool centred = false, float borderXpecentage = 0.0f, float borderYpercentage = 0.0f); //Could include it wihtout the horizontal and vertical offset
    
    /**
     * Funciton for placing sliders into a slider container based on number of divisions the container is to be split into. And to place the labels and text boxes.
     *
     * @param newComboBoxes is the array of comboBoxes being used
     * @param comboNum is the  min comboBox number in the array to place
     * @param containerPosX is the container X position (as percentage of width)
     * @param containerPosY is the container Y position (as percentage of height)
     * @param containerWidth is the container X size (as percentage of width)
     * @param containerHeight is the container Y size (as percentage of height)
     * @param numXDiv is the number of divisions to split container into horizontally
     * @param numYDiv is the number of divisions to splt the container into vertically
     * @param xDivPos is which x div the combobox is placed in
     * @param yDivPos  is which y div the combobox is placed in
     * @param xFillPercentage is how much combobox fills the horizontal division if >1 fills adjacent divisions as well
     * @param yFillPercentage is how mich combobox fills the vertical division
     *
    */
    void setComboPosition(OwnedArray<ComboBox>& newComboBoxes, int comboNum, float containerPosX, float containerPosY, float containerWidth, float containerHeight, int numXDiv, int numYDiv, int xDivPos, int yDivPos, float xFillPercentage, float yFillPercentage);
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthTesterAudioProcessor& processor;

    //Array of sliders
    OwnedArray<Slider> uiSliders;
    
    //Array for ui labels and titles
    OwnedArray<Label> uiLabels;
    OwnedArray<Label> boldUiLabels;
    OwnedArray<Label> titleLabels;
    
    //Number of oscillators, envolopes and filters
    int numOscs = 4;
    int numEnvs = 8;
    int numFilters = 2;
    
    //Array to store slider designs in
    OwnedArray<NewRotaryDesign> rotaryDesign;
    
    //Amount to decrease heights by to fit parameter envolopes in
    float hDecrease = 0.87f;
    
    //An array that has all the container sizes as percentage of window height and width
    float containerSizes[16] = {0.6, 0.55f * hDecrease, //Osc Container
                                0.6, 0.2f * hDecrease,  //Env X Container
                                0.15, 0.55f * hDecrease,   //Env Y Container
                                0.15, 0.2f * hDecrease,     //Lfo Container
                                0.25, 0.35f * hDecrease,    //Amp Env Container
                                0.25, 0.4f * hDecrease,     //Filter Container
                                0.1, 0.3475,           //Master Gain Container
                                0.9, 0.3475            //Param Env Container
                                };
    
    //An array that has all the slider container sizes as percentage of window height and width
    float sliderContainerSizes[30] =   {4, 0.288, 0.235f * hDecrease, //Osc Slider Container
                                        1, 0.15, 0.3f * hDecrease, //XY graph box
        
                                        1, 0.584, 0.2f * hDecrease, //EnvX SLider Container
                                        1, 0.15, 0.5f * hDecrease, //EnvY SliderContainer
        
                                        1, 0.14, 0.14f * hDecrease, //Lfo slider container
                                        1, 0.225, 0.28f * hDecrease,    //Amp env elider container
                                        2, 0.2375, 0.1f * hDecrease, //LP HP Slider Container
                                        1, 0.1f, 0.3475,    //Master Gain Slider Container
                                        5, 0.8f, 0.0655,    //Param Env Slider Container
                                        1, 0.8f, 0.0655    //Max Param Env Slider Container
                                        };
    
    //An array that has all the slider container positions as percentage of window height and width
    float sliderContainerPositions[34] =   {0.008, 0.06f * hDecrease, //Osc 1 Slider Container
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
                                            
                                            0.9f, 0.6545f, //Master Gain Slider Container
        
                                            0.1f, 0.6545f,  //Param 1 slider Env Container
                                            0.1f, 0.724f,  //Param 2 slider Env Container
                                            0.1f, 0.7935f, //Param 3 slider Env Container
                                            0.1f, 0.863f,  //Param 4 slider Env Container
                                            0.1f, 0.9325f, //Param 5 slider Env Container
                                            };
    
    //An array that has all the container sizes as percentage of window height and width
    float containerPositions[16] = {0, 0.0f * hDecrease, //Osc Container
                                    0, 0.55f * hDecrease,  //Env X Container
                                    0.6, 0.0f * hDecrease,   //Env Y Container
                                    0.6, 0.55f * hDecrease,     //Lfo Container
                                    0.75, 0.0f * hDecrease,    //Amp Env Container
                                    0.75, 0.35f * hDecrease,     //Filter Container
                                    0.9, 0.6525f, //Master Gain Container
                                    0, 0.6525f,          //Param env Container
                                    };
    
    float sliderSizes[2] = {0.0929, 0.072f * hDecrease}; //Slider Sizes
    
     //An array that has all the slider arrange information which references the position array, size array layout array, offset array and label positon
    int sliderArrangeInfo[110] ={0, 0, 0, 0, 0,//Osc 1   position ref, size ref, layout ref, offset ref, label pos
                                1, 0, 0, 1, 0,//Osc 2
                                2, 0, 0, 2, 0,//Osc 3
                                3, 0, 0, 3, 0,//Osc 4
                                5, 2, 1, 4, 0,//Env X
                                6, 3, 2, 5, 1,//Env Y
                                8, 5, 4, 8, 0,//Amp Env
        
                                12, 8, 6, 10, 0, //Param 1 slider Env
                                13, 8, 6, 10, 0, //Param 2 slider Env
                                14, 8, 6, 10, 0, //Param 3 slider Env
                                15, 8, 6, 10, 0,//Param 4 slider Env
                                16, 8, 6, 10, 0,//Param 4 slider Env
        
                                7, 4, 3, 6, 0,//Lfo row 1
                                7, 4, 3, 7, 0,//Lfo row 2

                                9, 6, 5, 9, 0,//Low Pass Filter
                                10,6, 5, 9, 0,//High Pass Filter
                                
                                11, 7, 8, 12, 1, //Master Gain Slider
        
                                12, 9, 7, 11, 0, //Param 1 max slider
                                13, 9, 7, 11, 0, //Param 2 max slider Env
                                14, 9, 7, 11, 0, //Param 3 max slider Env
                                15, 9, 7, 11, 0, //Param 4 max slider Env
                                16, 9, 7, 11, 0 //Param 4 max slider Env
                                };
    
    //Slider layout array that defines number of sliders in the slider container, the x and y divisions and number of sliders per horizontal
    //Num sliders, x div, y div, num sliders per horizintal
    float sliderLayout[36] =   {4, 3, 3, 2,     //Osc Sliders
                                4, 4, 3, 4,     //EnvX Sliders
                                4, 3, 4, 1,     //EnvY Sliders
                                1, 2, 2, 1,     //LFO SLiders
                                4, 2, 2, 2,     //Amp Env Sliders
                                2, 2, 1, 2,     //Filter sliders
                                4, 7, 1, 4,     //Param Env Sliders
                                1, 7, 1, 1,     //Param Env Max Val SLiders
                                1, 1, 1, 1      //Master Gain Slider
                                };
    
    //Slider Offset array that defines slider x division offset and y divsion offset
    float sliderOffsets[26] =  {0, 1,   //Osc 1 Sliders
                                1, 1,   //Osc 2 Sliders
                                0, 0,   //Osc 3 Sliders
                                1, 0,   //Osc 4 Sliders
                                0, 1,   //Env X Sliders
                                1, 0,   //Osc Y Sliders
                                0, 0,   //Lfo first Slider
                                1, 1,   //Lfo second Slider
                                0, 0,   //Amp Env Sliders
                                0, 0,   //Filter Siders
                                3, 0,   //Param Env Sliders
                                2, 0,   //Param Max Val Sliders
                                0, 0    //Mater Gain Slider
                                };
    //Arrays defining the colours of the containers
    Colour containerColours[8] = {Colours::darkgrey, Colours::slategrey, Colours::slategrey, Colours::darkgrey, Colours::darkgrey, Colours::dimgrey, Colours::darkgrey, Colours::grey};
    Colour sliderContainerColours[9] = {Colours::dimgrey, Colours::lightgrey, Colours::slategrey, Colours::slategrey,  Colours::dimgrey, Colours::dimgrey, Colours::darkgrey, Colours::black, Colours::darkgrey};
    
    //Array defining the posible slider colours
    Colour sliderColours[4] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};

    //Arrays defining label Names
    std::string  oscLabelNames[4] = {"Tune (ST)", "Pan", "Min Amp", "Max Amp"};
    std::string  envLabelNames[4] = {"Attack", "Decay", "Sustain", "Release"};
    std::string  filterLabels[2] = {"Filter Mode", "Cut-Off Freq"};
    std::string  lfoLabels[2] = {"Amp", "Freq"};
    
    //Arrays defining title Names
    std::string  nameLabels[7] = {"Sources", "X Axis Envolope", "Y Axis Envolope", "Lfo", "Amplitude Envolope", "Filters","Master Volume"};
    std::string filterNames[2] = {"Low Pass Filter", "High Pass Filter"};
    
    //The fonts used
    Font textFont = {12.0f};
    Font componentFont = {12.0f};
    Font titleFont = {12.0f};
    
    //Defing combo boxes and the arrays of strings to fill them with
    OwnedArray<ComboBox> comboBoxes;
    std::string comboBoxFill[6] = {"None", "Sine", "Square", "Triangle", "Saw", "Noise"};
    std::string comboBoxFillcustEnv[13] = {"None","Osc 1 Tune", "Osc 1 Pan", "Osc 2 Tune", "Osc 2 Pan", "Osc 3 Tune", "Osc 3 Pan", "Osc 4 Tune", "Osc 4 Pan", "Lfo Depth", "Lfo Frequency","Low pass Filter Frequency", "High pass Filter Frequency"};
    
    //Array of slider and comboBox attachments to like sliders and comboboxes to processor parameters
    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> comboAttachment;
    
    //The width and height of the ui box
    float width;
    float height;
    
    //Storing the value at which the max sliders exist
    int storeSlider = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthTesterAudioProcessorEditor)
};


