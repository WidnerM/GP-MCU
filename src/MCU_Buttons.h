// MCU button and knob CC assignments

#pragma once




#define VPOT_DISPLAY_SINGLE_DOT 0
#define VPOT_DISPLAY_BOOST_CUT 1
#define VPOT_DISPLAY_WRAP 2
#define VPOT_DISPLAY_SPREAD 3


#define KNOB_0 0x10   // Knob shows up on CC 0x10 - 0x17.  Channel 1 for first MCU.  Positive rotation show 01-0F, negative rotation 0x41-0x4F
#define KNOB_7 0x17
#define JOGWHEEL 0x3C  // Jog wheel CC 0x3C and same as knob rotation
#define KNOBRING_0 0x30  // For some reason we write to them on a different address

#define FADER_0 0xE0 // Faders come as pitch bend (E0) and different channels (1-9) for each fader
#define FADER_7 0xE7  // when a fader is touched it sends a note on, 0x68 - 0x70 for master, 127 on, 0 off


#define BUTTON_LIT 127
#define BUTTON_OFF	0
#define BUTTON_FLASH 1
#define BUTTON_CODE 0x90

#define MCU_LOWEST_BUTTON 0
#define MCU_HIGHEST_BUTTON 31  // this is the highest simple button (e.g., not a fancy function button)
#define SID_RECORD_ARM_BASE 0
#define SID_RECORD_ARM_CH1 0
#define SID_RECORD_ARM_CH2 1
#define SID_RECORD_ARM_CH3 2
#define SID_RECORD_ARM_CH4 3
#define SID_RECORD_ARM_CH5 4
#define SID_RECORD_ARM_CH6 5
#define SID_RECORD_ARM_CH7 6
#define SID_RECORD_ARM_CH8 7
#define SID_SOLO_BASE 8
#define SID_SOLO_CH1 8
#define SID_SOLO_CH2 9
#define SID_SOLO_CH3 0x0a
#define SID_SOLO_CH4 0x0b
#define SID_SOLO_CH5 0x0c
#define SID_SOLO_CH6 0x0d
#define SID_SOLO_CH7 0x0e
#define SID_SOLO_CH8 0x0f
#define SID_MUTE_BASE 0x10
#define SID_MUTE_CH1 0x10
#define SID_MUTE_CH2 0x11
#define SID_MUTE_CH3 0x12
#define SID_MUTE_CH4 0x13
#define SID_MUTE_CH5 0x14
#define SID_MUTE_CH6 0x15
#define SID_MUTE_CH7 0x16
#define SID_MUTE_CH8 0x17
#define SID_SELECT_BASE 0x18
#define SID_SELECT_CH1 0x19
#define SID_SELECT_CH2 0x1a
#define SID_SELECT_CH4 0x1b
#define SID_SELECT_CH5  0x1c
#define SID_SELECT_CH6  0x1d
#define SID_SELECT_CH7  0x1e
#define SID_SELECT_CH8  0x1f
#define SID_VPOD_PUSH_BASE  0x20
#define SID_VPOD_PUSH_CH1  0x20
#define SID_VPOD_PUSH_CH2  0x21
#define SID_VPOD_PUSH_CH3  0x22
#define SID_VPOD_PUSH_CH4  0x23
#define SID_VPOD_PUSH_CH5  0x24
#define SID_VPOD_PUSH_CH6  0x25
#define SID_VPOD_PUSH_CH7  0x26
#define SID_VPOD_PUSH_CH8  0x27

#define SID_ASSIGNMENT_TRACK  0x28
#define SID_ASSIGNMENT_SEND  0x29
#define SID_ASSIGNMENT_PAN  0x2a
#define SID_ASSIGNMENT_PLUG_IN  0x2b
#define SID_ASSIGNMENT_EQ  0x2c
#define SID_ASSIGNMENT_INSTRUMENT  0x2d

#define SID_FADERBANK_PREV_BANK  0x2e
#define SID_FADERBANK_NEXT_BANK  0x2f
#define SID_FADERBANK_PREV_CH  0x30
#define SID_FADERBANK_NEXT_CH  0x31
#define SID_FADERBANK_FLIP  0x32
#define SID_FADERBANK_GLOBAL  0x33

#define SID_DISPLAY_NAME_VALUE  0x34
#define SID_DISPLAY_SMPTE_BEATS  0x35

#define SID_FUNCTION_BASE  0x36
#define SID_F1  0x36
#define SID_F2  0x37
#define SID_F3  0x38
#define SID_F4  0x39
#define SID_F5  0x3a
#define SID_F6  0x3b
#define SID_F7  0x3c
#define SID_F8  0x3d

#define SID_VIEW_BASE  0x3e
#define SID_VIEW_MIDI  0x3e
#define SID_VIEW_INPUTS  0x3f
#define SID_VIEW_AUDIO  0x40
#define SID_VIEW_INSTRUMENT  0x41
#define SID_VIEW_AUX  0x42
#define SID_VIEW_BUSSES  0x43
#define SID_VIEW_OUTPUTS  0x44
#define SID_VIEW_USER  0x45

#define BCF_SONG_DOWN  0x46
#define BCF_SONG_UP  0x47

#define SID_MOD_SHIFT  0x46
#define SID_MOD_OPTION  0x47
#define SID_MOD_CTRL  0x48
#define SID_MOD_ALT  0x49

#define SID_AUTOMATION_READ  0x4a
#define SID_AUTOMATION_WRITE  0x4b

#define SID_AUTOMATION_TRIM  0x4c
#define SID_AUTOMATION_TOUCH  0x4d
#define SID_AUTOMATION_LATCH  0x4e
#define SID_AUTOMATION_GROUP  0x4f

#define SID_UTILITIES_SAVE 0x50
#define SID_UTILITIES_UNDO 0x51
#define SID_UTILITIES_CANCEL 0x52
#define SID_UTILITIES_ENTER 0x53

#define SID_MIXER  0x54
#define SID_MARKER  0x54
#define SID_NUDGE  0x55
#define SID_CYCLE  0x56
#define SID_DROP  0x57
#define SID_REPLACE  0x58
#define SID_CLICK  0x59
#define SID_SOLO  0x5a

#define SID_TRANSPORT_REWIND  0x5b
#define SID_TRANSPORT_FAST_FORWARD  0x5c
#define SID_TRANSPORT_STOP  0x5d
#define SID_TRANSPORT_PLAY  0x5e
#define SID_TRANSPORT_RECORD  0x5f


#define SID_JOG_CURSOR_UP  0x60
#define SID_JOG_CURSOR_DOWN  0x61
#define SID_JOG_CURSOR_LEFT  0x62
#define SID_JOG_CURSOR_RIGHT  0x63
#define SID_JOG_ZOOM  0x64
#define SID_JOG_SCRUB  0x65

#define SID_USER_FOOT_SWITCHA  0x66
#define SID_USER_FOOT_SWITCHB  0x67
#define SID_FADER_TOUCH_SENSE_BASE  0x68
#define SID_FADER_TOUCH_SENSE_CH1  0x68
#define SID_FADER_TOUCH_SENSE_CH2  0x69
#define SID_FADER_TOUCH_SENSE_CH3  0x6a
#define SID_FADER_TOUCH_SENSE_CH4  0x6b
#define SID_FADER_TOUCH_SENSE_CH5  0x6c
#define SID_FADER_TOUCH_SENSE_CH6  0x6d
#define SID_FADER_TOUCH_SENSE_CH7  0x6e
#define SID_FADER_TOUCH_SENSE_CH8  0x6f
#define SID_FADER_TOUCH_SENSE_MASTER  0x70