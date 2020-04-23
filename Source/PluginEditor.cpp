/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthTesterAudioProcessorEditor::SynthTesterAudioProcessorEditor (SynthTesterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    for(int i = 0; i < 4; ++i)
    {
        rotaryDesign.add(new NewRotaryDesign());
        rotaryDesign[i] -> setColour(sliderColours[i]);
    }
    
    for(int i = 0; i < numOscs; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            addSlider(uiSliders, rotaryDesign[i], oscLabelNames[j]);
            sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getOscParamName(i, j+1), *uiSliders[uiSliders.size()-1]));
        }
    }
    
    for(int i = 0; i < numEnvs; ++i)
    {
        bool labelPos = i == 1 ? false : true;
        for(int j = 0; j < 4; ++j)
        {
            addSlider(uiSliders, rotaryDesign[i%4], envLabelNames[j], labelPos);
            sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getEnvolopeParamName(i, j), *uiSliders[uiSliders.size()-1]));
        }
    }
    
    //Adding singular lfo
    for(int i =0; i < 2; i++)
    {
        addSlider(uiSliders, rotaryDesign[3], lfoLabels[i]);
        sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getLfoParamName(0, i), *uiSliders[uiSliders.size()-1]));
    }
    
    
    for(int i = 0; i < numFilters; ++i)
    {
        auto* label = boldUiLabels.add(new Label("", filterNames[i]));
        addAndMakeVisible(label);
        for(int j = 0; j < 2; ++j)
        {
            addSlider(uiSliders, rotaryDesign[i], filterLabels[j]);
            sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getFilterParamName(i, j), *uiSliders[uiSliders.size()-1]));
        }
    }

    addSlider(uiSliders, rotaryDesign[0], "Master Volume", false, true);
    sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "masterGain", *uiSliders[uiSliders.size()-1]));
    
    for(int i = 0; i < 7; ++i)
    {
        auto* label = titleLabels.add(new Label("", nameLabels[i]));
        addAndMakeVisible(label);
    }
    
    
    for(int i = 0; i < 4; ++i)
    {
        addComboBox(comboBoxes, comboBoxFill, 6, "Source " + std::to_string(i+1) +":      ");
        comboAttachment.add(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.parameters, processor.paramID.getOscParamName(i, 0), *comboBoxes[comboBoxes.size()-1]));
    }
    
    for(int i =0; i < numEnvs - 3; ++i)
    {
        addComboBox(comboBoxes, comboBoxFillcustEnv, 13, "Parameter "+ std::to_string(i+1) +" Env:  ",std::to_string(i));
        comboAttachment.add(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.parameters, processor.paramID.getEnvolopeParamName(i+3, 4), *comboBoxes[comboBoxes.size()-1]));
    
        comboBoxes[comboBoxes.size()-1] -> addListener(this);
    }
    
    for(int i =0; i < numEnvs - 3; ++i)
    {
        addSlider(uiSliders, rotaryDesign[(i+3)%4], "Max Val");
        sliderAttachment.add(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getMaxParamName(i), *uiSliders[uiSliders.size()-1]));
        storeSlider = uiSliders.size()-1;
    }
    
    
    setSize (1080, 600);
    isResizable();
}

SynthTesterAudioProcessorEditor::~SynthTesterAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    
    for(auto* slider : uiSliders)
        slider -> setLookAndFeel(nullptr);
}

//==============================================================================
void SynthTesterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    float height = getLocalBounds().getHeight();
    float width = getLocalBounds().getWidth();
    
    for(int i = 0; i < 7; ++i)
    {
        drawContainer(width * containerPositions[2 * i], containerPositions[2 * i + 1] * height, containerSizes[2 * i] * width, containerSizes[2 * i + 1] * height, containerColours[i], g);
    }
    
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
    Rectangle<float> rectContainer;
    g.setColour(colour);
    g.setOrigin(posX, posY);
    rectContainer.setSize(sizeX, sizeY);
    g.fillRect(rectContainer);
    g.setOrigin(-posX,-posY);
}


