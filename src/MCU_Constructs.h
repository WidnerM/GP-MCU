// For classes and such for MCU controllers

#define MCU_IN        "Platform M+ V2.15"
#define MIDI_IN_WIDGETNAME "mc_midiin"
#define MCU_OUT       "Platform M+ V2.15"
#define MIDI_OUT_WIDGETNAME "mc_midiout"
#define LAYOUT_WIDGETNAME "mc_layout"
#define RACKROW_WIDGETNAME "mc_rackrow"
#define VARROW_WIDGETNAME "mc_variationrow"

#define EXTENSION_IDENTIFIER "MCU extension"

#define UNIVERSAL_QUERY "F0 7E 7F 06 01 F7"
#define MFG_ID { 0x00, 0x20, 0x29}
#define FAMILY_CODE { 0x01, 0x01}
#define MEMBER_CODE { 0x00, 0x00}
#define DEVICE_CODE { 0x00, 0x20, 0x29, 0x01, 0x01, 0x00, 0x00 }


#define MCU_SYS_HEADER "F0 00 00 66 14 "
#define MCU_SYS_ENDER "F7"

// The Platform M+ doesn't respond to any of these
#define MCU_ZERO_FADERS "F0 00 00 66 14 61 F7"
#define MCU_LEDS_OFF    "F0 00 00 66 14 62 F7"
#define MCU_RESET		"F0 00 00 66 14 63 F7"
#define MCU_CLEAR_VU	"F0 00 00 66 14 20 "

// Commands for display on LCD are simple
// MCU_SYS_HEADER + [address] + ascii text + F7
// address 0-0x37 is top line (lower line on Platform M+), bottom line is 0x38-0x6F.  Room for 7 chars on each, but really 6 to allow spacing
#define MCU_TEXT_HDR	"F0 00 00 66 14 12"
#define MCU_TOP_HDR	"F0 00 00 66 14 12 00"
#define MCU_BOT_HDR	"F0 00 00 66 14 12 38"
#define MCU_CLEAR_TOP "f0 00 00 66 14 12 00 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 f7"
#define MCU_CLEAR_BOT "f0 00 00 66 14 12 38 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 f7"

// iCon P1-M specific things
// This is a regex we can check to see if the midi port name belongs to a P1-M
#define P1M_REGEX ".*iCON P1-M.*"

// this will define all five pages of softkeys to note on 0-79, channel 1
#define P1M_KEYDEF_1 ""
#define P1M_KEYDEF_2 ""
#define P1M_KEYDEF_3 ""
#define P1M_KEYDEF_CLOSE "F0 1D 03 10 07 25 01 09 1C 09 09 03 24 7F 00 02 00 09 09 03 25 7F 00 02 00 09 09 03 26 7F 00 02 00 09 09 03 27 7F 00 02 00 0B 0B 03 10 40 00 02 00 0B 0B 03 11 40 00 02 00 0B 0B 03 12 40 00 02 00 0B 0B 03 13 40 00 02 00 0B 0B 03 14 40 00 02 00 0B 0B 03 15 40 00 02 00 0B 0B 03 16 40 00 02 00 0B 0B 03 17 40 00 02 00 09 09 04 20 7F 00 02 00 09 09 04 21 7F 00 02 00 09 09 04 22 7F 00 02 00 09 09 04 23 7F 00 02 00 09 09 04 24 7F 00 02 00 09 09 04 25 7F 00 02 00 09 09 04 26 7F 00 02 00 09 09 04 27 7F 00 02 00 0B 0B 04 10 40 00 02 00 0B 0B 04 11 40 00 02 00 0B 0B 04 12 40 00 02 00 0B 0B 04 13 40 00 02 00 0B 0B 04 14 40 00 02 00 0B 0B 04 15 40 00 02 00 0B 0B 04 16 40 00 02 00 0B 0B 04 17 40 00 02 00 F7"
// this is to write button labels to the softkeys
#define P1M_NAME_START "F0 1D 03 10 07 26 01 01 1C"
#define P1M_NAME_PAGE 7
#define P1M_NAMES_PER_PAGE 10

#define P1M_TEXT_HDR  "F0 00 00 67 15 13 38"
#define P1M_CLEAR_TOP "f0 00 00 66 14 12 00 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 f7"
#define P1M_CLEAR_BOT "f0 00 00 66 14 12 38 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 f7"
