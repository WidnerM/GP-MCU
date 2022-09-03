#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"


// Initialize MCU screen and any essential variables
void LibMain::InitializeMCU()
{
    int x;

    Surface.Initialize();

    Surface.FirstShownSong = 0;

    // clear display
    CleanMCU();
}

void LibMain::ClearMCUDisplay()
{
    // scriptLog("MCU clear display", 1);
    sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeSysexMessage(gigperformer::sdk::GPUtils::hex2binaryString(MCU_CLEAR_BOT)));
    sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeSysexMessage(gigperformer::sdk::GPUtils::hex2binaryString(MCU_CLEAR_TOP)));
}

void LibMain::CleanMCU()
{
    int x;

    // clear display
    ClearMCUDisplay();

    // shut off all leds
    for (x = 0; x <= 0x76; x++) {
        sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeNoteOnMessage(x, 0, 0));
    }

    // clear the knobs
    for (x = 0; x <= 7; x++) {
        sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeControlChangeMessage(0x30 + x, 0, 0 ));
    }

    // clear the VU meters on real MCUs
    for (x = 0; x <= 7; x++) {
        sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeSysexMessage(gigperformer::sdk::GPUtils::hex2binaryString(MCU_CLEAR_VU + (std::string)"0" +
        std::to_string(x) + "00 F7")));
    }

}

// Transmits sysex midi data to display a message on the MCU text display
void LibMain::DisplayText(uint8_t column, uint8_t row, std::string text, uint8_t maxlength)
{
    std::string hexmessage, subtext, binmessage;

    // Could probably handle this better...  Adding blanks to the text to display so we're guaranteed to clear whatever's there, then just use front 'maxlength' chars
    if (column < 8) {

        subtext = cleanSysex(text);
        subtext = subtext.substr(0, maxlength) + "                                                                ";
        hexmessage = MCU_TEXT_HDR + gigperformer::sdk::GPUtils::intToHex(row * 0x38 + column * 7) + textToHexString(subtext.substr(0, (maxlength % 7 == 0 ) ? maxlength : maxlength + 7 - maxlength % 7)) + (std::string)" f7";
        binmessage = gigperformer::sdk::GPUtils::hex2binaryString(hexmessage);
        sendMidiMessage(binmessage);
    }
}

// Displays a string on the upper right of the MCU text display
void LibMain::DisplayBankInfo(std::string text)
{
    std::string hexmessage, subtext, binmessage;

    subtext = "                                                                " + cleanSysex(text);
    hexmessage = MCU_TEXT_HDR + (std::string) "1C" + textToHexString(subtext.substr(subtext.length()-28, subtext.length())) + (std::string)" f7";
    binmessage = gigperformer::sdk::GPUtils::hex2binaryString(hexmessage);
    sendMidiMessage(binmessage);
}


// Show value of a widget on it's linked control surface item
void LibMain::DisplayWidgetValue(SurfaceRow Row, uint8_t column, double value)
{
    uint8_t MidiMessage[3];
    int newvalue;

    MidiMessage[0] = Row.MidiCommand;

    if (Row.Type == BUTTON_TYPE)
    {
        MidiMessage[1] = Row.FirstID + column;
        MidiMessage[2] = (value != 0) ? BUTTON_LIT : BUTTON_OFF;
    }
    else if (Row.Type == FADER_TYPE)
    {
        newvalue = (int)(value * 16383); // convert to 14 bit value
        MidiMessage[0] += (column & 0x0F);  // MCU faders run on "pitch bend" 0xE[channel], with channel being fader 0-7 and master on 8.
        MidiMessage[1] = (uint8_t)(newvalue & 0x70);  // these are always 10 bit as far as I know
        MidiMessage[2] = (uint8_t)(newvalue >> 7 & 0x7f);
    }
    else if (Row.Type == KNOB_TYPE)
    {
        MidiMessage[1] = KNOBRING_0 + (column & 0x0F);
        MidiMessage[2] = (uint8_t)(value * 10.9999) + 1;
    }
    else if (Row.Type == KNOB_BUTTON_TYPE)
    {

    }
    sendMidiMessage(MidiMessage, sizeof(MidiMessage));
}

// light up or turn off a button on the control surface
void LibMain::DisplayButton(uint8_t button, uint8_t value)
{
    sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeNoteOnMessage(button, (value == 0) ? BUTTON_OFF : BUTTON_LIT, 0));
}

// light buttons to indicate what display mode we're in - showing faders, knobs, or songs/racks on the LCD display
void LibMain::DisplayModeButtons()
{
    sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeNoteOnMessage(Surface.CommandButtons[FADERS_SELECT], (Surface.TextDisplay == SHOW_FADERS) ? BUTTON_LIT : BUTTON_OFF, 0));
    sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeNoteOnMessage(Surface.CommandButtons[KNOBS_SELECT], (Surface.TextDisplay == SHOW_KNOBS) ? BUTTON_LIT : BUTTON_OFF, 0));
    sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeNoteOnMessage(Surface.CommandButtons[SONGSRACKS_SELECT], (Surface.TextDisplay == SHOW_SONGS) ? BUTTON_LIT : BUTTON_OFF, 0));
}

// Display a notification message - should have this notify on the MCU display, but we'd need some kind of timer to refresh the display
void LibMain::Notify(std::string text)
{
    consoleLog(text);
}

/// <summary>
/// Displays text on the top left portion of the MCU display using a MIDI sysex message.  Unsafe chars in the text will be deleted before sending.
/// </summary>
/// <param name="column">unused, deprecated</param>
/// <param name="label">text, automatically trimmed to 26 characters</param>
void LibMain::DisplayTopLeft(uint8_t column, const std::string label)
{
    if (column <= 8) { DisplayText(0, 0, label, 26); }
}

void LibMain::DisplayControlLabel(uint8_t column, const std::string label)
{
    if (column <= 7) {
        DisplayText(column, 1, label, 6);
    }
}