void SynthTesterAudioProcessorEditor::resized()
{
    if (uiSliders.isEmpty())
        return;
    
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
    
    auto area = getLocalBounds();
    for(int i = 0; i< 7; ++i)
    {
        titleLabels[i] -> setBounds(containerPositions[2*i] * area.getWidth(), containerPositions[2*i+1] * area.getHeight(), containerSizes[2*i] * area.getWidth(), 0.05 * area.getHeight());
    }

    for(int i = 0; i < 2; ++i)
    {
        boldUiLabels[i] -> setBounds(sliderContainerPositions[(i+9)*2] * area.getWidth(), sliderContainerPositions[(i+9)*2 + 1]*area.getHeight()-area.getHeight()*0.05, sliderContainerSizes[19]*area.getWidth(), 0.05 * area.getHeight());
    }
    
    //----Positioning up comboBoxes----//
    for(int i = 0; i < 4; ++i)
    {
        setComboPosition(comboBoxes, i, sliderContainerPositions[2 * i], sliderContainerPositions[2 * i + 1], sliderContainerSizes[1], sliderContainerSizes[2], 3, 3, 1, ((int)i/2) * 2, 1.95, 0.5);
    }
    
    for(int i = 0; i < numEnvs - 3; ++i)
    {
        setComboPosition(comboBoxes, i+4, sliderContainerPositions[2 * i + 24], sliderContainerPositions[2 * i + 25], sliderContainerSizes[25], sliderContainerSizes[26], 7, 1, 0, 0, 1.95, 0.7);
    }
    
    int workingSliderNum = 0;
    for(int i = 0; i < 22; ++i)
    {
        int arrangePos = i * 5;
        int sliderLayoutRef = sliderArrangeInfo[arrangePos + 2] * 4;
        int sliderPosRef = sliderArrangeInfo[arrangePos] * 2;
        int sliderSizeRef = sliderArrangeInfo[arrangePos + 1] *3;
        int sliderOffsetRef = sliderArrangeInfo[arrangePos + 3] * 2;
        
        setSliderPositions(uiSliders, workingSliderNum, sliderLayout[sliderLayoutRef], sliderContainerPositions[sliderPosRef], sliderContainerPositions[sliderPosRef + 1], sliderContainerSizes[sliderSizeRef + 1], sliderContainerSizes[sliderSizeRef + 2], sliderLayout[sliderLayoutRef + 1], sliderLayout[sliderLayoutRef + 2], sliderLayout[sliderLayoutRef + 3], sliderOffsets[sliderOffsetRef], sliderOffsets[sliderOffsetRef + 1], sliderArrangeInfo[arrangePos + 4] == 1 ? true : false, true);
        
        
        workingSliderNum = workingSliderNum + sliderLayout[sliderLayoutRef];
    }
    
    
    //setSliderPositions(uiSliders, uiSliders.size()-1, 1, 0.8, 0.8, 0.2, 0.2, 1, 1, 1, 0, 0);
    
}


void SynthTesterAudioProcessorEditor::addSlider(OwnedArray<Slider> &sliderArray, LookAndFeel *const newLookAndFeel, std::string labelName, bool labelOnLeft, bool noLabel)
{
    auto* slider = sliderArray.add(new Slider(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow));
    slider -> setRange(0, 5.0);
    slider -> setLookAndFeel(newLookAndFeel);
    addAndMakeVisible(slider);
    if(!noLabel)
    {
        auto* label = uiLabels.add(new Label("", labelName));
        label -> attachToComponent(slider, labelOnLeft);
        addAndMakeVisible(label);
    }
    
}


void SynthTesterAudioProcessorEditor::addComboBox(OwnedArray<ComboBox> &comboArray, std::string *comboFill, int numComboElements, std::string labelName, std::string comboBoxName)
{
    auto* combo = comboBoxes.add(new ComboBox(comboBoxName));
    combo -> setSelectedId(1);
    for(int i=0; i < numComboElements; ++i)
    {
        combo -> addItem(comboFill[i], i+1);
    }
    addAndMakeVisible(combo);

    auto* label = boldUiLabels.add(new Label("", labelName));
    label -> attachToComponent(combo, true);
    addAndMakeVisible(label);
}

