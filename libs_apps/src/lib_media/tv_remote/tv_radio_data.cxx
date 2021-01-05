#include "tv_panel.h"
#include "tv_radio_data.h"

// DATA DECLARATIONS REQUIRE NAMESPACE // using namespace TV_REM;

/*
	/etc/X11/XF86Config Hitatchi, CM751ET,
	Modeline  "pal"  50.00  768 832 856 1000  576 590 595 630  -hsync -vsync

*/

namespace TV_REM {

#define N_items_in_array(ary) ( sizeof( ary ) / sizeof( ary[0] ))

#define FV DS608P // FreeView Daewoo // see ir_client.cxx send_commands()

/*!
	The radio channels that reach the arial of the PCI tv/radio card

	These values are used, this library sets the tuning
*/

	radio_channel_info radio_channel_table[] = {
	 { "102.9 Two-Ten", 		102.9, 0.9 },
//	 { "88.5 Two -> 89.8",		88.5, 0.9 },
	 { "89.8 Two",	 		89.8, 0.9 },
	 { "91.9 One", 			99.1, 0.9 },
	 { "92.0 Three", 		92.0, 0.9 },
	 { "93.5 Four 94.1", 		93.5, 0.9 },
	 { "94.2 Four", 		94.2, 0.9 },
	 { "95.8 Capital", 		95.8, 0.9 },
	 { "97.0 Two Ten", 		97.0, 0.9 },
	 { "98.1 One", 			98.1, 0.9 },
	 { "99.4 One", 			99.4, 0.9 },
	 { "100.5 -",			100.5, 0.9 },
	 { "100.9 -",			100.9, 0.9 },
//	 { "101.3 - Use 100.3", 	101.3, 0.9 },
	 { "101.1 - berks",	 		101.1, 0.9 },
	 { "101.8 -",	 		101.8, 0.9 },
	 { "102.9 Two-Ten", 		102.9, 0.9 },
	 { "104.4 Berks FM", 		104.4, 0.9 },
	 { "105.2 Wave", 		105.2, 0.9 },
	 { "107.0 Reading", 		107.0, 0.9 },
	 { "107.6 Kestrel", 		107.6, 0.9 }
	};

	int N_radio_channel_table = N_items_in_array( radio_channel_table );


/*!
	The TV channels that reach my tuner

	The data is not all used,
	channel.fine data is for later,
	for now xawtv does the work,
	taking its values from ~/.xawtv

	NB CABLE is an NTL Cable box which itself receives many channels
	(TODO add another FreeView box ?)
*/

	tv_chan_info tv_chan_table[] = {
	 { "One",    "1", 1.0, 38, -12 },
	 { "Two",    "2", 1.0, 40, -14 },
	 { "Three",  "3", 1.0, 44,  -9 },
	 { "Four",   "4", 1.0, 46,  +9 },
	 { "Five",   "5", 1.0, 48,  +9 },
	 { "Daewoo", "6", 1.0, 69,   0 },
	 { "VCR",    "7", 1.0, 60,   0 },
	 { "CABLE",  "8", 1.0, 54,   0 }
	};
	int N_tv_chan_table = N_items_in_array( tv_chan_table );

// MENUS

	ir_menu ir_menu_table[] =
	{
	 { M01, "Menu 01" },
	 { M02, "Menu 02" },
	 { M03, "Menu 03" },
	 { M04, "Menu 04" },
	 { M05, "Menu 05" },
	 { M06, "Menu 06" },
	 { M07, "Menu 07" },
	 { M08, "Menu 08" },
	 { M09, "Menu 09" },
	 { M10, "Menu 10" },
	 { M11, "Menu 11" },
	 { M12, "Menu 12" },
	 { M13, "Menu 13" },
	 { M14, "Menu 14" },
	 { M15, "Menu 15" },
	 { M16, "Menu 16" },
	 { M17, "Menu 17" },
	 { M18, "Menu 18" }
	};
	int N_ir_menu_table = N_items_in_array( ir_menu_table );

// NTL minimum is below base pack
#define NTL_more_than_min 0

// CABLE CHANNELS (but not controls)

