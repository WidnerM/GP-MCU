#pragma once

#include "MCU_Constructs.h"
#include "MCU_Colors.h"
#include "MCU_Buttons.h"
#include "LibMain.h"

// Define identifiers GP user must use to name their widgets
#define THIS_PREFIX "mc"
#define RECORD_PREFIX "mc_r"
#define SOLO_PREFIX "mc_s"
#define MUTE_PREFIX "mc_m"
#define SELECT_PREFIX "mc_l"
#define FADER_PREFIX "mc_f"
#define KNOB_PREFIX "mc_k"
#define KNOB_BUTTON_PREFIX "mc_b"
#define FUNCTION_PREFIX "mc_n"
#define VIEW_PREFIX "mc_v"
#define ROW_PREFIX_ARRAY {RECORD_PREFIX, SOLO_PREFIX, MUTE_PREFIX, SELECT_PREFIX, FUNCTION_PREFIX, VIEW_PREFIX, KNOB_BUTTON_PREFIX, FADER_PREFIX, KNOB_PREFIX}



#define RECORD_TAG "r"
#define SOLO_TAG "s"
#define MUTE_TAG "m"
#define SELECT_TAG "l"
#define FADER_TAG "f"
#define KNOB_TAG "k"
#define KNOB_BUTTON_TAG "b"
#define FUNCTION_TAG "n"
#define VIEW_TAG "v"

#define TAG_ARRAY {RECORD_TAG, SOLO_TAG, MUTE_TAG, SELECT_TAG, FUNCTION_TAG, VIEW_TAG, KNOB_BUTTON_TAG, FADER_TAG, KNOB_TAG}
#define ROW_LABEL_ARRAY { "Rec", "Solo", "Mute", "Sel", "Fn", "View", "KPush" , "Fader", "Knob"}

#define BUTTON_TYPE "Button"
#define FADER_TYPE "Fader"
#define KNOB_TYPE "Knob"
#define KNOB_BUTTON_TYPE "KButton"
#define ROW_TYPE_ARRAY {BUTTON_TYPE, BUTTON_TYPE, BUTTON_TYPE, BUTTON_TYPE, BUTTON_TYPE, BUTTON_TYPE, KNOB_BUTTON_TYPE, FADER_TYPE, KNOB_TYPE}


#define SHOW_SONGS 0
#define SHOW_SONGPARTS 1
#define SHOW_RACKSPACES 2
#define SHOW_VARIATIONS 3
#define SHOW_ALLVARIATIONS 4
#define SHOW_BUTTONS 5
#define SHOW_KNOBS 6
#define SHOW_FADERS 7
#define SHOW_KNOB_BUTTONS 8
#define SHOW_ASSIGNED 9
#define SHOW_ARRAY { SHOW_BUTTONS, SHOW_BUTTONS, SHOW_SONGPARTS, SHOW_SONGS, SHOW_BUTTONS, SHOW_BUTTONS, SHOW_KNOB_BUTTONS, SHOW_FADERS, SHOW_KNOBS}


#define RECORD_ROW 0
#define SOLO_ROW 1
#define MUTE_ROW 2
#define SELECT_ROW 3
#define FUNCTION_ROW 4
#define VIEW_ROW 5
#define KNOB_BUTTON_ROW 6
#define FADER_ROW 7
#define KNOB_ROW 8


#define SONGSRACKS_SELECT 0
#define KNOBS_SELECT 1
#define FADERS_SELECT 2
#define SONGS_BANK_UP 3
#define SONGS_BANK_DOWN 4
#define KNOBS_BANK_UP 5
#define KNOBS_BANK_DOWN 6
#define FADERS_BANK_UP 7
#define FADERS_BANK_DOWN 8
#define SETLIST_TOGGLE 9
#define DEFAULT_COMMAND_BUTTONS {SID_ASSIGNMENT_TRACK, SID_ASSIGNMENT_PAN, SID_ASSIGNMENT_EQ, SID_TRANSPORT_FAST_FORWARD, SID_TRANSPORT_REWIND, SID_FADERBANK_NEXT_BANK, SID_FADERBANK_PREV_BANK, SID_FADERBANK_NEXT_CH, SID_FADERBANK_PREV_CH, SID_TRANSPORT_RECORD }
#define ICON_MPLUS_COMMAND_BUTTONS {SID_AUTOMATION_WRITE, SID_AUTOMATION_READ, SID_MARKER, SID_TRANSPORT_FAST_FORWARD, SID_TRANSPORT_REWIND, SID_FADERBANK_NEXT_BANK, SID_FADERBANK_PREV_BANK, SID_FADERBANK_NEXT_CH, SID_FADERBANK_PREV_CH, SID_TRANSPORT_RECORD }

