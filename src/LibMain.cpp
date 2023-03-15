#include "LibMain.h"


// List of panels
std::vector<std::string> panelNames = { "MCU to OSC" };
std::vector<std::string> relativePanelLocations = { "MCUtoOSC.gppanel" };


std::string pathToMe; // This needs to be initialized from the initialization secttion of the LibMain class so it can be used in the standalone functions directly below

int LibMain::GetPanelCount()
{
    return panelNames.size();
}

std::string  LibMain::GetPanelName(int index)
{
    std::string text;
    if (index >= 0 && index < panelNames.size())
        text = panelNames[index];

    return text;
}

// Return panel layout in XML format
std::string  LibMain::GetPanelXML(int index)
{
    std::string text;
    if (index >= 0 && index < panelNames.size())
    {
        // We assume the panels are in the same folder as the library
        // scriptLog("MC: Path to panels: " + getPathToMe() + relativePanelLocations[index], 0);
        gigperformer::sdk::GPUtils::loadTextFile(getPathToMe() + relativePanelLocations[index], text);
    }
    return text;
}



// List of menu items
std::vector<std::string> menuNames = { "MCU Standard Layout", "Icon M+ Layout", "X-Touch Layout", "Re-initialize extention" };


int LibMain::GetMenuCount()
{
    return menuNames.size();
}

std::string  LibMain::GetMenuName(int index)
{
    std::string text;
    if (index >= 0 && index < menuNames.size())
        text = menuNames[index];

    return text;
}


void LibMain::InvokeMenu(int index)
{
    std::vector <std::string> widgetlist;
    std::string widgetname;

    if (index >= 0 && index < menuNames.size())
    {
        switch (index)
        {
        case 0:
            if (widgetExists(LAYOUT_WIDGETNAME)) { setWidgetValue(LAYOUT_WIDGETNAME, 1.0); }  // Layout # is stored in a widget as 1/x because value is constrained to 0-1.  ie, 0.25 = layout 4
            else SetSurfaceLayout(0);
            break;
        case 1:
            if (widgetExists(LAYOUT_WIDGETNAME)) { setWidgetValue(LAYOUT_WIDGETNAME, 0.5); }
            else SetSurfaceLayout(1);
            break;
        case 2:
            if (widgetExists(LAYOUT_WIDGETNAME)) { setWidgetValue(LAYOUT_WIDGETNAME, 0.33333); }
            else SetSurfaceLayout(2);
            break;
        case 3:
            OnStatusChanged(GPStatus_GigFinishedLoading);
            break;

        default:
            break;
        }
    }
}

void LibMain::sendMidiMessage(std::string MidiMessage) {
    for (int i = 0; i < MidiOut.size(); i++) {
        sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage);
    }
}

void LibMain::sendMidiMessage(gigperformer::sdk::GPMidiMessage MidiMessage) {
    for (int i = 0; i < MidiOut.size(); i++) {
        sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage);
    }
}

void LibMain::sendMidiMessage(const uint8_t* MidiMessage, int length) {
    for (int i = 0; i < MidiOut.size(); i++) {
        sendMidiMessageToMidiOutDevice(MidiOut[i], MidiMessage, length);
    }
}

// MCU compatible surfaces come in different configurations.  We have a configuration variable in the Surface structure so we can define different action keys for different functions
void LibMain::SetSurfaceLayout(uint8_t config) {
    const uint8_t std_commandarray[] = DEFAULT_COMMAND_BUTTONS;
    const uint8_t config_array[3][sizeof(std_commandarray)] = { DEFAULT_COMMAND_BUTTONS, ICON_MPLUS_COMMAND_BUTTONS, XTOUCH_COMMAND_BUTTONS };

    if (config >= 0 && config <= 2)
    {
        for (auto x = 0; x < sizeof(Surface.CommandButtons); ++x)
        {
            DisplayButton(Surface.CommandButtons[x], BUTTON_OFF);  // turn off all the prior mode indicators
            Surface.CommandButtons[x] = config_array[config][x];
        }
        Surface.ButtonLayout = config;
        DisplayModeButtons();
    }
}

gigperformer::sdk::GigPerformerAPI* gigperformer::sdk::CreateGPExtension(LibraryHandle handle)
{
    return new LibMain(handle);
}