/*
  Lattices - A Just-Intonation graphical MTS-ESP Source
  
  Copyright 2023-2024 Andreya Ek Frisk and Paul Walker.
  
  This code is released under the MIT licence, but do note that it depends
  on the JUCE library, see licence for more details.
  
  Source available at https://github.com/Andreya-Autumn/lattices
*/

#include "LatticesProcessor.h"
#include "LatticesEditor.h"

//==============================================================================
LatticesEditor::LatticesEditor(LatticesProcessor &p)
    : juce::AudioProcessorEditor(&p), processor(p)
{
    
    latticeComponent = std::make_unique<LatticeComponent>(p.coOrds);
    addAndMakeVisible(*latticeComponent);
    

    
    
    midiButton = std::make_unique<juce::TextButton>("MIDI Settings");
    addAndMakeVisible(*midiButton);
    midiButton->onClick = [this]{ showMidiMenu(); };
    midiButton->setClickingTogglesState(true);
    midiButton->setToggleState(false, juce::dontSendNotification);
    
    midiComponent = std::make_unique<MIDIMenuComponent>(p.shiftCCs[0],
                                                        p.shiftCCs[1],
                                                        p.shiftCCs[2],
                                                        p.shiftCCs[3],
                                                        p.shiftCCs[4],
                                                        p.listenOnChannel);
    addAndMakeVisible(*midiComponent);
    midiComponent->setVisible(false);
    
    tuningButton = std::make_unique<juce::TextButton>("Tuning Settings");
    addAndMakeVisible(*tuningButton);
    tuningButton->onClick = [this]{ showTuningMenu(); };
    tuningButton->setClickingTogglesState(true);
    tuningButton->setToggleState(false, juce::dontSendNotification);
    
    modeComponent = std::make_unique<ModeComponent>(p.mode);
    addAndMakeVisible(*modeComponent);
    modeComponent->setVisible(false);
    
    originComponent = std::make_unique<OriginComponent>(p.originalRefNote,
                                                        p.originalRefFreq);
    addAndMakeVisible(*originComponent);
    originComponent->setVisible(false);
    
    startTimer(5);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(width, height);
    setResizable(true, true);
}

LatticesEditor::~LatticesEditor() {}

//==============================================================================
void LatticesEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
}

void LatticesEditor::idle() {}

void LatticesEditor::resized()
{
    auto b = this->getLocalBounds();
    
    latticeComponent->setBounds(b);
    
    midiButton->setBounds(10, b.getBottom() - 40, 120, 30);
    midiComponent->setBounds(10, b.getBottom() - 155 - 30 - 10, 120, 155);
    
    tuningButton->setBounds(b.getRight() - 216 - 10, b.getBottom() - 40, 216, 30);
    modeComponent->setBounds(b.getRight() - 216 - 10, b.getBottom() - 180 - 40, 216, 90);
    originComponent->setBounds(b.getRight() - 216 - 10, b.getBottom() - 95 - 40, 216, 95);
}



void LatticesEditor::showMidiMenu()
{
    bool show = midiButton->getToggleState();
    midiComponent->setVisible(show);
    
    if (show)
    {
        midiButton->setConnectedEdges(4);
    }
    else
    {
        midiButton->setConnectedEdges(!4);
    }
}

void LatticesEditor::showTuningMenu()
{
    bool show = tuningButton->getToggleState();
    originComponent->setVisible(show);
    modeComponent->setVisible(show);
    
    if (show)
    {
        tuningButton->setConnectedEdges(4);
    }
    else
    {
        tuningButton->setConnectedEdges(!4);
    }
}

void LatticesEditor::timerCallback()
{
    if (processor.changed)
    {
        latticeComponent->update(processor.coOrds);
        latticeComponent->repaint();
        processor.changed = false;
    }
    
    if (modeComponent->modeChanged)
    {
        processor.modeSwitch(modeComponent->whichMode());
        modeComponent->modeChanged = false;
    }
    
    if (midiComponent->settingChanged)
    {
        processor.updateMIDI(midiComponent->data[0],
                             midiComponent->data[1],
                             midiComponent->data[2],
                             midiComponent->data[3],
                             midiComponent->data[4],
                             midiComponent->midiChannel);
    }
    
    if (originComponent->freqChanged)
    {
        processor.updateFreq(originComponent->whatFreq);
        originComponent->freqChanged = false;
    }
    
    if (originComponent->rootChanged)
    {
        int r = originComponent->whichNote();
        originComponent->resetFreqOnRootChange(processor.updateRoot(r));
    }
}