	ir_cable_channel ir_cable_channel_table[] =
	{

	 { M01,  101, 1.0,	"BBC One"		},
	 { M01,  102, 1.0,	"BBC Two"		},
	 { M01,  103, 1.0,	"ITV Meridian"		},
	 { M01,  104, 1.0,	"Four"			},
#if NTL_more_than_min
	 { M01,  105, 1.0,	"Five"			},
#endif
	 { M01,  117, 1.0,	"ITV2"			},
//	 { M01,  118, 1.0,	"Studio"		}, // dead
	 { M01,  126, 1.0,	"BBC Three"		},
	 { M01,  127, 1.0,	"BBC Four"		},

#if NTL_more_than_min
	 { M01,  0, 1.0,	"-"		},

	 { M02,  113, 1.0,	"Bloomberg"		},
	 { M02,  121, 1.0,	"Sky News"		},
	 { M02,  123, 1.0,	"ITV News"		},
	 { M02,  124, 1.0,	"CNBC"			},
#endif
	 { M02,  125, 1.0,	"BBC News"		},
#if NTL_more_than_min
	 { M02,  128, 1.0,	"BBC Parliament"	},
	 { M02,  607, 1.0,	"UK History"		},
	 { M02,  300, 1.0,	"UK Gold"		},
	 { M02,  0, 1.0,	"-"		},

	 { M03,  106, 1.0,	"Guest-XXX"		},
	 { M03,  140, 1.0,	"Sky One"		},
//	 { M03,  817, 1.0,	"Smash Hits"		}, // now pay
	 { M03,  910, 1.0,	"The Hits"		},
	 { M03,  911, 1.0,	"The Music Factory"	},
	 { M03,  913, 1.0,	"UK Bright Ideas"	},
	 { M03,  914, 1.0,	"FTN"			},
	 { M03,  598, 1.0,	"CBBC"			},
	 { M03,  599, 1.0,	"BBC Cbeebies"		},

	 { M04,  858, 1.0,	"RAD BBC 1"		},
	 { M04,  859, 1.0,	"RAD BBC 1Xtra"		},
	 { M04,  860, 1.0,	"RAD BBC 2"		},
	 { M04,  861, 1.0,	"RAD BBC 3"		},
	 { M04,  862, 1.0,	"RAD BBC 4"		},
	 { M04,  863, 1.0,	"RAD BBC 5 live"	},
	 { M04,  864, 1.0,	"RAD BBC 5 live Extra"	},
	 { M04,  865, 1.0,	"RAD BBC 6 Music"	},
	 { M04,  867, 1.0,	"RAD BBC Asia Service"	},
	 { M04,  868, 1.0,	"RAD BBC World Service"	},
	 { M04,  869, 1.0,	"RAD BBC Scotland"	},
	 { M04,  870, 1.0,	"RAD BBC Wales"	},
	 { M04,  871, 1.0,	"RAD BBC Ulster"	},
	 { M04,  872, 1.0,	"RAD Primetime"	},
	 { M04,  873, 1.0,	"RAD Magic"	},
	 { M04,  874, 1.0,	"RAD Total Rock"	},
	 { M04,  875, 1.0,	"RAD UCB Europe"	},
	 { M04,  876, 1.0,	"RAD Jazz FM"		},
	 { M04,  877, 1.0,	"RAD Classic FM"	},
	 { M04,  878, 1.0,	"RAD Q"	},
	 { M04,  879, 1.0,	"RAD Core Radio"	},
	 { M04,  880, 1.0,	"RAD Planet Rock"	},
	 { M04,  881, 1.0,	"RAD The Storm"		},
	 { M04,  882, 1.0,	"RAD The Mix"		},
	 { M04,  883, 1.0,	"RAD Heart FM"		},
	 { M04,  884, 1.0,	"RAD The HITS"		},
	 { M04,  885, 1.0,	"RAD Century"		},
	 { M04,  886, 1.0,	"RAD Christian Premier"	},
	 { M04,  887, 1.0,	"RAD Galaxy"	},
	 { M04,  888, 1.0,	"RAD Capital"	},
	 { M04,  889, 1.0,	"RAD Capital Disney"		},
	 { M04,  890, 1.0,	"RAD Capital Gold"		},
	 { M04,  891, 1.0,	"RAD XFM"		},
	 { M04,  892, 1.0,	"RAD Virgin"		},
	 { M04,  893, 1.0,	"RAD Oneword - classic comedy"		},
	 { M04,  894, 1.0,	"RAD Kiss"		},
	 { M04,  895, 1.0,	"RAD Kerrang"		},
	 { M04,  896, 1.0,	"RAD SmashHits"		},
	 { M04,  897, 1.0,	"RAD Cymru"		},
	 { M04,  898, 1.0,	"RAD RnanGaidhea"	},
	 { M04,  899, 1.0,	"RAD Talk Sport"	},

	 { M08,  112, 1.0,	"Euro Sport"		},
	 { M08,  908, 1.0,	"At the races"		},
	 { M08,  0, 1.0,	"-"		},

	 { M07,  120, 1.0,	"Info Channel"		},
	 { M07,  107, 1.0,	"Front Row Guide"	},
	 { M07,  200, 1.0,	"Front Row Guide"	},
	 { M07,  233, 1.0,	"On Demand"		},
	 { M07,  116, 1.0,	"MTV2"			},
	 { M07,  604, 1.0,	"MTV"			},
	 { M07,  609, 1.0,	"MTV Base"		},
	 { M07,  610, 1.0,	"MTV Hits"		},
	 { M07,  144, 1.0,	"E4"			},
	 { M07,  307, 1.0,	"Challenge"		},
	 { M07,  502, 1.0,	"CNN"		},
	 { M07,  504, 1.0,	"History"		},
	 { M07,  740, 1.0,	"SET Asia"		},
	 { M07,  0, 1.0,	"-"		},

	 { M08,  851, 1.0,	"ITV Carlton"		},
	 { M08,  114, 1.0,	"QVC Shopping"		},
	 { M08,  115, 1.0,	"TV Travel Shop"	},
	 { M08,  119, 1.0,	"buy ntl"		},
	 { M08,  633, 1.0,	"TV Travel Shop 2"	},
	 { M08,  647, 1.0,	"SHOP www.Bid-Up.tv"		},
	 { M08,  854, 1.0,	"Simply Shopping"	},
	 { M08,  855, 1.0,	"Ideal World"		},
	 { M08,  856, 1.0,	"Screen Shop"		},
	 { M08,  857, 1.0,	"Thomas Cook TV"	},
	 { M08,  902, 1.0,	"TV WareHouse"		},
	 { M08,  907, 1.0,	"Thane Dir"		},
	 { M08,  923, 1.0,	"Stop n shop/"		}, // Thane
	 { M08,  924, 1.0,	"Auction"		},
	 { M08,  812, 1.0,	"Sky Sports News"	},
	 { M08,  930, 1.0,	"Sky One Mix"		},
	 { M08,  931, 1.0,	"Sky Travel"		},
	 { M08,  932, 1.0,	"Sky Travel Extra"	}
#endif

	};
	int N_ir_cable_channel_table = N_items_in_array( ir_cable_channel_table );

/*!
	Channels and commands that Infra-Red can send/select
*/

