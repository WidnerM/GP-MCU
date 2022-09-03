#pragma once

#include "gigperformer/sdk/GPMidiMessages.h"
#include "gigperformer/sdk/GPUtils.h"
#include "gigperformer/sdk/GigPerformerAPI.h"
#include "gigperformer/sdk/types.h"
#include <cstdio>
#include <iostream>
#include <string>
#include "MCU_Classes.h"
#include "General_Utils.h"


// define an XML string describing your product
const std::string XMLProductDescription =   
     // Replace with your information            
    "<Library>" 
    "<Product Name=\"GP MCU Extension\" Version=\"1.0\" BuildDate=\"6/4/2021\"></Product> "
    "<Description>Control integration for Mackie MCU protocol devices.</Description>"
    "<ImagePath>/Users/Downloads/nothing.jpg</ImagePath>"
    "</Library>";


// External code runs in the LibMain class, inherited from GigPerformerAPI

class LibMain : public gigperformer::sdk::GigPerformerAPI
{

protected:
    int GetPanelCount() override;
    std::string GetPanelName(int index) override;
    std::string GetPanelXML(int index) override;

    // These are for creating menu items in Gig Performer that can be used to trigger external functions provided by the extension developer
    int GetMenuCount() override;
    std::string GetMenuName(int index) override;
    void InvokeMenu(int itemIndex) override;

public:
    // These must be here but no need to do anything unless you want extra behavior
    LibMain(LibraryHandle handle) : GigPerformerAPI(handle)  {}
    virtual ~LibMain() {}

    //  Global declarations and initializations
    //  [Global to the LibMain class, that is]
    SurfaceClass Surface;

    std::vector<std::string> MidiOut = { MCU_OUT };
    std::vector<std::string> MidiIn = { MCU_IN };

    // from LibMain.cpp.  We make these to simplify sending the same midi message to all destinations if we have multiple destinations
    void sendMidiMessage(std::string MidiMessage);
    void sendMidiMessage(gigperformer::sdk::GPMidiMessage MidiMessage);
    void sendMidiMessage(const uint8_t* MidiMessage, int length);
    void SetSurfaceLayout(uint8_t config);


    // from Display.cpp - functions for displaying things on the control surface (fader positions, LCD, button lights, etc.)
    void InitializeMCU();
    void ClearMCUDisplay();
    void CleanMCU();
    void Notify(std::string text);
    void DisplayText(uint8_t column, uint8_t row, std::string text, uint8_t maxlength);
    void DisplayBankInfo(std::string text);
    void DisplayModeButtons();
    void DisplayButton(uint8_t button, uint8_t value);

    void DisplayWidgetValue(SurfaceRow Row, uint8_t column, double Value);
    void DisplayTopLeft(uint8_t column, const std::string caption);
    void DisplayControlLabel(uint8_t column, const std::string caption);


    // from Inputs.cpp
    void ProcessButton(uint8_t button, uint8_t value);
    void ToggleButton(SurfaceRow Row, uint8_t button);


    void ProcessKnob(uint8_t column, uint8_t value);
    void ProcessWheel(uint8_t value);
    void ProcessFader(uint8_t data0, uint8_t data1, uint8_t data2);

    bool IsKnob(const uint8_t* data, int length);
    bool IsWheel(const uint8_t* data, int length);
    bool IsFader(const uint8_t* data, int length);
    bool IsButton(const uint8_t* data, int length);


    // from Songlist.cpp file, functions having to do with song management
    void DisplaySongs(SurfaceRow Row, bool forcetocurrent); // Shows the song list along the bottom bar, allowing song selection with bottom buttons in SONG_MODE
    void DisplaySongParts(SurfaceRow Row, int newIndex);  // Shows the song parts on specified Row
    void DisplayRacks(SurfaceRow Row, bool forcetocurrent);
    void DisplayVariations(SurfaceRow Row, int current);
    void DisplayRow(SurfaceRow Row, bool forcetocurrent);

    // from Knobs.cpp  // void DisplayKnobs();  // Shows the active knob bank (as stored in Surface.ActiveKnobBank)

    // from Faders.cpp
    void DisplayFaders(SurfaceRow Row);  // Shows the active knob bank (as stored in Surface.ActiveKnobBank)