void SynthTesterAudioProcessorEditor::setSliderPositions(OwnedArray<Slider> &sliderArray, int minSliderNum, int numSliders, float containerPosX, float containerPosY, float containerWidth, float containerHeight, int numXDiv, float numYDiv, int numHorizontal, int rowOffset, int columnOffset, bool labelAbove, bool centred, float borderXpecentage, float borderYpercentage)
{   //Might need a few additional parameters like slider offset(could be included in x pos though tbf)
    //Can remove right positioning and bottom positionig and border stuff!!!
    
    //float width = getLocalBounds().getWidth();
    //float height = getLocalBounds().getHeight();
    
    float xBorder = containerWidth * borderXpecentage * width;
    float yBorder = containerHeight * borderYpercentage * height;
    
    std::cout<< "here for: "<<minSliderNum<<std::endl;
    
    float containerX = containerWidth * width - 2.0f * xBorder;
    float containerY = containerHeight * height - 2.0f * yBorder;
    
    float xIncrement = (containerWidth * width - 2.0f * xBorder) / (float)numXDiv;
    float yIncrement = (containerHeight * height - 2.0f * yBorder)  / (float)numYDiv;
    
    bool move2FitX = false;
    bool move2FitY = false;
    
    float sliderSize = sliderSizes[1] * height;
    
    int j = rowOffset;
    int k = columnOffset;
    float labelAllowance = labelAbove ? 0 : sliderSize;
    
    float xPositioning = labelAllowance;
    
    float yPositioning = 0;
    
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
        float xContainerPos =  j * xIncrement + xBorder + xPositioning;
        float yContainerPos =  k * yIncrement + yBorder + yPositioning;
        if(move2FitX && xContainerPos > (containerX - sliderSize))
                xContainerPos = containerX - sliderSize;
        if(move2FitY && yContainerPos+sliderSize > containerY)
                yContainerPos = containerY - sliderSize;

        slider -> setBounds(containerPosX * width + xContainerPos, containerPosY * height + yContainerPos, sliderSize, sliderSize);
        slider -> setTextBoxStyle(Slider::TextBoxBelow, false, sliderSize, 0.3* sliderSize);
        
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
    
    std::cout<< "finished for: "<<minSliderNum<<std::endl;
}

void SynthTesterAudioProcessorEditor::setLabelFonts(OwnedArray<Label> &labels, Font labelFont)
{
    for(auto* label : labels)
        label -> setFont(labelFont);
}

void SynthTesterAudioProcessorEditor::setComboPosition(OwnedArray<ComboBox>& newComboBoxes, int comboNum, float containerPosX, float containerPosY, float containerWidth, float containerHeight, int numXDiv, int numYDiv, int xDivPos, int yDivPos, float xFillPercentage, float yFillPercentage)
{
    float width = getLocalBounds().getWidth();
    float height = getLocalBounds().getHeight();
    
    float xIncrement = containerWidth * width  / (float)numXDiv;
    float yIncrement = containerHeight * height / (float)numYDiv;
    
    float comboSizeX = xFillPercentage * xIncrement;
    float comboSizeY = yFillPercentage * yIncrement;
    
    int numCoveredXDivs = ceil(xFillPercentage);
    int numCoveredYDivs = ceil(yFillPercentage);
    
    float xPos = containerPosX * width + xIncrement * (xDivPos + (numCoveredXDivs/2.0f)) - 0.5 * comboSizeX;
    float yPos = containerPosY  * height+ yIncrement * (yDivPos + (numCoveredYDivs/2.0f)) - 0.5 * comboSizeY;
    
    
    newComboBoxes[comboNum] -> setBounds(xPos, yPos, comboSizeX, comboSizeY);
}


void SynthTesterAudioProcessorEditor::setFontHeight(Font& thisFont, float height, bool bold)
{
    thisFont.setHeight(getLocalBounds().getHeight() * height);
    thisFont.setBold(bold);
}


void SynthTesterAudioProcessorEditor::comboBoxChanged (ComboBox *comboBoxThatHasChanged)
{
    int currentVal = comboBoxThatHasChanged -> getSelectedId();
    
    int comboNum = (comboBoxThatHasChanged -> Component::getName()).getIntValue();
    
std::cout<<comboNum<<std::endl;
    
    int sliderNum = uiSliders.size() - 5 +comboNum;
    
    if(currentVal != 1)
    {
        sliderAttachment.remove(sliderNum);
        sliderAttachment.insert(sliderNum, new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, processor.paramID.getMaxParamName(currentVal-2), *uiSliders[sliderNum]));
    }
}