	ir_command_info vcr_play_pause = {
	  M11, " VCR PLAY_PAUSE",	"VCR",	"PLAY_PAUSE"
	};

	ir_command_info ir_command_table[] = {

	 { M10,  " VCR PLAY_PAUSE",	"VCR",	"PLAY_PAUSE"	},
	 { M10,  "-",	"-",	"-"	},

	 { M10,  " VCR POWER",		"VCR",	"POWER"		},
	 { M10,  " VCR INPUT",		"VCR",	"INPUT"		},
	 { M10,  " VCR EJECT",		"VCR",	"EJECT"		},
	 { M10,  " VCR SPEED",		"VCR",	"SPEED"		},
	 { M10,  " VCR RECORD_PAUSE",	"VCR",	"RECORD_PAUSE"	},
	 { M10,  "-",	"-",	"-"	},

	 { M10,  " VCR REWIND",		"VCR",	"REWIND"	},
	 { M10,  " VCR FORWARD",	"VCR",	"FORWARD"	},
	 { M10,  " VCR STOP",		"VCR",	"STOP"		},
	 { M10,  " VCR PROG-DOWN",	"VCR",	"PROG-DOWN"	},
	 { M10,  " VCR PROG-UP",	"VCR",	"PROG-UP"	},
	 { M10,  " VCR INPUT",		"VCR",	"INPUT"		},
	 { M10,  "-",	"-",	"-"	},

	 { M10,  " VCR DISPLAY",	"VCR",	"DISPLAY"	},
	 { M10,  " VCR MENU",		"VCR",	"MENU"		},
	 { M10,  " VCR clear_reset",	"VCR",	"clear_reset"	},
	 { M10,  " VCR OK",		"VCR",	"OK"		},
	 { M10,  " vcr LEFT",		"VCR",	"LEFT"		},
	 { M10,  " vcr RIGHT",		"VCR",	"RIGHT"		},
	 { M10,  " vcr UP",		"VCR",	"UP"		},
	 { M10,  " vcr DOWN",		"VCR",	"DOWN"		},
	 { M10,  "-",	"-",	"-"	},

	 { M10,  " VCR AUDIO",		"VCR",	"AUDIO"		},
	 { M10,  " VCR INDEX",		"VCR",	"INDEX"		},
	 { M10,  " VCR timer",		"VCR",	"timer"		},
	 { M10,  " VCR IPC",		"VCR",	"IPC"		},
	 { M10,  " VCR SPEED",		"VCR",	"SPEED"		},
	 { M10,  " VCR TRK-DOWN",	"VCR",	"TRK-DOWN"	},
	 { M10,  " VCR TRK-UP",		"VCR",	"TRK-UP"	},
	 { M10,  "-",	"-",	"-"	},

	 { M10,  " vcr A.TRK",		"VCR",	"A.TRK"		},
	 { M10,  " vcr DUB",		"VCR",	"DUB"		},
	 { M10,  " vcr TV_VCR",		"VCR",	"TV_VCR"	},
	 { M10,  " vcr SHOW-VIEW",	"VCR",	"SHOW-VIEW"	},
	 { M10,  "-",	"-",	"-"	},

	 { M11,  "NTL 1 0 1"	,	"CABLE",	"1 0 1"	},
	 { M11,  "NTL Channel-",	"CABLE",	"Channel-"	},
	 { M11,  "NTL Channel+",	"CABLE",	"Channel+"	},
	 { M11,  "NTL Select",		"CABLE",	"Select"	},
	 { M11,  "NTL Browse",		"CABLE",	"Browse"	},
	 { M11,  "NTL Back", 		"CABLE",	"Back"		},
	 { M11,  "NTL Help", 		"CABLE",	"Help"		},
	 { M11,  "-",	"-",	"-"	},

	 { M11,  "NTL Red",		"CABLE",	"Red"		},
	 { M11,  "NTL Green",		"CABLE",	"Green"		},
	 { M11,  "NTL Yellow",		"CABLE",	"Yellow"	},
	 { M11,  "NTL Blue", 		"CABLE",	"Blue"		},
	 { M11,  "-",	"-",	"-"	},

	 { M11,  "NTL Left", 		"CABLE",	"Left"		},
	 { M11,  "NTL Right",		"CABLE",	"Right"		},
	 { M11,  "NTL Up",		"CABLE",	"Up"		},
	 { M11,  "NTL Down", 		"CABLE",	"Down"		},
	 { M11,  "-",	"-",	"-"	},

	 { M11,  "NTL Menu", 		"CABLE",	"Menu"		},
	 { M11,  "NTL Email",		"CABLE",	"Email"		},
	 { M11,  "NTL Internet",	"CABLE",	"Internet"	},
	 { M11,  "NTL TV",		"CABLE",	"TV"		},
	 { M11,  "NTL Interactive",	"CABLE",	"Interactive"	},
	 { M11,  "NTL Guide",		"CABLE",	"Guide"		},
	 { M11,  "-",	"-",	"-"	},

	 { M11,  "NTL Standby",		"CABLE",	"Standby"	},
	 { M11,  "NTL On_Demand",	"CABLE",	"On_Demand"	},
	 { M11,  "NTL Page_Up",		"CABLE",	"Page_Up"	},
	 { M11,  "NTL Page_Down",	"CABLE",	"Page_Down"	},
	 { M11,  "NTL Audio",		"CABLE",	"Audio"		},
	 { M11,  "NTL Volume+",		"CABLE",	"Volume+"	},
	 { M11,  "NTL Volume-",		"CABLE",	"Volume-"	},
	 { M11,  "NTL Mute",		"CABLE",	"Mute"		},
	 { M11,  "-",	"-",	"-"	},

	 { M11,  "NTL 1",		"CABLE",	"1"		},
	 { M11,  "NTL 2 abc",		"CABLE",	"2"		},
	 { M11,  "NTL 3 def",		"CABLE",	"3"		},
	 { M11,  "NTL 4 ghi",		"CABLE",	"4"		},
	 { M11,  "NTL 5 jkl",		"CABLE",	"5"		},
	 { M11,  "NTL 6 mno",		"CABLE",	"6"		},
	 { M11,  "NTL 7 pqrs",		"CABLE",	"7"		},
	 { M11,  "NTL 8 tuv",		"CABLE",	"8"		},
	 { M11,  "NTL 9 wxyz",		"CABLE",	"9"		},
	 { M11,  "NTL 0",		"CABLE",	"0"		},
	 { M11,  "-",	"-",	"-"	},

	 { M12,  "LS mem_up",		"LS",	"mem_up"	},
	 { M12,  "LS vol_up",		"LS",	"vol_up"	},
	 { M12,  "LS vol_down",		"LS",	"vol_down"	},
	 { M12,  "LS tune_down",	"LS",	"tune_down"	},
	 { M12,  "LS tune_up",		"LS",	"tune_up"	},
	 { M12,  "LS scan_up",		"LS",	"scan_up"	},
	 { M12,  "LS mem_prog",		"LS",	"mem_prog"	},
	 { M12,  "LS intro",		"LS",	"intro"		},
	 { M12,  "LS random",		"LS",	"random"	},
	 { M12,  "LS mode",		"LS",	"mode"		},
	 { M12,  "LS play_pause",	"LS",	"play_pause"	},
	 { M12,  "LS mute",		"LS",	"mute"		},
	 { M12,  "LS power",		"LS",	"power"		},
	 { M12,  "LS stop",		"LS",	"stop"		},
	 { M12,  "-",	"-",	"-"	},

	 { M13,  " GRUNDIG",		"UMS9V",	"GRUNDIG"	},
	 { M13,  " bass",		"UMS9V",	"BASS"	},
	 { M13,  " sleep",		"UMS9V",	"sleep"	},
	 { M13,  " timer",		"UMS9V",	"timer"	},
	 { M13,  " MODE_remain",	"UMS9V",	"MODE_remain"	},
	 { M13,  " INTRO",		"UMS9V",	"INTRO"	},
	 { M13,  " MEMORY_clock",	"UMS9V",	"MEMORY_clock"	},
	 { M13,  " RANDOM",		"UMS9V",	"RANDOM"	},
	 { M13,  " CD",			"UMS9V",	"CD"	},
	 { M13,  " TUNER",		"UMS9V",	"TUNER"	},
	 { M13,  " TAPE",		"UMS9V",	"TAPE"	},
	 { M13,  " REPEAT_PRESET",	"UMS9V",	"REPEAT_PRESET"	},
	 { M13,  " BAND",		"UMS9V",	"BAND"	},
	 { M13,  " TUNE_UP",		"UMS9V",	"TUNE_UP"	},
	 { M13,  " TUNE_DOWN",		"UMS9V",	"TUNE_DOWN"	},
	 { M13,  " VOL_UP",		"UMS9V",	"VOL_UP"	},
	 { M13,  " VOL_DOWN",		"UMS9V",	"VOL_DOWN"	},
	 { M13,  " STOP",		"UMS9V",	"STOP"	},
	 { M13,  " PLAY",		"UMS9V",	"PLAY"	},
	 { M13,  " POWER",		"UMS9V",	"POWER"	},
	 { M13,  "-",	"-",	"-"	},

	 { M14,  "FV 1",		"FV",	"1"	},
	 { M14,  "FV 2",		"FV",	"2"	},
	 { M14,  "FV 3",		"FV",	"3"	},
	 { M14,  "FV 4",		"FV",	"4"	},
	 { M14,  "FV 5",		"FV",	"5"	},
	 { M14,  "FV 6",		"FV",	"6"	},
	 { M14,  "FV 7",		"FV",	"7"	},
	 { M14,  "FV 8",		"FV",	"8"	},
	 { M14,  "FV 9",		"FV",	"9"	},
	 { M14,  "FV 0",		"FV",	"0"	},
	 { M14,  "FV power",		"FV",	"power"	},
	 { M14,  "FV mute",		"FV",	"mute"	},
	 { M14,  "FV red",		"FV",	"red"	},
	 { M14,  "FV green",		"FV",	"green"	},
	 { M14,  "FV yellow",		"FV",	"yellow"	},
	 { M14,  "FV guide",		"FV",	"guide"	},
	 { M14,  "FV info",		"FV",	"info"	},
	 { M14,  "FV up",		"FV",	"up"	},
	 { M14,  "FV down",		"FV",	"down"	},
	 { M14,  "FV left",		"FV",	"left"	},
	 { M14,  "FV right",		"FV",	"right"	},
	 { M14,  "FV text",		"FV",	"text"	},
	 { M14,  "FV cancel",		"FV",	"cancel"	},
	 { M14,  "FV wide",		"FV",	"wide"	},
	 { M14,  "FV status",		"FV",	"status"	},
	 { M14,  "FV subtitles",	"FV",	"subtitles"	},
	 { M14,  "FV swap",		"FV",	"swap"	},
	 { M14,  "FV audio",		"FV",	"audio"	},


////////--------------------------------
//                                  Transmitter
//   LCN               Channel Name Hannington

//                                                    GENERAL ENTERTAINMENT

   { M15,  "-- GENERAL -",	"-",	"-"	},
   { M15, "01  BBC ONE",	"FV", "0 1" },
   { M15, "02  BBC TWO",	"FV", "0 2" },
   { M15, "03  ITV1",		"FV", "0 3" },
   { M15, "04  Channel 4",	"FV", "0 4" },
   { M15, "05  five",		"FV", "0 5" },
   { M15, "06  ITV2",		"FV", "0 6" },
   { M15, "07  BBC THREE",	"FV", "0 7" },
   { M15, "09  BBC FOUR",	"FV", "0 9" },
   { M15, "10  ITV3",		"FV", "1 0" },
   { M15, "11  Sky Three",	"FV", "1 1" },
   { M15, "12  UKTV History",	"FV", "1 2" },
   { M15, "13  Channel 4+1",	"FV", "1 3" },
   { M15, "14  More4",		"FV", "1 4" },
   { M15, "15  ABC1",		"FV", "1 5" },
   { M15, "16  QVC",		"FV", "1 6" },
   { M15, "18  The Hits",	"FV", "1 8" },
   { M15, "19  UK Bright Ideas", "FV", "1 9" },
   { M15, "20  f tn",		"FV", "2 0" },
   { M15, "21  TMF",		"FV", "2 1" },
   { M15, "22  Ideal World",	"FV", "2 2" },
   { M15, "23  bid tv",		"FV", "2 3" },
   { M15, "24  price-drop.tv",	"FV", "2 4" },
   { M15, "28  ITV4",		"FV", "2 8" },
   { M15, "29  E4",		"FV", "2 9" },
   { M15, "30  E4+1",		"FV", "3 0" },
   { M15, "31  ITV2+1",		"FV", "3 1" },
   { M15, "32  FILM4",		"FV", "3 2" },
   { M15, "34  SETANTA Sports",	"FV", "3 4" },
   { M15, "35  Five US",	"FV", "3 5" },
   { M15, "36  Five Life",	"FV", "3 6" },
   { M15, "37  Smile TV",	"FV", "3 7" },
   { M15, "41  THOMAS COOK TV",	"FV", "4 1" },

   { M15, "70  CBBC",		"FV", "7 0" },
   { M15, "74  CBeeBies",	"FV", "7 4" },
   { M15, "75  CITV",		"FV", "7 5" },
   { M15, "80  BBC NEWS 24",	"FV", "7 0" },
   { M15, "81  BBC Parliament",	"FV", "8 1" },
   { M15, "82  Sky News ",	"FV", "8 2" },
   { M15, "83  Sky Sport News ", "FV", "8 3" },
   { M15, "87  Community",	"FV", "8 7" },
   { M15, "88  Teachers TV",	"FV", "8 8" },
   { M15, "97  Television X",	"FV", "9 7" },

   { M15, "100  Teletext",	"FV", "1 0 0" },
   { M15, "101  Teletext Hols",	"FV", "1 0 1" },
   { M15, "102  Teletext Cars",	"FV", "1 0 2" },
   { M15, "103  Teletext Games","FV", "1 0 3" },
   { M15, "104  Teletext on 4",	"FV", "1 0 4" },
   { M15, "105  BBCi",		"FV", "1 0 5" },
   { M15, "108  Sky Text",	"FV", "1 0 8" },
   { M15, "300  4TVInteractive", "FV", "3 0 0" },
   { M15, "301  X-301",		"FV", "3 0 1" },
   { M15, "302  X-302",		"FV", "3 0 2" },
   { M15, "303  X-303",		"FV", "3 0 3" },
   { M15, "304  X-304",		"FV", "3 0 4" },
   { M15, "305  X-305",		"FV", "3 0 5" },

//                                                                    RADIO

   { M15,  "-- RADIO -",	"-",	"-"	},
   { M15, "700  BBC Radio 1",		"FV", "7 0 0" },
   { M15, "701  1Xtra BBC",		"FV", "7 0 1" },
   { M15, "702  BBC Radio 2",		"FV", "7 0 2" },
   { M15, "703  BBC Radio 3",		"FV", "7 0 3" },
   { M15, "704  BBC Radio 4",		"FV", "7 0 4" },
   { M15, "705  Radio 5 Live",		"FV", "7 0 5" },
   { M15, "706  BBC Five LiveSportsX",	"FV", "7 0 6" },
   { M15, "707  BBC 6 Music",		"FV", "7 0 7" },
   { M15, "708  BBC 7",			"FV", "7 0 8" },
   { M15, "709  BBC Asian Network",	"FV", "7 0 9" },
   { M15, "710  BBC World Service",	"FV", "7 1 0" },
   { M15, "711  The Hits Radio",	"FV", "7 1 1" },
   { M15, "712  Smash Hits!",		"FV", "7 1 2" },
   { M15, "713  Kiss",			"FV", "7 1 3" },
   { M15, "714  Heat",			"FV", "7 1 4" },
   { M15, "715  Magic",			"FV", "7 1 5" },
   { M15, "716  Q",			"FV", "7 1 6" },
   { M15, "717  Oneword",		"FV", "7 1 7" },
   { M15, "718  SMOOTH Radio",		"FV", "7 1 8" },
   { M15, "721  Mojo",			"FV", "7 2 1" },
   { M15, "722  Kerrang!",		"FV", "7 2 2" },

   { M15, "723  talkSPORT",		"FV", "7 2 3" },
   { M15, "724  Clyde 1",		"FV", "7 2 4" },
   { M15, "725  Premier",		"FV", "7 2 5" },
   { M15, "727  Virgin",		"FV", "7 2 7" },
   { M15, "728  Heart",			"FV", "7 2 8" },
   { M15, "729  RadioMusicShop",	"FV", "7 2 9" },

/*
	all these give INVALID CHANNEL
	you need a FreeView with a TopUp-TV card slot
//                                                             PAY SERVICES
   { M15,  "-- PAY -",	"-",	"-"	},
   { M15, "14  E4",			"FV", "1 4" },
   { M15, "17  UK Gold",		"FV", "1 7" },
   { M15, "25  TCM",			"FV", "2 5" },
   { M15, "26  UK Style",		"FV", "2 6" },
   { M15, "27  Discovery",		"FV", "2 7" },
   { M15, "28  Discovery H&L",		"FV", "2 8" },
   { M15, "29  UK Food",		"FV", "2 9" },
   { M15, "32  Cartoon Network",	"FV", "3 2" },
   { M15, "33  Boomerang",		"FV", "3 3" },
   { M15, "44  Bloomberg",		"FV", "4 4" },
   { M15, "60  Television X",		"FV", "6 0" },
*/


	 { M12,  "X",		"X",	"X"	}

	};
	int N_ir_command_table = N_items_in_array( ir_command_table );

}; // NAMESPACE