    // from Buttons.cpp
    void LightButtonsForRow(SurfaceRow Row, uint8_t firstbutton, uint8_t number);


       
    // General routines we define before the varoius callbacks
    bool SetMidiInOutDevices() {
        bool foundin = false, foundout = false;
        std::string name;
        std::vector <std::string> validInPorts = {};
        std::vector <std::string> validOutPorts = {};

        for (int i = 0; i < getMidiInDeviceCount(); i++)
        {
            name = getMidiInDeviceName(i);
            for (int j = 0; j < MidiIn.size(); j++) {
                if (name == MidiIn[j]) {
                    listenForMidi(getMidiInDeviceName(i), 1);
                    foundin = true;
                    validInPorts.push_back(name);
                    scriptLog("MCU:  Using midi in " + name, 0);
                }
            }
        }

        for (int i = 0; i < getMidiOutDeviceCount(); i++)
        {
            name = getMidiOutDeviceName(i);
            // scriptLog("Evaluating midi out " + name, 1);
            for (int j = 0; j < MidiOut.size(); j++) {
                if (name == MidiOut[j]) { 
                    foundout = true; 
                    validOutPorts.push_back(name);
                    scriptLog("MCU:  Using midi out " + name, 0);
                }
            }
        }
        MidiOut = validOutPorts;
        // scriptLog(foundout ? EXTENSION_IDENTIFIER + (std::string)" using midi out " + MidiOut : EXTENSION_IDENTIFIER + (std::string)"COULD NOT FIND midi out " + MidiOut, 1);
        return (foundin && foundout);
    }


    // Here's where all the callbacks are.
    // For organization, most of the real routines are split out into other files.
    void OnStatusChanged(GPStatusType status) override
    {
        uint8_t row, column, i;
        std::string widgetname, setting;

        if (status == GPStatus_GigFinishedLoading)
        {
            // scriptLog("MC: Gig loaded.", 1);
            registerCallback("OnSongChanged");
            registerCallback("OnSongPartChanged");
            registerCallback("OnModeChanged");
            registerCallback("OnWidgetValueChanged");
            registerCallback("OnGlobalPlayStateChanged");
            registerCallback("OnRackspaceActivated");
            registerCallback("OnVariationChanged");
            registerCallback("OnMidiIn");
            registerCallback("OnWidgetStateChanged");
            registerCallback("OnTempoChanged");

            // Look for names for Midi in and out, preferably in global rack
            if (widgetExists(MIDI_IN_WIDGETNAME)) { MidiIn = ParseWidgetName(getWidgetCaption(MIDI_IN_WIDGETNAME), ','); }
            if (widgetExists(MIDI_OUT_WIDGETNAME)) { MidiOut = ParseWidgetName(getWidgetCaption(MIDI_OUT_WIDGETNAME), ','); }

            SetMidiInOutDevices();

            InitializeMCU();

            if (widgetExists(LAYOUT_WIDGETNAME)) {
                column = (uint8_t)round(1 / std::max(getWidgetValue(LAYOUT_WIDGETNAME), 1.0 / 127)) - 1;
                SetSurfaceLayout(column);
            }

            OnRackspaceActivated();  // will move faders, light buttons, set display.
            OnModeChanged(inSetlistMode());  // will set indicators and buttons for whether we're in setlist or rackspace mode

            // LightButtonsForRow(Surface.Row[0], 0, 8);
            DisplayModeButtons();
        }
    }

    // On closing GP, clear the MCU display and button LEDs
    void OnClose() override
    {
        int row, column;
        std::string widgetname;

        // scriptLog("MC: Shutting down.", 1);
        InitializeMCU();  // clear buttons, display, and VCUs

        // Clear all the OSC displays so we don't have stray junk left behind or on next Gig load
        for (row = 0; row < std::size(Surface.Row); row++)
        {
            for (column = 0; column < Surface.Row[row].Columns; column++)
            {
                widgetname = Surface.Row[row].WidgetPrefix + "_active_" + std::to_string(column);
                if (widgetExists(widgetname))
                {
                    setWidgetCaption(widgetname, "");
                    if (getWidgetValue(widgetname) == 0) { setWidgetValue(widgetname, 0.1); }
                    setWidgetValue(widgetname, 0);
                }
            }
        }
    }

