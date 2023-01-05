#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"


// process button inputs from the control surface
void LibMain::ProcessButton(uint8_t button, uint8_t value)  // processes a midi button press
{
    uint8_t x, songnumber;
    std::string labelwidget;

    if (value == 127) {  // only process button downs
        if (button == Surface.CommandButtons[FADERS_BANK_UP])  // next Fader bank
        {
            if (Surface.TextDisplay == SHOW_KNOBS)
            {
                Surface.TextDisplay = SHOW_FADERS;
                DisplayModeButtons();
            }
            if (Surface.Row[FADER_ROW].IncrementBank())
            {
                SyncBankIDs(FADER_ROW);
            }
            // DisplayFaders(Surface.Row[FADER_ROW]);
        }
        else if (button == Surface.CommandButtons[FADERS_BANK_DOWN])  // prior Fader bank
        {
            if (Surface.TextDisplay == SHOW_KNOBS)
            {
                Surface.TextDisplay = SHOW_FADERS;
                DisplayModeButtons();
            }
            if (Surface.Row[FADER_ROW].DecrementBank())
            {
                SyncBankIDs(FADER_ROW);
            }
        }
        else if (button == Surface.CommandButtons[KNOBS_BANK_UP])  // next Knob bank
        {
            if (Surface.TextDisplay == SHOW_FADERS)
            {
                Surface.TextDisplay = SHOW_KNOBS;
                DisplayModeButtons();
            }
            if (Surface.Row[KNOB_ROW].IncrementBank())
            {
                SyncBankIDs(KNOB_ROW);
            }
        }
        else if (button == Surface.CommandButtons[KNOBS_BANK_DOWN])  // prior Knob bank
        {
            if (Surface.TextDisplay == SHOW_FADERS)
            {
                Surface.TextDisplay = SHOW_KNOBS;
                DisplayModeButtons();
            }
            if (Surface.Row[KNOB_ROW].DecrementBank())
            {
                SyncBankIDs(KNOB_ROW);
            }
        }
        else if (button == Surface.CommandButtons[SONGS_BANK_UP])   // next song/rack bank
        {
            if (inSetlistMode() == 1) { Surface.FirstShownSong += 8; }
            else { Surface.FirstShownRack += 8; }
            if (Surface.RackRow < Surface.ButtonRows) { DisplayRow(Surface.Row[Surface.RackRow], false); }
        }
        else if (button == Surface.CommandButtons[SONGS_BANK_DOWN])  // prior Song bank
        {
            if (inSetlistMode() == 1) { Surface.FirstShownSong -= 8; }
            else { Surface.FirstShownRack -= 8; }
            if (Surface.RackRow < Surface.ButtonRows) { DisplayRow(Surface.Row[Surface.RackRow], false); }
        }
        else if (button == Surface.CommandButtons[SETLIST_TOGGLE])  // Toggle between in and out of Setlist mode
        {
            Surface.reportWidgetChanges = false;
            inSetlistMode() ? switchToPanelView() : switchToSetlistView();
            Surface.reportWidgetChanges = true;
        }
        else if (button == Surface.CommandButtons[FADERS_SELECT]) // show faders
        {
            if (Surface.TextDisplay == SHOW_SONGS) { DisplayText(0, 0, "", 28); }  // clear upper left area if we're coming out of Songs/Racks display
            Surface.TextDisplay = SHOW_FADERS;
            DisplayModeButtons();
            DisplayFaders(Surface.Row[FADER_ROW]);
        }
        else if (button == Surface.CommandButtons[KNOBS_SELECT]) // show knobs
        {
            if (Surface.TextDisplay == SHOW_SONGS) { DisplayText(0, 0, "", 28); }  // clear upper left area if we're coming out of Songs/Racks display
            Surface.TextDisplay = SHOW_KNOBS;
            DisplayModeButtons();
            DisplayFaders(Surface.Row[KNOB_ROW]);
        }
        else if (button == Surface.CommandButtons[SONGSRACKS_SELECT]) // show songlist
        {
            Surface.TextDisplay = SHOW_SONGS;
            DisplayModeButtons();
            if (Surface.RackRow < Surface.ButtonRows) { DisplayRow(Surface.Row[Surface.RackRow], false); }
        }
        else if (button == SID_TRANSPORT_PLAY) // 
        {
            setPlayheadState(true);
        }
        else if (button == SID_TRANSPORT_STOP) // 
        {
            setPlayheadState(false);
        }
        else for (x = 0; x < Surface.ButtonRows ; x++)
        {
            if ((button >= Surface.Row[x].FirstID) && (button < Surface.Row[x].FirstID + Surface.Row[x].Columns) )
            {
                if (Surface.Row[x].Showing == SHOW_SONGS) // if the Row is in song select mode, process it as a song select
                {
                    songnumber = button - Surface.Row[x].FirstID + Surface.FirstShownSong;
                    if (songnumber < getSongCount()) 
                    { 
                        Surface.reportWidgetChanges = false;
                        switchToSong(songnumber, 0); 
                        Surface.reportWidgetChanges = true;
                    }
                }
                else if (Surface.Row[x].Showing == SHOW_SONGPARTS)
                {
                    songnumber = button - Surface.Row[x].FirstID;
                    if (songnumber <= getSongpartCount(getCurrentSongIndex())) 
                    { 
                        Surface.reportWidgetChanges = false;
                        switchToSongPart(songnumber);
                        Surface.reportWidgetChanges = true;
                    }
                }
                else if (Surface.Row[x].Showing == SHOW_RACKSPACES) // if the Row is in Rackspace mode, process it as a Rackspace select
                {
                    songnumber = button - Surface.Row[x].FirstID + Surface.FirstShownRack;
                    if (songnumber < getRackspaceCount()) 
                    { 
                        // switchToRackspace(songnumber, 0);
                        Surface.reportWidgetChanges = false;
                        switchToRackspaceName(getRackspaceName(songnumber), "");
                        Surface.reportWidgetChanges = true;
                    }
                }
                else if (Surface.Row[x].Showing == SHOW_VARIATIONS)
                {
                    songnumber = button - Surface.Row[x].FirstID;
                    if (songnumber < getVariationCount(getCurrentRackspaceIndex()))
                    { 
                        Surface.reportWidgetChanges = false;
                        switchToRackspace(getCurrentRackspaceIndex(), songnumber);
                        DisplayRow(Surface.Row[x], true);
                        Surface.reportWidgetChanges = true;
                    }
                }
                else if (Surface.Row[x].BankValid()) // make sure ActiveBank is a valid bank to avoid exceptions
                {
                    ToggleButton(Surface.Row[x], button - Surface.Row[x].FirstID);
                    // ShowButton(Surface.Row[x], button - Surface.Row[x].FirstID);
                }
            }
        }
    }
}


