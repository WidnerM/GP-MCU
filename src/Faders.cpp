#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"


//  Call this when the rackspace changes or the active fader bank changes

// Displays Faders or Knobs on the LCD display
void LibMain::DisplayFaders(SurfaceRow Row)  
{
    std::string widgetname, oscwidget, oscname;
    std::string Caption, Label, Extras, TextValue;
    double Value = 0, oldvalue;
    std::string hexmessage, subtext, binmessage;
    uint8_t BarColor = SLMKIII_BLACK, KnobColor = SLMKIII_BLACK;
    uint8_t upcolor = 0;
    uint8_t downcolor = 0;
    int x;


    if (Row.BankIDs.empty() || Row.ActiveBank > (Row.BankIDs.size() - 1)) // if there are no FaderBanks or ActiveFaderBank is out of range we clear the display
    {
        if ((Surface.TextDisplay == SHOW_FADERS && Row.WidgetID == "f") || (Surface.TextDisplay == SHOW_KNOBS && Row.WidgetID == "k")) { ClearMCUDisplay(); }
    }
    else
    { // Set the bank indicator widgets, if they exist
        for (x = 0; x < Row.BankIDs.size(); x++)
        {
            widgetname = Row.WidgetPrefix + (std::string)"_" + Row.BankIDs[x] + "_i";
            // scriptLog("DisplayFaders setting bank indicator " + widgetname, 1);
            if (widgetExists(widgetname)) {
                setWidgetValue(widgetname, (x == Row.ActiveBank) ? 1.0 : 0.3); // light up panel for active bank, otherwise turn it off
            }
        }

        // check for a bank name on mc_f_[ActiveBank]_p widget & display it on upper right of display
        widgetname = Row.WidgetPrefix + (std::string)"_" + Row.BankIDs[Row.ActiveBank] + "_p";
        if (widgetExists(widgetname))
        {
            //        DisplayBankInfo(getWidgetCaption(widgetname) + ":Faders " + std::to_string(Surface.ActiveFaderBank));
            if (Surface.TextDisplay != SHOW_SONGS) {
                DisplayBankInfo(getWidgetCaption(widgetname) + ":" + Row.RowLabel + " " + Row.BankIDs[Row.ActiveBank]);
            }
        }
        else // if a name doesn't exist for the bank, just show the bank ID
        {
            //        DisplayBankInfo("Faders " + std::to_string(Surface.ActiveFaderBank));
            if (Surface.TextDisplay != SHOW_SONGS) {
                DisplayBankInfo(Row.RowLabel + " " + Row.BankIDs[Row.ActiveBank]);
            }
        }
    }

    // Set the positions and displays for each of the faders using the active bank
    for (x = Row.Columns; x >= 0; x--)
    {
        if (!Row.BankIDs.empty())
        {
            widgetname = Row.WidgetPrefix + (std::string)"_" + Row.BankIDs[Row.ActiveBank] + "_" + std::to_string(x);
            if (widgetExists(widgetname) == true)
            {
                Value = getWidgetValue(widgetname);
                TextValue = getWidgetTextValue(widgetname);
                Label = getWidgetCaption(widgetname);
                Caption = "";
                if (Surface.TextDisplay != SHOW_SONGS) { DisplayControlLabel((uint8_t)x, Label); } // show the label on the MCU LCD by sending midi
                // ShowFaderCaption((uint8_t)x, TextValue);  // this only outputs to the display on the MCU
                if (Row.Type == FADER_TYPE || Row.Type == KNOB_TYPE) { DisplayWidgetValue(Row, (uint8_t)x, Value); }
            }
            else  // we end up here if the widget doesn't exist, so then we set the whole thing blank
            {
                Label = " ";
                Caption = " ";
                Value = 0;
                KnobColor = SLMKIII_BLACK;
                // Show = false;
            }
        }
        else {
            Label = " ";
        } 
        // if the OSC interface widget exists, set or clear it appropriately
        oscwidget = Row.WidgetPrefix + (std::string)"_active_" + std::to_string(x);

        // scriptLog("MC: Set " + oscwidget + " to " + Label,1);
        if (widgetExists(oscwidget) == true)
        {
            setWidgetCaption(oscwidget, Label);
            // OSC won't send update for Caption only, we need to make sure Value changes.
            if (Value == getWidgetValue(oscwidget)) { Value > 0.9 ? setWidgetValue(oscwidget, Value - 0.05) : setWidgetValue(oscwidget, Value + 0.05); }
            setWidgetValue(oscwidget, Value);
        }
    }
}
