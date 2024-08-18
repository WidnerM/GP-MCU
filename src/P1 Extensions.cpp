#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "LibMain.h"

void LibMain::InitializeP1M()
{
    int x;

    // Surface.Initialize();

    // Surface.FirstShownSong = 0;

    // clear display
    CleanP1M();
}

void LibMain::ClearP1MDisplay()
{
    // scriptLog("MCU clear display", 1);
    sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeSysexMessage(gigperformer::sdk::GPUtils::hex2binaryString(P1M_CLEAR_BOT)));
    sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeSysexMessage(gigperformer::sdk::GPUtils::hex2binaryString(P1M_CLEAR_TOP)));
}

void LibMain::CleanP1M()
{
    int x;

    // clear secondary display
    ClearP1MDisplay();

    // shut off all leds
    for (x = 0; x <= 0x76; x++) {
        sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeNoteOnMessage(x, 0, 0));
    }

    // clear the knobs
    for (x = 0; x <= 7; x++) {
        sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeControlChangeMessage(0x30 + x, 0, 0));
    }

    // clear the VU meters on real MCUs
    for (x = 0; x <= 7; x++) {
        sendMidiMessage(gigperformer::sdk::GPMidiMessage::makeSysexMessage(gigperformer::sdk::GPUtils::hex2binaryString(MCU_CLEAR_VU + (std::string)"0" +
            std::to_string(x) + "00 F7")));
    }

}


// put knob text on top for P1M, fader text on top - if Surface.P1MType is true always do it this way
// set up basic softbutton printing routine
// set up label rewriting routine - integrate into DisplayRow, WidgetValueChanged??
// implement color bars

void LibMain::DisplayP1MText(uint8_t column, uint8_t row, std::string text, uint8_t maxlength)
{
    std::string hexmessage, subtext, binmessage;

    // Could probably handle this better...  Adding blanks to the text to display so we're guaranteed to clear whatever's there, then just use front 'maxlength' chars
    if (column < 8) {

        subtext = cleanSysex(text);
        subtext = subtext.substr(0, maxlength) + "                                                                                                               ";
        int insertpoint = row * 0x38 + 7 * column;
        // hexmessage = Surface.P1MText;

        // Surface.P1MText.replace(insertpoint, 7, subtext.substr(0,7) );

        binmessage = gigperformer::sdk::GPUtils::hex2binaryString(P1M_TEXT_HDR);
        // binmessage += Surface.P1MText;
        binmessage += gigperformer::sdk::GPUtils::hex2binaryString("f7");
        sendMidiMessage(binmessage);
    }
}