// button is position within the active bank on the given Row
void LibMain::ToggleButton(SurfaceRow Row, uint8_t button)
{
    std::string widgetname;

    widgetname = Row.WidgetPrefix + "_" + Row.BankIDs[Row.ActiveBank] + "_" + std::to_string(button);
    setWidgetValue(widgetname, (getWidgetValue(widgetname) == 0.0) ? (double)1.0 : (double)0.0);
}


void LibMain::ProcessKnob(uint8_t column, uint8_t value)  // processes a midi message for a knob turn (0-7)
{
    std::string widgetname, caption;
    int resolution = 200;
    double newValue = 0;

    if (Surface.Row[KNOB_ROW].BankValid()) {

        widgetname = KNOB_PREFIX + (std::string)"_" + Surface.Row[KNOB_ROW].BankIDs[Surface.Row[KNOB_ROW].ActiveBank] + "_" + std::to_string(column);
        if (widgetExists(widgetname) == true)  // if the widget doesn't exist we ignore the input
        {
            if (column < 8) // if it's < 8 it's a real knob
            {
                if (widgetExists(widgetname + "_p"))  // if there's a mc_k_1_5_p type caption, process first field as resolution (integer)
                {
                    caption = getWidgetCaption(widgetname + "_p");
                    std::vector< std::string> name_segments = ParseWidgetName(caption, '_');
                    (name_segments.size() >= 1) ? resolution = (int)std::stoi("0" + name_segments[0]) : resolution = 200;  // default to 200
                }
                newValue = getWidgetValue(widgetname);
                if (value < 0x10) {  // small numbers are turns in the clockwise direction
                    // newValue = newValue + 0.005 * value;
                    newValue = newValue + static_cast<double>(value) / static_cast<double>(resolution);
                    if (newValue > 1) { newValue = 1; }
                }
                else if (value > 0x40) {  // above 0x40 is a counter-clockwise move
                    // newValue = newValue - 0.005 * (value - 0x40);
                    newValue = newValue - static_cast<double>(value - 0x40) / static_cast<double>(resolution);
                    if (newValue < 0) { newValue = 0; }
                }
                setWidgetValue(widgetname, newValue);  // Move the widget, and the OnWidgetChange callback will move the display
            }
            else
            {
                // if it's above column 7 then something weird is going on and we ignore it
            }
        }
        // caption = getWidgetCaption(widgetname);  // right now (GP 4.0.2) this is always same as label
        // Knob[column].SetCaption(caption);
    }
}

