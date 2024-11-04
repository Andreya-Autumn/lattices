/*
  Lattices - A Just-Intonation graphical MTS-ESP Source
  
  Copyright 2023-2024 Andreya Ek Frisk and Paul Walker.
  
  This code is released under the MIT licence, but do note that it depends
  on the JUCE library, see licence for more details.
  
  Source available at https://github.com/Andreya-Autumn/lattices
*/

#pragma once

//==============================================================================
struct ModeComponent : public juce::ToggleButton
{
    ModeComponent(int m)
    {
        addAndMakeVisible(pythButton);
        pythButton.onClick = [this]{ updateToggleState(); };
        pythButton.setClickingTogglesState(true);
        pythButton.setRadioGroupId(1);

        addAndMakeVisible(syntonicButton);
        syntonicButton.onClick = [this]{ updateToggleState(); };
        syntonicButton.setClickingTogglesState(true);
        syntonicButton.setRadioGroupId(1);
        
        addAndMakeVisible(duodeneButton);
        duodeneButton.onClick = [this]{ updateToggleState(); };
        duodeneButton.setClickingTogglesState(true);
        duodeneButton.setRadioGroupId(1);
        
        switch (m)
        {
            case LatticesProcessor::Pyth:
                pythButton.setToggleState(true, juce::dontSendNotification);
                break;
            case LatticesProcessor::Syntonic:
                syntonicButton.setToggleState(true, juce::dontSendNotification);
                break;
            case LatticesProcessor::Duodene:
                duodeneButton.setToggleState(true, juce::dontSendNotification);
                break;
            default:
                duodeneButton.setToggleState(true, juce::dontSendNotification);
                break;
        }
    }
    
    void resized() override
    {
        pythButton.setBounds(5,5,110,35);
        syntonicButton.setBounds(5,45,110,35);
        duodeneButton.setBounds(5,85,110,35);
    }
    
    void updateToggleState()
    {
        modeChanged = true;
    }
    
    void paint(juce::Graphics &g) override
    {
        g.setColour(bg);
        g.fillRect(this->getLocalBounds());
        
//        g.setColour(juce::Colours::thistle);
//        g.drawRect(this->getLocalBounds());
    }
    
    int whichMode()
    {
        if (duodeneButton.getToggleState() == true)
        {
            return 2;
        }
        if (syntonicButton.getToggleState() == true)
        {
            return 1;
        }
        if (pythButton.getToggleState() == true)
        {
            return 0;
        }
        std::cout << "something went wrong" << std::endl;
        return 0;
    }
    
    bool modeChanged = false;
    
private:
    juce::Colour bg = findColour(juce::TextEditor::backgroundColourId);
    
    juce::TextButton pythButton { "Pythagorean" };
    juce::TextButton syntonicButton { "Syntonic" };
    juce::TextButton duodeneButton { "Duodene" };
};