class SurfaceRow
{
public:
	std::vector<std::string> BankIDs;
	std::string WidgetPrefix; // the hardware identifier (e.g. "mc") + the WidgetID (e.g. "f") together to simplify a bunch of widget checks (e.g., mc_f)
	std::string WidgetID;  // the widget ID that signifies this row, e.g., f, k, b, etc.  Right now these are defined in Display.cpp as part of the InitializeMCU routine.
	std::string RowLabel = "";  // a friendly name for what the row is, e.g., "Faders", "Knobs", etc.  Used to indicate on the display what bank of controls the displayed labels are for

	std::string Type = BUTTON_TYPE;

	int ActiveBank = -1;
	uint8_t Showing = SHOW_ASSIGNED;
	uint8_t Columns = 8;
	uint8_t MidiCommand = 0x90;
	uint8_t FirstID = 0;

	bool BankValid()
	{
		return (ActiveBank >= 0 && ActiveBank < BankIDs.size());
	}

	bool addBank(std::string bank) {
		int index, result;
		bool found = false;

		for (index = 0; index < BankIDs.size(); index++)
		{
			result = BankIDs[index].compare(bank);
			if (result == 0)
			{
				found = true;
				break;
			}
			else if (result > 0) { break; }
		}
		if (!found) {
			BankIDs.insert(BankIDs.begin() + index, bank);
		}
		return (! found);
	}

	int NextBank()
	{
		if (ActiveBank < 0)
		{
			return -1;
		}
		else if (ActiveBank < BankIDs.size() - 1)
		{
			return ActiveBank + 1;
		}
		else
		{
			return 0;
		}
	}

	int PreviousBank()
	{
		if (ActiveBank > 0) { return ActiveBank - 1; }
		else { return BankIDs.size() - 1; }
	}

	bool IncrementBank()
	{
		if (NextBank() >= 0)
		{
			ActiveBank = NextBank();
			return true;
		}
		else return false;
	}

	bool DecrementBank()
	{
		if (PreviousBank() >= 0)
		{
			ActiveBank = PreviousBank();
			return true;
		}
		else return false;
	}

	bool makeActiveBank(std::string bank) {
		int index, result;
		bool found = false;

		for (index = 0; index < BankIDs.size(); index++)
		{
			result = BankIDs[index].compare(bank);
			if (result == 0)
			{
				found = true;
				ActiveBank = index;
				break;
			}
			else if (result > 0) { break; }
		}
		return found;
	}
};

class SurfaceClass
{
public:
	SurfaceRow Row[9];
	uint8_t ButtonRows = 7;
	std::string IgnoreWidget = "";
	bool reportWidgetChanges = true;

	uint8_t TextDisplay = SHOW_FADERS;
	int FirstShownSong = 0;
	int FirstShownRack = 0;
	uint8_t CommandButtons[10] = DEFAULT_COMMAND_BUTTONS;
	uint8_t ButtonLayout = 0;

	bool Initialize()
	{
		int x;
		std::string row_prefixes[] = ROW_PREFIX_ARRAY;
		std::string row_tags[] = TAG_ARRAY;
		std::string row_types[] = ROW_TYPE_ARRAY;
		std::string row_labels[] = ROW_LABEL_ARRAY;
		uint8_t show_array[] = SHOW_ARRAY;
		uint8_t midi_commands[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xB0, 0xE0, 0xB0 };
		int row_columns[] = { 8, 8, 8, 8, 8, 8, 8, 9, 8 };

		uint8_t first_midi[] = { SID_RECORD_ARM_BASE, SID_SOLO_BASE, SID_MUTE_BASE, SID_SELECT_BASE, SID_FUNCTION_BASE, SID_VIEW_BASE, SID_VPOD_PUSH_BASE, FADER_0, KNOB_0};

		// basic Surface structure initializations
		for (x = 0; x < std::size(Row); x++)
		{
			Row[x].WidgetPrefix = row_prefixes[x];
			Row[x].WidgetID = row_tags[x];
			Row[x].RowLabel = row_labels[x];
			Row[x].Type = row_types[x];
			Row[x].Columns = row_columns[x];
			Row[x].FirstID = first_midi[x];
			Row[x].MidiCommand = midi_commands[x];
			Row[x].Showing = SHOW_ASSIGNED; // show_array[x]; thinking about how to handle this...  SHOW_ASSIGNED which means show what the WidgetID says, or use the SHOW_xxxx codes
		}
		Row[3].Showing = SHOW_SONGS;
		Row[2].Showing = SHOW_SONGPARTS;

		return true;
	}

	bool addSurfaceBank(std::string type, std::string bank) {
		int index;

		for (index = 0; index < std::size(Row); index++) {
			if (type == Row[index].WidgetID)
			{
				return Row[index].addBank(bank);
			}
		}
		return false;  // if we get here, we didn't find an appropriate Row in the array
	}

	void SwapRows(int a, int b) {
		SurfaceRow temp;

		temp = Row[a];
		Row[a].WidgetID = Row[b].WidgetID;
		Row[a].WidgetPrefix = Row[b].WidgetPrefix;
		Row[b].WidgetID = temp.WidgetID;
		Row[b].WidgetPrefix = temp.WidgetPrefix;

	}
};