void LibMain::ProcessWheel(uint8_t value)  // processes a midi message for jog wheel
{
    std::string widgetname, caption;
    int resolution = 2;

    auto newValue = getBPM();
    if (value < 0x10) {  // small numbers are turns in the clockwise direction
        newValue = newValue + static_cast<double>(value) / static_cast<double>(resolution);
        if (newValue > 640) { newValue = 640; }
    }
    else if (value > 0x40) {  // above 0x40 is a counter-clockwise move
        newValue = newValue - static_cast<double>(value - 0x40) / static_cast<double>(resolution);
        if (newValue < 20) { newValue = 20; }
    }
    setBPM(newValue);
}

// processes a midi message for a fader
void LibMain::ProcessFader(uint8_t channel, uint8_t data1, uint8_t data2)  
{
    std::string widgetname, caption;
    double newValue = 0;

    channel = channel & 0x0F;
//    widgetname = FADER_PREFIX + (std::string) "_" + std::to_string(Surface.ActiveFaderBank) + "_" + std::to_string(channel);
    if (Surface.Row[FADER_ROW].BankValid()) {
        widgetname = FADER_PREFIX + (std::string)"_" + Surface.Row[FADER_ROW].BankIDs[Surface.Row[FADER_ROW].ActiveBank] + "_" + std::to_string(channel);

        // scriptLog(widgetname, 1);
        if (channel < 9) // if it's < 9 it's a real fader
        {
            newValue = ((double)data2 * 128 + (double)data1) / 16383;
            setWidgetValue(widgetname, newValue);  // Move the widget, and the OnWidgetChange callback will move the MCU
            widgetname = FADER_PREFIX + (std::string)"_" + std::to_string(channel);
            if (widgetExists(widgetname)) { setWidgetValue(widgetname, newValue); }
        }
        else
        {
            // if it's above channel 9 then something weird is going on and we ignore it
        }
        // caption = getWidgetCaption(widgetname);  // right now (GP 4.0.2) this is always same as label
        // scriptLog(caption,1);
        // Knob[column].SetCaption(caption);
    }
}


bool LibMain::IsKnob(const uint8_t* data, int length)  // Is midi event from a knob turn?
{
    return(data[0] == 0xB0 && data[1] >= KNOB_0 && data[1] <= KNOB_7);
}

bool LibMain::IsWheel(const uint8_t* data, int length)  // Is midi event from a jow wheel turn?
{
    return(data[0] == 0xB0 && data[1] == JOGWHEEL);
}


bool LibMain::IsFader(const uint8_t* data, int length)  // Is midi event from a fader?
{
    return((data[0] & 0xE0) == 0xE0);
}

bool LibMain::IsButton(const uint8_t* data, int length)   // Is midi event from a button?
{
    return( (data[0] == 0x90) || (data[0] == 0x80) ); // any note on, channel 1
}
