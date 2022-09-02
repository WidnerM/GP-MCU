#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"


//  Call this when the rackspace changes or the active button bank changes
    //   For different hardware buttons are color LEDs, full RGB, simple on/off, or on/off/flash.
    //   Buttons can also be put into flash and pulse modes, which we do individually rather than as a bank like this.
    //   DO NOT SET WIDGET VALUES IN HERE THAT HAVE CALLBACKS
 
void LibMain::LightButtonsForRow(SurfaceRow Row, uint8_t firstbutton, uint8_t number)
{
    std::string widgetname, oscwidget;
    std::string Caption, Label, Extras;
    double Value = 0;
    std::string hexmessage, subtext, binmessage;
    int x, LitColor, DimColor = SLMKIII_BLACK;
    uint8_t upcolor = 0;
    uint8_t downcolor = 0;

    if (true) // only do this stuff if there is at least one bank   std::size(Row.BankIDs) > 0
    {
        for (x = 0; x < std::size(Row.BankIDs) ; x++)  // cycle through banks to turn on/off the panel widget indicators for active bank and set up/down arrows for colors
        {
            widgetname = THIS_PREFIX + (std::string)"_" + Row.WidgetID + "_" + Row.BankIDs[x] + "_i";
            if (widgetExists(widgetname)) {
                setWidgetValue(widgetname, (x == Row.ActiveBank) ? 1 : 0.3); // light up panel for active bank, otherwise turn it off
                if (x == (Row.ActiveBank - 1)) {    // light up the button for "next bank" of one exists
                    upcolor = ON_COLOR;
                }
                if (x == (Row.ActiveBank + 1))
                {
                    downcolor = ON_COLOR;
                }
            }
        }

        // we go 8 back to 0 here so we can pickup the knobcolor for the whole group on knob 8
        for (x = firstbutton + number - 1; x >= firstbutton; x--)
        {
            if (Row.ActiveBank >= 0) {
                widgetname = THIS_PREFIX + (std::string)"_" + Row.WidgetID + "_" + Row.BankIDs[Row.ActiveBank] + "_" + std::to_string(x);
            }
            if (Row.ActiveBank >= 0 && widgetExists(widgetname))
            {
                Value = getWidgetValue(widgetname);
                Label = getWidgetCaption(widgetname);
            }
            else  // we end up here if the widget doesn't exist or there's no ActiveBank
            {
                Label = "";
                Caption = "";
                Value = 0.0;
                LitColor = SLMKIII_BLACK;
            }
            oscwidget = THIS_PREFIX + (std::string)"_" + Row.WidgetID + "_active_" + std::to_string(x);  // same widget name with the bank # set to "active"
            setWidgetCaption(oscwidget, Label);
            setWidgetValue(oscwidget, 1.0 - Value);  // ensure a widget toggle so caption gets propagated
            setWidgetValue(oscwidget, Value);
            DisplayWidgetValue(Row, x, Value);
        }
    }
}