    // Light or unlight the play state button
    void OnGlobalPlayStateChanged(bool playing) override
    {
        consoleLog(std::string("playing = ") + (playing ? " true" : "false"));
        sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeNoteOnMessage(SID_TRANSPORT_PLAY, (playing == 1) ? BUTTON_LIT : BUTTON_OFF, 0));
    }

    void OnWidgetStateChanged(const std::string& widgetname, int newstate) override
    {
        if (newstate == 0) {
            // scriptLog("Widget state " + widgetname + std::to_string(newstate), 1);
            // add code here to clear controls or zero faders when the widget it was attached to is deleted
        }
    }

    // if the widget caption changes and it's the active bank then update the bank "active" widgets so it's sent out over OSC
    void OnWidgetCaptionChanged(const std::string& widgetName, const std::string& newCaption) override
    {
        uint8_t column = 255;
        uint8_t row;
        double Value;

        std::string widget_prefix, control_type, control_bank, control_number, control_color, setwidget;

        scriptLog("MC: Callback for Caption " + widgetName, 1);
        std::vector< std::string> name_segments = ParseWidgetName(widgetName, '_');

        if (name_segments.size() >= 4)
        {
            widget_prefix = name_segments[0];
            control_type = name_segments[1];
            control_bank = name_segments[2];
            control_number = name_segments[3];
            if (name_segments.size() > 4) {
                control_color = name_segments[4];
            }

            column = std::stoi("0" + control_number);
            if (std::to_string(column) != control_number) { column = 255; } // flag it as 255 if it's an invalid id

            for (row = 0; row < std::size(Surface.Row); row++)  // cycle through each Row of control
            {
                if (control_type == Surface.Row[row].WidgetID && Surface.Row[row].Showing == SHOW_ASSIGNED && column <= Surface.Row[row].Columns && Surface.Row[row].BankValid())
                {
                    //scriptLog("MC: Callback for " + widgetName, 1);
                    if (control_bank.compare(Surface.Row[row].BankIDs[Surface.Row[row].ActiveBank]) == 0)  // if it's the active bank we update the OSC label
                    {
                        Value = getWidgetValue(widgetName);
                        setWidgetCaption(Surface.Row[row].WidgetPrefix + "_active_" + control_number, newCaption);
                        // Gig Performer doesn't send OSC updates for just the caption.  Need to move the value to make it send an OSC update.
                        Value > 0.9 ? setWidgetValue(widgetName, Value - 0.05) : setWidgetValue(widgetName, Value + 0.05);
                        setWidgetValue(widgetName, Value);
                    }
                }
            }
        }
    }

    void OnTempoChanged(double newValue) override
    {
        if (Surface.reportWidgetChanges == true && Surface.TextDisplay != SHOW_SONGS)  // show changes on upper left display if not showing racks/songs
        {
            DisplayTopLeft(0, "BPM: " + std::to_string(newValue));
        }
    }

    void OnWidgetValueChanged(const std::string & widgetName, double newValue) override
    {
        uint8_t column=255;
        uint8_t banknumber, row;

        std::string widget_prefix, control_type, control_bank, control_number, control_color, setwidget;

        // scriptLog("MC: Widget changed: " + widgetName, 1);
        if (Surface.IgnoreWidget != "") {
            scriptLog("MC: Ignoring widget: " + widgetName, 0);
            Surface.IgnoreWidget = "";
        }
        else {

            // widget names are generally structured as [controller id]_[control type]_[bank]_[unit]_[optional additional arguments]
            // typical would be "mc_f_3_1" to indicate MCU controller, fader, bank 3, fader 1.  Banks are typically 1-8, knobs and faders numbered 0-7.
            std::vector< std::string> name_segments = ParseWidgetName(widgetName, '_');

            if (widgetName == LAYOUT_WIDGETNAME) {  // if it's a widget that changes the control layout
                column = (uint8_t) round(1 / std::max(newValue, 1.0 / 127)) - 1;
                if (column != Surface.ButtonLayout)
                {
                    SetSurfaceLayout(column);
                }
            }
            else if (name_segments.size() >= 4)  // our widgets all have at least 4 fields in the name, e.g., mc_f_3_1
            {
                widget_prefix = name_segments[0];
                control_type = name_segments[1];
                control_bank = name_segments[2];
                control_number = name_segments[3];
                if (name_segments.size() > 4) {
                    control_color = name_segments[4];
                }

                column = std::stoi("0" + control_number);
                if (std::to_string(column) != control_number) { column = 255; } // flag it as 255 if it's an invalid id
                // scriptLog("MC: Parsed: " + widget_prefix + " " + control_type + " " + control_bank + " " + control_number + " ", 1);

                if (control_bank != "active" && control_number != "b")  // we ignore the "active" bank, which is for OSC linking, and "b" column number for indicators
                {
                    for (row = 0; row < std::size(Surface.Row); row++)  // cycle through each Row of control
                    {
                        if (control_type == Surface.Row[row].WidgetID && Surface.Row[row].Showing == SHOW_ASSIGNED && column <= Surface.Row[row].Columns && Surface.Row[row].BankIDs.size() > Surface.Row[row].ActiveBank)
                        {
                            //scriptLog("MC: Callback for " + widgetName, 1);
                            if (control_bank.compare(Surface.Row[row].BankIDs[Surface.Row[row].ActiveBank]) == 0)  // if it's the active bank we show it on the control surface
                            {
                                // scriptLog("WCallback Row:" + std::to_string(row) + " ID:" + Surface.Row[row].WidgetID + " Val:" + std::to_string(newValue), 1);
                                setWidgetValue(Surface.Row[row].WidgetPrefix + "_active_" + control_number, newValue);
                                DisplayWidgetValue(Surface.Row[row], column, newValue);
                                if (Surface.reportWidgetChanges == true && Surface.TextDisplay != SHOW_SONGS)  // show changes on upper left display if not showing racks/songs
                                {
                                    DisplayTopLeft(column, Surface.Row[row].RowLabel + std::to_string(column + 1) + ": " + getWidgetTextValue(widgetName));
                                }
                            }
                            else if (control_bank == (std::string)"active") {
                                // scriptLog("MC: Trying to set " + widgetName, 1);
                                // SetButtonColor(Surface.Row[row], column, (newValue != 0) ? BUTTON_LIT : BUTTON_OFF);
                            }
                        }
                    }
                }
            }
        } // IgnoreWidget check

    } 

    // A midi device was added or removed
    void OnMidiDeviceListChanged(std::vector< std::string> & inputs, std::vector< std::string> & outputs) override
    {
        SetMidiInOutDevices();
    }    


    // process data coming from the MCU (or a virtual MCU)
    bool OnMidiIn(const std::string & deviceName, const uint8_t* data, int length) override
    {
        char str[1024];
        char extra[5];
        int x;

        // scriptLog("MC: In " + deviceName, 1);
        if (IsKnob(data, length) ) {
             ProcessKnob(data[1] - KNOB_0, data[2]);  // pass the knob position (0-7) and the value (relative pos is usually 1 or 127)
        }
        else if (IsButton(data, length)) {
             ProcessButton(data[1], data[2]);  // it's a button press
        }
        else if (IsFader(data, length)) {
            ProcessFader(data[0], data[1], data[2]);
        }
        else if (IsWheel(data, length)) {
            ProcessWheel(data[2]);
        }
        else
        {
            sprintf_s(str, "MCU:  Unexpected midi event: ");
            for (x = 0; x < length; x++)
                {
                    sprintf_s(extra, " %0x", data[x]);
                    strcat_s(str, extra);
                }
                scriptLog(str, 0);
        }
        return(false);
    }


    // Called when a new gig file has been loaded and when song is changed
    void OnSongChanged(int oldIndex, int newIndex) override
    {
        uint8_t row;
        
        // scriptLog("MC: Song changed to number " + std::to_string(newIndex), 1);
        // scriptLog("MC: getCurrentSongIndex = " + std::to_string(getCurrentSongIndex()), 1);
        
        for (row = 0; row < Surface.ButtonRows; row++)
        {
            if ((Surface.Row[row].Showing == SHOW_SONGS) && (getCurrentSongIndex() >= 0))
            {
                DisplaySongs(Surface.Row[row], true);
            }
            if ((Surface.Row[row].Showing == SHOW_SONGPARTS) && (getCurrentSongIndex() >= 0))
            {
                DisplaySongParts(Surface.Row[row], getCurrentSongpartIndex());
            }
        }
    } 

    // Called when a song part is changed
    void OnSongPartChanged(int oldIndex, int newIndex) override
    {
        uint8_t row;

        // scriptLog("MC: Song part changed to number " + std::to_string(newIndex), 1);
        // scriptLog("MC: getCurrentSongIndex = " + std::to_string(getCurrentSongIndex()), 1);

        for (row = 0; row < Surface.ButtonRows; row++)
        {
            if ((Surface.Row[row].Showing == SHOW_SONGPARTS) && (getCurrentSongIndex() >= 0))
            {
                DisplaySongParts(Surface.Row[row], newIndex);
            }
        }
    }

    // Called when entering song mode
    void OnModeChanged(int mode) override
    {
        uint8_t row;

        // scriptLog("MC: Mode changed to " + std::to_string(mode), 1);
        // scriptLog("MC: getCurrentSongIndex = " + std::to_string(getCurrentSongIndex()), 1);

        DisplayTopLeft(0, "");

        Surface.Row[3].Showing = (mode == 1) ? SHOW_SONGS : SHOW_RACKSPACES;
        Surface.Row[2].Showing = (mode == 1) ? SHOW_SONGPARTS : SHOW_VARIATIONS;
        DisplayRow(Surface.Row[3], true);
        DisplayRow(Surface.Row[2], true);

        // Light or turn off the SID_MIXER button to indicate if we're in Setlist mode (mode==1) or rackspace mode
        sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeNoteOnMessage(Surface.CommandButtons[SETLIST_TOGGLE], (mode == 1) ? BUTTON_LIT : BUTTON_OFF, 0));
    }

    // examine a vector of widgets and structure the Surface structure to reflect widgets in the rackspace or global rackspace
    void buildSurfaceModel(std::vector <std::string> widgetlist)
    {
        std::string widgetname, prefix, type, bank, column;

        for (auto index = widgetlist.begin(); index != widgetlist.end(); ++index)
        {
            widgetname = *index;
            widgetname = widgetname.substr(widgetname.find(":") + 1);

            std::vector< std::string> name_segments = ParseWidgetName(widgetname, '_');

            // scriptLog("bS sees widget " + widgetname, 1);

            if (widgetname == LAYOUT_WIDGETNAME)
            { 
                listenForWidget(LAYOUT_WIDGETNAME, true);
            }
            else if (name_segments.size() >= 4)
            {
                prefix = name_segments[0];
                type = name_segments[1];
                bank = name_segments[2];
                column = name_segments[3];

                // if it's a widget we're interested in, add a bank for it if it doesn't already exist, and listen for it
                if (prefix == THIS_PREFIX && bank != "active" )  // we don't listen for "active" widgets, which are generally just for linking to an OSC display
                {
                    if (Surface.addSurfaceBank(type, bank) == true) {  /* scriptLog("bS added bank for " + widgetname, 1); */ }
                    if (column != "b") { listenForWidget(widgetname, true); }
                }
            }
        }
    }

    /// examines a Row of widgets and determines which bank should be active and sets things accordingly
    /// this is required because after Rackspace or Variation switching we can end up in states where multiple banks or no banks are flagged as "active" by the 'b' widgets
    void setActiveBank(int row)
    {
        std::string widgetname;
        int index;

        Surface.Row[row].ActiveBank = -1;

        if (Surface.Row[row].BankIDs.empty())
        {
            Surface.Row[row].ActiveBank = -1;
        }
        else {
            for (index = 0; index < Surface.Row[row].BankIDs.size(); ++index)
            {
                widgetname = Surface.Row[row].WidgetPrefix + (std::string)"_" + Surface.Row[row].BankIDs[index] + (std::string)"_i";
                // scriptLog("sAFB sees " + widgetname + (std::string)" as " + std::to_string(getWidgetValue(widgetname)), 1);
                if (widgetExists(widgetname))
                {
                    if ((getWidgetValue(widgetname) > 0.99) && (Surface.Row[row].ActiveBank == -1)) {
                        Surface.Row[row].ActiveBank = index;
                        // scriptLog("setActiveBank set active to " + std::to_string(index) + " " + Surface.Row[row].BankIDs[index], 1);
                    }
                    else if (index != Surface.Row[row].ActiveBank) { setWidgetValue(widgetname, 0.3); }
                }
            }
            if (Surface.Row[row].ActiveBank == -1) { Surface.Row[row].ActiveBank = 0; } // if there are no bank indicator widgets, set ActiveBank to first bank
        }
    }

    // Called when rackspace changed
    void OnRackspaceActivated() override
    {
        uint8_t row, column, bank;
        std::string widgetname, setting;
        std::vector<std::string> widgetlist, globalwidgetlist;

        // scriptLog("MC: Rackspace Changed to " + getRackspaceName(getCurrentRackspaceIndex()),1);
        Surface.reportWidgetChanges = false;
        if (Surface.TextDisplay != SHOW_SONGS) {   
            ClearMCUDisplay();
        }

        // Clear the BankIDs and active bank data from the prior rackspace's widget set
        for (row = 0; row < std::size(Surface.Row); row++)
        {
            Surface.Row[row].ActiveBank = -1;
            Surface.Row[row].BankIDs.clear();
        }

        // scriptLog("Getting global widgets", 1);
        getWidgetList(globalwidgetlist, true);
        // buildSurfaceModel(widgetlist);

        // scriptLog("Getting local widgets", 1);
        getWidgetList(widgetlist, false);
        widgetlist.insert(widgetlist.end(), globalwidgetlist.begin(), globalwidgetlist.end());
        buildSurfaceModel(widgetlist);

        // scriptLog("Done buildSurface", 1);

        // Button row initializations
        for (row = 0; row < Surface.ButtonRows; row++) {
            setActiveBank(row);
            // scriptLog("Set active bank for row " + std::to_string(row) + " to " + std::to_string(Surface.Row[row].ActiveBank), 1);
            for (column = 0; column < Surface.Row[row].Columns; column++)
            {  // if widget doesn't exist, clear the LED so we don't have stray lit up junk
                if (Surface.Row[row].BankValid())
                {  // if there's no valid bank, or if there is a valid bank but no valid widget for this column, then un-light the button 
                    widgetname = THIS_PREFIX + (std::string)"_" + Surface.Row[row].WidgetID + "_" + Surface.Row[row].BankIDs[Surface.Row[row].ActiveBank] + "_" + std::to_string(column);
                    if (widgetExists(widgetname) != true && Surface.Row[row].Showing == SHOW_ASSIGNED) { DisplayWidgetValue(Surface.Row[row], column, BUTTON_OFF); }
                }
                else { DisplayWidgetValue(Surface.Row[row], column, BUTTON_OFF); }
            }
            DisplayRow(Surface.Row[row], true);
        }


        // scriptLog("Done button rows", 1);
        // we need to decisively set the active bank because it's possible that upon rackspace or variation change more than one, or zero, are set
        setActiveBank(KNOB_ROW);
        DisplayFaders(Surface.Row[KNOB_ROW]);
        // scriptLog("Done Knob row", 1);
        setActiveBank(FADER_ROW);
        DisplayFaders(Surface.Row[FADER_ROW]);

        DisplayModeButtons();


        // scriptLog("Finishing OnRackspaceChanged.", 1);

        Surface.reportWidgetChanges = true;
    }
    



    // Called when variation changed
    void OnVariationChanged(int oldIndex, int newIndex) override
    {
        int row;

        // scriptLog("MC: Variation Changed to " + std::to_string(newIndex) + getVariationName(getCurrentRackspaceIndex(), getCurrentVariationIndex()), 1);
        Surface.reportWidgetChanges = false;
        if (Surface.TextDisplay != SHOW_SONGS) { DisplayTopLeft(0, ""); } // clears the upper left part of display that shows last widget touched

        for (row = 0; row < Surface.ButtonRows; row++)
        {
            if (Surface.Row[row].Showing == SHOW_VARIATIONS) {
                DisplayVariations(Surface.Row[row], newIndex);
            }
        }

        // we need a routine to decisively set the active fader bank because it's possible that upon rackspace or variation change more than one, or zero, are set
        setActiveBank(KNOB_ROW);
        DisplayFaders(Surface.Row[KNOB_ROW]);

        setActiveBank(FADER_ROW);
        DisplayFaders(Surface.Row[FADER_ROW]);
        // scriptLog("Finishing OnVariationChanged.", 1);
        Surface.reportWidgetChanges = true;
    }

 

    void Initialization() override
    {
        // Do any basic initializations

        // Register callbacks.
        // We only register for OnGigLogaded() first because we don't want callbacks for all the other stuff until the Gig is fully loaded.

        registerCallback("OnStatusChanged");
        registerCallback("OnMidiDeviceListChanged");
        registerCallback("OnClose");

    }


    // Generally don't touch this - simply define the constant 'XMLProductDescription' at the top of this file with
    // an XML description of your product
    std::string GetProductDescription() override // This MUST be defined in your class
    {
        return XMLProductDescription;
    }
    
};

