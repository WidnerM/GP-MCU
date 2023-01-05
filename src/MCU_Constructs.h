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
