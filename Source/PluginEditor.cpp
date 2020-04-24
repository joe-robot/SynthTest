/*
  ==============================================================================

    Plugin Editor - to generate the UI

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthTesterAudioProcessorEditor::SynthTesterAudioProcessorEditor (SynthTesterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    //Filling array with possible slider styles to be used by all sliders
    for(int i = 0; i < 4; ++i)
    {
        rotaryDesign.add(new NewRotaryDesign());
        rotaryDesign[i] -> setColour(sliderColours[i]);
    }
    
    //Adding the oscillator sliders and attaching them to appropriate parameters
    for(int i = 0; i < numOscs; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            addSlider(uiSliders, rotaryDesign[i], oscLabelNames[j]);
            sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getOscParamName(i, j+1), *uiSliders[uiSliders.size()-1]));
        }
    }
    
    //Adding the envolope sliders and attaching them to appropriate parameters
    for(int i = 0; i < numEnvs; ++i)
    {
        bool labelPos = i == 1 ? false : true;  //Placing the ENV Y sliders label above
        for(int j = 0; j < 4; ++j)
        {
            addSlider(uiSliders, rotaryDesign[i%4], envLabelNames[j], "ms", labelPos);
            int envNum = i < 3 ? (i+1) % 3 : i;         //Fixing issue caused by how parameters were named so that the amp env is 0
            sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getEnvolopeParamName(envNum, j), *uiSliders[uiSliders.size()-1]));  //Attaching slider to appopriate parameter
        }
    }
    
    //Adding singular lfo and it's sliders and attaching them to approriate parameters
    for(int i =0; i < 2; i++)
    {
        std::string suffix = i  == 1 ? "Hz" : "";
        addSlider(uiSliders, rotaryDesign[3], lfoLabels[i], suffix);
        sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getLfoParamName(0, i), *uiSliders[uiSliders.size()-1]));
    }
    
    //Adding the filter sliders and attaching them to appropriate parameters
    for(int i = 0; i < numFilters; ++i)
    {
        auto* label = boldUiLabels.add(new Label("", filterNames[i]));
        addAndMakeVisible(label);
        for(int j = 0; j < 2; ++j)
        {
            std::string suffix = j  == 1 ? "Hz" : "";
            addSlider(uiSliders, rotaryDesign[i], filterLabels[j], suffix);
            sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getFilterParamName(i, j), *uiSliders[uiSliders.size()-1]));
        }
    }
    
    //Adding master volume slider and attaching it to that parameter
    addSlider(uiSliders, rotaryDesign[0], "Master Volume", "",false, true);
    sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "masterGain", *uiSliders[uiSliders.size()-1]));
    
    //Intialising the title labels
    for(int i = 0; i < 7; ++i)
    {
        auto* label = titleLabels.add(new Label("", nameLabels[i]));
        addAndMakeVisible(label);
    }
    
    //Intialising the comboboxes for the oscillators and connecting it to appropriate parameters
    for(int i = 0; i < 4; ++i)
    {
        addComboBox(comboBoxes, comboBoxFill, 6, "Source " + std::to_string(i+1) +":      ");
        comboAttachment.add(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.parameters, processor.paramID.getOscParamName(i, 0), *comboBoxes[comboBoxes.size()-1]));
    }
    
    //Adding comboboxes for the parameter envolopes and connecting it to appropriate parameters
    for(int i =0; i < numEnvs - 3; ++i)
    {
        addComboBox(comboBoxes, comboBoxFillcustEnv, 13, "Parameter "+ std::to_string(i+1) +" Env:  ",std::to_string(i));
        comboAttachment.add(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.parameters, processor.paramID.getEnvolopeParamName(i+3, 4), *comboBoxes[comboBoxes.size()-1]));
    
        comboBoxes[comboBoxes.size()-1] -> addListener(this);   //Adding a listener to these combo boxes as they need to change slider attachments
    }
    
    //Adding sliders for param envs max value sliders and attaching them to parameters
    for(int i =0; i < numEnvs - 3; ++i)
    {
        addSlider(uiSliders, rotaryDesign[(i+3)%4], "Max Val");
        sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getMaxParamName(i), *uiSliders[uiSliders.size()-1]));
        storeSlider = uiSliders.size()-1;   //Storing position of last add slider
    }
    
    //Setting size of the plugin so the resize() funciton is called
    setSize (1080, 600);
}

SynthTesterAudioProcessorEditor::~SynthTesterAudioProcessorEditor()
{
    //Setting the look and feel parameters to null to stop crashing when plugin window is closed
    setLookAndFeel(nullptr);
    
    for(auto* slider : uiSliders)
        slider -> setLookAndFeel(nullptr);
}

//==============================================================================
void SynthTesterAudioProcessorEditor::paint (Graphics& g)
{
    //Filling all with a background colour
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    //Defining the height and width
    height = getLocalBounds().getHeight();
    width = getLocalBounds().getWidth();
    
    //Drawing all containers
    for(int i = 0; i < 7; ++i)
    {
        drawContainer(width * containerPositions[2 * i], containerPositions[2 * i + 1] * height, containerSizes[2 * i] * width, containerSizes[2 * i + 1] * height, containerColours[i], g);
    }
    
    //Drawing all slider containers
    int sliderContainerNum = 0;
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < sliderContainerSizes[3 * i]; ++j)
        {
            drawContainer(width * sliderContainerPositions[2 * sliderContainerNum], sliderContainerPositions[2 * sliderContainerNum + 1] * height, sliderContainerSizes[3 * i + 1] * width, sliderContainerSizes[3 * i + 2] * height, sliderContainerColours[i], g);
            sliderContainerNum ++;
        }
    }
    
}

void SynthTesterAudioProcessorEditor::drawContainer(float posX, float posY, float sizeX, float sizeY, Colour colour, Graphics& g)
{
    Rectangle<float> rectContainer; //intialising a rectangle
    g.setColour(colour);    //Setting rectangle colour
    g.setOrigin(posX, posY);    //Setting position
    rectContainer.setSize(sizeX, sizeY);    //initialising it's size
    g.fillRect(rectContainer);      //Filling rectangle with set colour
    g.setOrigin(-posX,-posY);       //Resetting orgin position to 0,0 for next placement
}


void SynthTesterAudioProcessorEditor::resized()
{
    //Exit if sliders are not yet defined
    if (uiSliders.isEmpty())
        return;
    
    //Getting height and width of the container
    width = getLocalBounds().getWidth();
    height = getLocalBounds().getHeight();
    
    //-----Setting Up Fonts----//
    //Setting Font Heights
    setFontHeight(textFont, sliderContainerSizes[2]/9.0f, true);
    setFontHeight(componentFont, sliderContainerSizes[2]/11.0f, false);
    setFontHeight(titleFont, sliderContainerSizes[2]/8.0f, true);
    
    //Attaching fonts to labels
    setLabelFonts(uiLabels, componentFont);
    setLabelFonts(boldUiLabels, textFont);
    setLabelFonts(titleLabels, titleFont);
    
    //Setting positon of the container titles
    for(int i = 0; i< 7; ++i)
    {
        titleLabels[i] -> setBounds(containerPositions[2*i] * width, containerPositions[2*i+1] * height, containerSizes[2*i] * width, 0.05 * height);
    }

    //Setting positons of the other UI titles
    for(int i = 0; i < 2; ++i)
    {
        boldUiLabels[i] -> setBounds(sliderContainerPositions[(i+9)*2] * width, sliderContainerPositions[(i+9)*2 + 1]*height - height *0.05, sliderContainerSizes[19] * width, 0.05 * height);
    }
    
    //----Positioning up comboBoxes----//
    for(int i = 0; i < 4; ++i)  //Oscillator comboboxes
    {
        setComboPosition(comboBoxes, i, sliderContainerPositions[2 * i], sliderContainerPositions[2 * i + 1], sliderContainerSizes[1], sliderContainerSizes[2], 3, 3, 1, ((int)i/2) * 2, 1.95, 0.5);
    }
    
    for(int i = 0; i < numEnvs - 3; ++i)    //Param env comboboxes
    {
        setComboPosition(comboBoxes, i+4, sliderContainerPositions[2 * i + 24], sliderContainerPositions[2 * i + 25], sliderContainerSizes[25], sliderContainerSizes[26], 7, 1, 0, 0, 1.95, 0.7);
    }
    
    //----Positioning Sliders ---//
    int workingSliderNum = 0;
    for(int i = 0; i < 22; ++i)
    {
        //Getting array positons for the slider from the silder arrange array
        int arrangePos = i * 5;
        int sliderLayoutRef = sliderArrangeInfo[arrangePos + 2] * 4;
        int sliderPosRef = sliderArrangeInfo[arrangePos] * 2;
        int sliderSizeRef = sliderArrangeInfo[arrangePos + 1] *3;
        int sliderOffsetRef = sliderArrangeInfo[arrangePos + 3] * 2;
        
        //Positoning sliders
        setSliderPositions(uiSliders, workingSliderNum, sliderLayout[sliderLayoutRef], sliderContainerPositions[sliderPosRef], sliderContainerPositions[sliderPosRef + 1], sliderContainerSizes[sliderSizeRef + 1], sliderContainerSizes[sliderSizeRef + 2], sliderLayout[sliderLayoutRef + 1], sliderLayout[sliderLayoutRef + 2], sliderLayout[sliderLayoutRef + 3], sliderOffsets[sliderOffsetRef], sliderOffsets[sliderOffsetRef + 1], sliderArrangeInfo[arrangePos + 4] == 1 ? true : false, true);
        
        //Updating working number of sliders
        workingSliderNum = workingSliderNum + sliderLayout[sliderLayoutRef];
    }
}


void SynthTesterAudioProcessorEditor::addSlider(OwnedArray<Slider> &sliderArray, LookAndFeel *const newLookAndFeel, std::string labelName, std::string suffix ,bool labelOnLeft, bool noLabel)
{
    auto* slider = sliderArray.add(new Slider(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow)); //Adding new slider
    slider -> setRange(0, 5.0); //Setting default range
    slider -> setLookAndFeel(newLookAndFeel);       //Setting the look and feel
    slider -> setTextValueSuffix(suffix);           //Setting the units of the sloder
    addAndMakeVisible(slider);                      //Making it visible
    
    if(!noLabel)                                    //If a label is to be made then make one
    {
        auto* label = uiLabels.add(new Label("", labelName));
        label -> attachToComponent(slider, labelOnLeft);    //Place label in desired position
        addAndMakeVisible(label);
    }
    
}


void SynthTesterAudioProcessorEditor::addComboBox(OwnedArray<ComboBox> &comboArray, std::string *comboFill, int numComboElements, std::string labelName, std::string comboBoxName)
{
    auto* combo = comboBoxes.add(new ComboBox(comboBoxName));   //Adding new comboBox with a defined name
    combo -> setSelectedId(1);                      //Setting default combo ID
    for(int i=0; i < numComboElements; ++i)         //Filling combo with array elements
    {
        combo -> addItem(comboFill[i], i+1);
    }
    addAndMakeVisible(combo);                       //make combo visible

    //Add label to combo box
    auto* label = boldUiLabels.add(new Label("", labelName));
    label -> attachToComponent(combo, true);
    addAndMakeVisible(label);
}

void SynthTesterAudioProcessorEditor::setSliderPositions(OwnedArray<Slider> &sliderArray, int minSliderNum, int numSliders, float containerPosX, float containerPosY, float containerWidth, float containerHeight, int numXDiv, float numYDiv, int numHorizontal, int rowOffset, int columnOffset, bool labelAbove, bool centred, float borderXpecentage, float borderYpercentage)
{
    //Gettting border size
    float xBorder = containerWidth * borderXpecentage * width;
    float yBorder = containerHeight * borderYpercentage * height;
    
    //Getting container size with border taken into account
    float containerX = containerWidth * width - 2.0f * xBorder;
    float containerY = containerHeight * height - 2.0f * yBorder;
    
    //Gettingx and y increments based on the number of divsions specified in each direction
    float xIncrement = (containerWidth * width - 2.0f * xBorder) / (float)numXDiv;
    float yIncrement = (containerHeight * height - 2.0f * yBorder)  / (float)numYDiv;
    
    //Bools that define if sliders may need to be shifted to fit in container box
    bool move2FitX = false;
    bool move2FitY = false;
    
    //Defining slider size
    float sliderSize = sliderSizes[1] * height;
    
    //Parameters to keep track of row and column position to ensure max row length isn't exceeded
    int j = rowOffset;
    int k = columnOffset;
    float labelAllowance = labelAbove ? 0 : sliderSize; //If a label placed to the left then need to add a horizontal label allowance
    
    float xPositioning = labelAllowance;    //Non centered x positon is just the label allowance from the left
    
    float yPositioning = 0;     //Non centered y position is 0
    
    float maxXLength = labelAllowance +  sliderSize + xIncrement * (numXDiv-1); //Calculating the max length of all sliders in row to end of last slider
    
    if(containerX < maxXLength) //If could reach past the length of the container with the label then move over extending sliders into containers
    {
        move2FitX = true;
    }
    if(yIncrement < sliderSize)
    {
        move2FitY = true;
    }
    if(centred)    //If size not too big and meant to be centred then centre the dials in the container
    {
        yPositioning = (yIncrement - sliderSize)/2.0f;  //Positioning the sliders centrally vertically

        xPositioning = move2FitX ? labelAllowance : (containerX - maxXLength)/2.0f + labelAllowance; //Positioning the sliders central horizontally by halving space after final slider
    }
    
    for(int i = minSliderNum; i < (minSliderNum+numSliders); ++i)   //Going through all sliders in this container
    {
        auto* slider = sliderArray[i];
        float xContainerPos =  j * xIncrement + xBorder + xPositioning; //Getting slider container psoitonis
        float yContainerPos =  k * yIncrement + yBorder + yPositioning;
        
        //If exceeded bounds of container move it back within bounds
        if(move2FitX && xContainerPos > (containerX - sliderSize))
                xContainerPos = containerX - sliderSize;
        if(move2FitY && yContainerPos+sliderSize > containerY)
                yContainerPos = containerY - sliderSize;

        //Setting position of slider and adding a textbox
        slider -> setBounds(containerPosX * width + xContainerPos, containerPosY * height + yContainerPos, sliderSize, sliderSize);
        slider -> setTextBoxStyle(Slider::TextBoxBelow, false, sliderSize, 0.35* sliderSize);
        
        //Ensuring not exceeding maximum horizontal positons
        if(j < (numHorizontal + rowOffset - 1))
        {
            j++;
        }
        else
        {
            k++;
            j=rowOffset;
        }
    }
}

void SynthTesterAudioProcessorEditor::setLabelFonts(OwnedArray<Label> &labels, Font labelFont)
{
    for(auto* label : labels)       //iterating through all labels and setting them to the desired font
        label -> setFont(labelFont);
}

void SynthTesterAudioProcessorEditor::setComboPosition(OwnedArray<ComboBox>& newComboBoxes, int comboNum, float containerPosX, float containerPosY, float containerWidth, float containerHeight, int numXDiv, int numYDiv, int xDivPos, int yDivPos, float xFillPercentage, float yFillPercentage)
{
    //Getting x and y increments based on the set x and y divisions
    float xIncrement = containerWidth * width  / (float)numXDiv;
    float yIncrement = containerHeight * height / (float)numYDiv;
    
    //Getting the size of the combobox
    float comboSizeX = xFillPercentage * xIncrement;
    float comboSizeY = yFillPercentage * yIncrement;
    
    //Getting the number of divisions the comboBox covers at that size
    int numCoveredXDivs = ceil(xFillPercentage);
    int numCoveredYDivs = ceil(yFillPercentage);
    
    //Getting position of combobox such that it is centred in the divisions it covers
    float xPos = containerPosX * width + xIncrement * (xDivPos + (numCoveredXDivs/2.0f)) - 0.5 * comboSizeX;
    float yPos = containerPosY  * height+ yIncrement * (yDivPos + (numCoveredYDivs/2.0f)) - 0.5 * comboSizeY;
    
    //Placing the comboBox
    newComboBoxes[comboNum] -> setBounds(xPos, yPos, comboSizeX, comboSizeY);
}


void SynthTesterAudioProcessorEditor::setFontHeight(Font& thisFont, float fontHeight, bool bold)
{
    thisFont.setHeight(height * fontHeight);  //Setting the height of the font
    thisFont.setBold(bold); //Setting font bold status
}


void SynthTesterAudioProcessorEditor::comboBoxChanged (ComboBox *comboBoxThatHasChanged)
{
    int currentVal = comboBoxThatHasChanged -> getSelectedId();     //Getting selected value in combobox
        
    int comboNum = (comboBoxThatHasChanged -> Component::getName()).getIntValue();  //Getting combobox name
    
    int sliderNum = storeSlider - 4 +comboNum; //Getting which slider number we are referencing
    
    if(currentVal != 1) //Removing the slider attachment for this slider and updating it with new parameter to attach to
    {
        sliderAttachment.remove(sliderNum);
        sliderAttachment.insert(sliderNum, new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getMaxParamName(currentVal-2), *uiSliders[sliderNum]));
    }
}
